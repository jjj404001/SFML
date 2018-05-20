#include "Game.h"

Game::Game() : m_window()
{
	TextureLoading();
}

Game::Game(sf::Vector2u size, std::string & title)
	: m_window(size, title) 
{
	TextureLoading();
}

void Game::Update()
{
	m_window.Update();
	MoveMushroom();
}

void Game::Draw()
{
	m_window.StartDraw();
	//Draw something...
	m_window.GetRenderWindow()->draw(m_sprite);
	m_window.EndDraw();
}

void Game::MoveMushroom()
{
	sf::Vector2u windowSize = m_window.GetWindowSize();

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
		(m_increment.x * m_elapsed.asSeconds()),
		m_sprite.getPosition().y + 
		(m_increment.y * m_elapsed.asSeconds()));
}

void Game::TextureLoading()
{
	//Texture & Sprite setting
	m_texture.loadFromFile("Mushroom.jpg");
	m_sprite.setTexture(m_texture);
	m_sprite.setOrigin(m_texture.getSize().x / 2.0f,
		m_texture.getSize().y / 2.0f);

	sf::Vector2u windowSize = 
		GetWindow()->GetWindowSize();
	m_sprite.setPosition(windowSize.x / 2.0f,
		windowSize.y / 2.0f);

	m_increment = sf::Vector2i(400, 400);
}
