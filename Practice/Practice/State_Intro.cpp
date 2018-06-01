#include "StateManager.h"
#include "State_Intro.h"

State_Intro::State_Intro(StateManager * stateMgr)
	: BaseState(stateMgr), m_increment(sf::Vector2f(400, 400))
{
	m_texture.loadFromFile("Mushroom.jpg");
	m_sprite.setTexture(m_texture);
	m_sprite.setOrigin(m_texture.getSize().x / 2, 
		m_texture.getSize().y / 2);

	sf::Vector2u windowSize =
		m_stateMgr->GetContext()->m_window->GetWindowSize();
	m_sprite.setPosition(windowSize.x / 2, windowSize.y / 2);

	m_stateMgr->GetContext()->m_evMgr->
		AddCallback(StateType::INTRO, &State_Intro::SwitchTo, this, "Move_To_Intro");
}

void State_Intro::Update(const sf::Time & elapsed)
{
	sf::Vector2u windowSize =
		m_stateMgr->GetContext()->m_window->GetWindowSize();

	if ((m_sprite.getPosition().x + 
		m_texture.getSize().x / 2 > windowSize.x) || 
		(m_sprite.getPosition().x -
		m_texture.getSize().x / 2 < 0))
		m_increment.x = -m_increment.x;

	m_sprite.setPosition(
		m_sprite.getPosition().x + m_increment.x * elapsed.asSeconds(),
		m_sprite.getPosition().y);
}

void State_Intro::Draw()
{
	m_stateMgr->GetContext()->m_window->
		GetRenderWindow()->draw(m_sprite);
}

void State_Intro::Create()
{
	//..
}

void State_Intro::Destroy()
{
	//..
}

void State_Intro::SwitchTo(EventDetails * details)
{
	std::cout << "aa" << std::endl;
	m_stateMgr->SwitchTo(StateType::GAME);
}
