#include "EntityManager.h"

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

}

void EntityBase::AddVelocity(float x, float y)
{

}

void EntityBase::Accelerate(float x, float y)
{

}

void EntityBase::SetAccelerate(float x, float y)
{

}
void EntityBase::ApplyFriction(float x, float y)
{

}
