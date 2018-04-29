#include "Game.h"
//#include <SFML/Graphics.hpp>
#include <iostream>

Game::Game() : m_window(sf::Vector2u(800, 600), "Snake"),
	m_world(sf::Vector2u(800, 600)), 
	m_snake(m_world.GetBlockSize())
{
	//Is it work?
}

Game::~Game()
{
}

void Game::InputHandler()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
		m_snake.GetDirection() != Direction::Down)
		m_snake.SetDirection(Direction::Up);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
		m_snake.GetDirection() != Direction::Up)
		m_snake.SetDirection(Direction::Down);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
		m_snake.GetDirection() != Direction::Right)
		m_snake.SetDirection(Direction::Left);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
		m_snake.GetDirection() != Direction::Left)
		m_snake.SetDirection(Direction::Right);

}

void Game::Update()
{
	m_window.Update();
	float timestep = 1.0f / m_snake.GetSpeed();

	//Bug here
	InputHandler();

	if (m_elapsed.asSeconds() >= timestep)
	{
		//InputHandler(); -> this is not a good idea.
		m_snake.Tick();
		m_world.Update(m_snake);
		m_elapsed -= sf::seconds(timestep);
		if (m_snake.HasLost())
			m_snake.Reset();
	}
}

void Game::Render()
{
	m_window.BeginDraw();
	//..
	m_world.Render(m_window.GetRenderWindow());
	m_snake.Render(m_window.GetRenderWindow());
	m_window.EndDraw();
}
