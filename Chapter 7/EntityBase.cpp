#include "EntityBase.h"
#include "Map.h"
#include "EntityManager.h"

bool SortCollisions(const CollisionElement & lhs,
	const CollisionElement & rhs);

EntityBase::EntityBase(EntityManager * entityMgr)
	: m_entityManager(entityMgr), m_name("BaseEntity"),
	m_type(EntityType::Base), m_referenceTile(nullptr),
	m_state(EntityState::Idle), m_id(0),
	m_collidingOnX(false), m_collidingOnY(false) { }

//... Getters & Setters
void EntityBase::SetPosition(const float & x, const float & y)
{
	m_position.x = x;
	m_position.y = y;
	UpdateAABB();
}

void EntityBase::SetPosition(const sf::Vector2f & pos)
{
	m_position = pos;
	UpdateAABB();
}

void EntityBase::SetSize(const float & x, const float & y)
{
	m_size.x = x;
	m_size.y = y;
	UpdateAABB();
}

void EntityBase::SetState(const EntityState & state)
{
	if (m_state == EntityState::Dying)
		return;
	m_state = state;
}


void EntityBase::Move(float x, float y)
{
	m_positionOld = m_position;
	m_position += sf::Vector2f(x, y);
	sf::Vector2u mapSize = m_entityManager->
		GetContext()->m_gameMap->GetMapSize();

	if (m_position.x < 0)
		m_position.x = 0;
	else if (m_position.x > (mapSize.x + 1) * Sheet::Tile_Size)
		m_position.x = (mapSize.x + 1) * Sheet::Tile_Size;

	if (m_position.y < 0)
		m_position.y = 0;
	else if (m_position.y > (mapSize.y + 1) * Sheet::Tile_Size)
	{
		m_position.y = (mapSize.y + 1) * Sheet::Tile_Size;
		SetState(EntityState::Dying);
	}

	UpdateAABB();
}

void EntityBase::AddVelocity(float x, float y)
{
	m_velocity += sf::Vector2f(x, y);
	if (abs(m_velocity.x) > m_maxVelocity.x)
	{
		if (m_velocity.x < 0)
			m_velocity.x = -m_maxVelocity.x;
		else
			m_velocity.x = m_maxVelocity.x;
	}

	if (abs(m_velocity.y) > m_maxVelocity.y)
	{
		if (m_velocity.y < 0)
			m_velocity.y = -m_maxVelocity.y;
		else
			m_velocity.y = m_maxVelocity.y;
	}
}

void EntityBase::Accelerate(float x, float y)
{
	m_acceleration += sf::Vector2f(x, y);
}

void EntityBase::SetAccelerate(float x, float y)
{
	m_acceleration = sf::Vector2f(x, y);
}
void EntityBase::ApplyFriction(float x, float y)
{
	if (m_velocity.x != 0)
	{
		if (abs(m_velocity.x) - abs(x) < 0)
			m_velocity.x = 0;
		else
		{
			if (m_velocity.x < 0)
				m_velocity.x += x;
			else
				m_velocity.x -= x;
		}
	}

	if (m_velocity.y != 0)
	{
		if (abs(m_velocity.y) - abs(y) < 0)
			m_velocity.y = 0;
		else
		{
			if (m_velocity.y < 0)
				m_velocity.y += y;
			else
				m_velocity.y -= y;
		}
	}
}


void EntityBase::Update(float dt)
{
	Map * map = m_entityManager->GetContext()->m_gameMap;
	float gravity = map->GetGravity();
	Accelerate(0, gravity);
	AddVelocity(m_acceleration.x * dt, m_acceleration.y * dt);
	SetAccelerate(0.f, 0.f);

	sf::Vector2f frictionValue;
	if (m_referenceTile)
	{
		frictionValue = m_referenceTile->m_friction;
		if (m_referenceTile->m_deadly)
			SetState(EntityState::Dying);
	}
	else if (map->GetDefaultTile())
		frictionValue = map->GetDefaultTile()->m_friction;
	else
		frictionValue = m_friction;

	float friction_x = (m_speed.x * frictionValue.x) * dt;
	float friction_y = (m_speed.y * frictionValue.y) * dt;
	ApplyFriction(friction_x, friction_y);
	sf::Vector2f deltaPos = m_velocity * dt;
	Move(deltaPos.x, deltaPos.y);
	m_collidingOnX = false;
	m_collidingOnY = false;
	CheckCollisions();
	ResolveCollisions();
}

