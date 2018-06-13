#include "Player.h"
#include "SpriteSheet.h"
#include "Anim_Base.h"

Player::Player(EntityManager * entityMgr)
	: Character(entityMgr)
{
	Load("Player.char");
	m_type = EntityType::Player;

	EventManager * events = m_entityManager->
		GetContext()->m_eventManager;
	events->AddCallback<Player>(StateType::Game,
		"Player_MoveLeft", &Player::React, this);
	events->AddCallback<Player>(StateType::Game,
		"Player_MoveRight", &Player::React, this);
	events->AddCallback<Player>(StateType::Game,
		"Player_Jump", &Player::React, this);
	events->AddCallback<Player>(StateType::Game,
		"Player_Attack", &Player::React, this);
}

Player::~Player()
{
	EventManager * events =
		m_entityManager->GetContext()->m_eventManager;
	events->RemoveCallback(StateType::Game, "Player_MoveLeft");
	events->RemoveCallback(StateType::Game, "Player_MoveRight");
	events->RemoveCallback(StateType::Game, "Player_Jump");
	events->RemoveCallback(StateType::Game, "Player_Attack");
}

void Player::OnEntityCollision(EntityBase * collider, bool attack)
{
	if (m_state == EntityState::Dying)
		return;
	if (attack)
	{
		if (m_state != EntityState::Attacking)
			return;
		if (!m_spriteSheet.GetCurrentAnim("Body")->IsInAction())
			return;
		if (collider->GetType() != EntityType::Enemy &&
			collider->GetType() != EntityType::Player)
			return;

		Character * opponent = (Character*)collider;
		opponent->GetHurt(1);
		if (m_position.x > opponent->GetPosition().x)
			opponent->AddVelocity(-32, 0);
		else
			opponent->AddVelocity(32, 0);
	}
	else
	{
		//Other behavior
	}
}

void Player::React(EventDetails * details)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		Move(Direction::Left);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		Move(Direction::Right);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		Jump();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		Attack();
}
