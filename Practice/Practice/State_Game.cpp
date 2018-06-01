#include "State_Game.h"

State_Game::State_Game(StateManager * stateMgr)
	: BaseState(stateMgr), m_increment(sf::Vector2i(400, 400)),
	  m_isPaused(false)
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

	m_rect.setFillColor(sf::Color(0, 0, 0, 200));
	m_rect.setSize(sf::Vector2f(windowSize));
	m_rect.setOrigin(windowSize.x / 2, windowSize.y / 2);
	m_rect.setPosition(windowSize.x / 2, windowSize.y / 2);

	m_font.loadFromFile("endomca.TTF");
	m_text.setFont(m_font);
	m_text.setCharacterSize(30);
	m_text.setString(sf::String("Paused"));
	m_text.setPosition(m_rect.getPosition());
	m_text.setOrigin(m_text.getLocalBounds().width / 2,
		m_text.getLocalBounds().height / 2);

	m_stateMgr->GetContext()->m_evMgr->
		AddCallback(StateType::GAME, &State_Game::TogglePaused,
			this, "Toggle_Paused");
}

void State_Game::Update(const sf::Time & elapsed)
{
	if (m_isPaused == false)
		MoveMushroom(elapsed);
	else
		;
}

void State_Game::Draw()
{
	m_stateMgr->GetContext()->m_window->
		GetRenderWindow()->draw(m_sprite);
	if (m_isPaused == true)
	{
		m_stateMgr->GetContext()->m_window->
			GetRenderWindow()->draw(m_rect);
		m_stateMgr->GetContext()->m_window->
			GetRenderWindow()->draw(m_text);
	}
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