void EntityBase::UpdateAABB()
{
	m_AABB = sf::FloatRect(m_position.x - (m_size.x / 2),
		m_position.y - (m_size.y / 2), m_size.x, m_size.y);
	m_AABBshape.setSize(sf::Vector2f(m_size.x ,m_size.y));
	m_AABBshape.setPosition(sf::Vector2f(m_AABB.left, m_AABB.top));
	m_AABBshape.setFillColor(sf::Color(0, 128, 0, 128));
}

void EntityBase::CheckCollisions()
{
	Map * gameMap = m_entityManager->
		GetContext()->m_gameMap;
	unsigned int tileSize = gameMap->GetTileSize();
	int fromX = floor(m_AABB.left / tileSize);
	int toX = floor((m_AABB.left + m_AABB.width) / tileSize);
	int fromY = floor(m_AABB.top / tileSize);
	int toY = floor((m_AABB.top + m_AABB.height) / tileSize);

	for (int x = fromX; x <= toX; ++x)
	{
		for (int y = fromY; y <= toY; ++y)
		{
			Tile * tile = gameMap->GetTile(x, y);
			if (!tile)
				continue;

			sf::FloatRect tileBounds(x * tileSize,
				y * tileSize, tileSize, tileSize);
			sf::FloatRect intersection;
			m_AABB.intersects(tileBounds, intersection);
			float area = intersection.width * intersection.height;

			CollisionElement e(area, tile->m_properties, tileBounds);
			m_collisions.emplace_back(e);
			if (tile->m_warp && m_type == EntityType::Player)
				gameMap->LoadNext();
		}
	}

}

void EntityBase::ResolveCollisions()
{
	if (!m_collisions.empty())
	{
		std::sort(m_collisions.begin(),
			m_collisions.end(), SortCollisions);
		Map * gameMap = m_entityManager->GetContext()->m_gameMap;
		unsigned int tileSize = gameMap->GetTileSize();
		for (auto & itr : m_collisions)
		{
			if (!m_AABB.intersects(itr.m_tileBounds))
				continue;

			float xDiff = (m_AABB.left + (m_AABB.width / 2)) -
				(itr.m_tileBounds.left + (itr.m_tileBounds.width / 2));
			float yDiff = (m_AABB.top + (m_AABB.height / 2)) -
				(itr.m_tileBounds.top + (itr.m_tileBounds.height / 2));
			float resolve = 0;

			if (abs(xDiff) > abs(yDiff))
			{
				if (xDiff > 0)
				{
					resolve = (itr.m_tileBounds.left + tileSize) - 
						m_AABB.left;
				}
				else
				{
					resolve = -((m_AABB.left + m_AABB.width) -
						itr.m_tileBounds.left);
				}
				Move(resolve, 0);
				m_velocity.x = 0;
				m_collidingOnX = true;
			}
			else
			{
				if (yDiff > 0)
				{
					resolve = (itr.m_tileBounds.top + tileSize) -
						m_AABB.top;
				}
				else
				{
					resolve = -((m_AABB.top + m_AABB.height) -
						itr.m_tileBounds.top);
				}
				Move(0, resolve);
				m_velocity.y = 0;
				if (m_collidingOnY)
					continue;
				m_referenceTile = itr.m_tile;
				m_collidingOnY = true;
			}
		}
		m_collisions.clear();
	}
	if (!m_collidingOnY)
		m_referenceTile = nullptr;
}

bool SortCollisions(const CollisionElement & lhs,
	const CollisionElement & rhs)
{
	return lhs.m_area > rhs.m_area;
}