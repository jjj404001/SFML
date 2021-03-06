#include "Character.h"
#include "EntityManager.h"
#include "Anim_Base.h"

Character::Character(EntityManager * entityMgr)
	: EntityBase(entityMgr), 
	m_spriteSheet(m_entityManager->GetContext()->m_textureManager),
	m_jumpVelocity(250), m_hitpoints(5)
{
	m_name = "Character";
	//m_spriteSheet.SetAnimation("Idle", true, true);
	
}

void Character::Move(const Direction & dir)
{
	if (GetState() == EntityState::Dying)
		return;
	m_spriteSheet.SetDirection(dir);
	if (dir == Direction::Left)
		Accelerate(-m_speed.x, 0);
	else
		Accelerate(m_speed.x, 0);

	if (GetState() == EntityState::Idle)
		SetState(EntityState::Walking);
}

void Character::Jump()
{
	if (GetState() == EntityState::Dying ||
		GetState() == EntityState::Jumping ||
		GetState() == EntityState::Hurt)
		return;

	SetState(EntityState::Jumping);
	AddVelocity(0, -m_jumpVelocity);
}

void Character::Attack()
{
	if (GetState() == EntityState::Dying ||
		GetState() == EntityState::Jumping ||
		GetState() == EntityState::Hurt ||
		GetState() == EntityState::Attacking)
		return;
	SetState(EntityState::Attacking);
}

void Character::GetHurt(const int damage)
{
	if (GetState() == EntityState::Dying ||
		GetState() == EntityState::Hurt)
		return;
	m_hitpoints = (m_hitpoints - damage > 0 ?
		m_hitpoints - damage : 0);
	if (m_hitpoints)
		SetState(EntityState::Hurt);
	else
		SetState(EntityState::Dying);
}

void Character::Load(const std::string & path)
{
	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() + path);
	if (!file.is_open())
		std::cout << "! Cannot open the file: "
		<< path << std::endl;

	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream keystream(line);
		std::string type;
		keystream >> type;

		if (type == "Name")
			keystream >> m_name;
		else if (type == "Spritesheet")
		{
			std::string path;
			keystream >> path;
			m_spriteSheet.LoadSheet("media/SpriteSheets/" + path);
		}
		else if (type == "Hitpoints")
			keystream >> m_hitpoints;
		else if (type == "BoundingBox")
		{
			sf::Vector2f boundingSize;
			keystream >> boundingSize.x >> boundingSize.y;
			SetSize(boundingSize.x, boundingSize.y);
		}
		else if (type == "DamageBox")
		{
			keystream >> m_attackAABBoffset.x >> m_attackAABBoffset.y
				>> m_attackAABB.width >> m_attackAABB.height;
		}
		else if (type == "Speed")
			keystream >> m_speed.x >> m_speed.y;
		else if (type == "JumpVelocity")
			keystream >> m_jumpVelocity;
		else if (type == "MaxVelocity")
			keystream >> m_maxVelocity.x >> m_maxVelocity.y;
		else
			std::cout << "! Unknown type in character file: "
			<< type << std::endl;
	}
	file.close();
}

void Character::Update(float dt)
{
	EntityBase::Update(dt);
	if (m_attackAABB.width != 0 && m_attackAABB.height != 0)
		UpdateAttackAABB();
	if (GetState() != EntityState::Dying &&
		GetState() != EntityState::Attacking &&
		GetState() != EntityState::Hurt)
	{
		if (abs(m_velocity.y) >= 0.001f)
			SetState(EntityState::Jumping);
		else if (abs(m_velocity.x) >= 0.1f)
			SetState(EntityState::Walking);
		else
			SetState(EntityState::Idle);
	}
	else if (GetState() == EntityState::Attacking ||
		GetState() == EntityState::Hurt)
	{
		if (!m_spriteSheet.GetCurrentAnim()->IsPlaying())
			SetState(EntityState::Idle);
	}
	else if (GetState() == EntityState::Dying)
	{
		if (!m_spriteSheet.GetCurrentAnim()->IsPlaying())
			m_entityManager->Remove(m_id);
	}
	Animate();
	m_spriteSheet.Update(dt);
	m_spriteSheet.SetSpritePosition(m_position);
}

void Character::Draw(sf::RenderWindow * wind)
{
	m_spriteSheet.Draw(wind);
	wind->draw(m_AABBshape);
	wind->draw(m_attackShape);
}

void Character::UpdateAttackAABB()
{
	m_attackAABB.left =
		(m_spriteSheet.GetDirection() == Direction::Left ?
		 m_AABB.left - m_attackAABB.width
			: m_AABB.left + m_AABB.width);

	m_attackAABB.top = m_AABB.top + m_attackAABBoffset.y;
	m_attackShape.setSize(sf::Vector2f(m_attackAABB.width, m_attackAABB.height));
	m_attackShape.setPosition(sf::Vector2f(m_attackAABB.left, m_attackAABB.top));

	m_attackShape.setFillColor(sf::Color(128, 0, 0, 128));
}

void Character::Animate()
{
	EntityState state = GetState();

	if (state == EntityState::Walking &&
		m_spriteSheet.GetCurrentAnim()->GetName() != "Walk")
		m_spriteSheet.SetAnimation("Walk", true, true);
	else if (state == EntityState::Jumping &&
		m_spriteSheet.GetCurrentAnim()->GetName() != "Jump")
		m_spriteSheet.SetAnimation("Jump", true, false);
	else if (state == EntityState::Attacking &&
		m_spriteSheet.GetCurrentAnim()->GetName() != "Attack")
		m_spriteSheet.SetAnimation("Attack", true, false);
	else if (state == EntityState::Hurt &&
		m_spriteSheet.GetCurrentAnim()->GetName() != "Hurt")
		m_spriteSheet.SetAnimation("Hurt", true, false);
	else if (state == EntityState::Dying &&
		m_spriteSheet.GetCurrentAnim()->GetName() != "Death")
		m_spriteSheet.SetAnimation("Death", true, false);
	else if (state == EntityState::Idle &&
		m_spriteSheet.GetCurrentAnim()->GetName() != "Idle")
		m_spriteSheet.SetAnimation("Idle", true, true);
}
