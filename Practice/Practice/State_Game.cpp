#include "State_Game.h"

State_Game::State_Game(StateManager * stateMgr)
	: BaseState(stateMgr), m_increment(sf::Vector2i(400, 400))
{
	//Texture & Sprite setting
	m_texture.loadFromFile("Mushroom.jpg");
	m_sprite.setTexture(m_texture);
	m_sprite.setOrigin(m_texture.getSize().x / 2.0f,
		m_texture.getSize().y / 2.0f);

	sf::Vector2u windowSize =
		m_stateMgr->GetContext()->m_window->GetWindowSize();
	m_sprite.setPosition(windowSize.x / 2.0f,
		windowSize.y / 2.0f);
}

void State_Game::Update(const sf::Time & elapsed)
{
	MoveMushroom(elapsed);
}

void State_Game::Draw()
{
	m_stateMgr->GetContext()->m_window->
		GetRenderWindow()->draw(m_sprite);
}

void State_Game::MoveMushroom(const sf::Time & elapsed)
{
	sf::Vector2u windowSize = 
		m_stateMgr->GetContext()->m_window->GetWindowSize();

	if ((m_sprite.getPosition().x >
		windowSize.x - m_texture.getSize().x / 2.0f
		&& m_increment.x > 0)
		||
		(m_sprite.getPosition().x -
			m_texture.getSize().x / 2.0f < 0 && m_increment.x < 0))
	{
		m_increment.x = -m_increment.x;
	}
	if ((m_sprite.getPosition().y >
		windowSize.y - m_texture.getSize().y / 2.0f
		&& m_increment.y > 0)
		||
		(m_sprite.getPosition().y -
			m_texture.getSize().y / 2.0f < 0 && m_increment.y < 0))
	{
		m_increment.y = -m_increment.y;
	}

	m_sprite.setPosition(
		m_sprite.getPosition().x +
		(m_increment.x * elapsed.asSeconds()),
		m_sprite.getPosition().y +
		(m_increment.y * elapsed.asSeconds()));
}
