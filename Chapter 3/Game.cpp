#include "Game.h"
#include "Textbox.h"
#include <iostream>

Game::Game() : m_window(sf::Vector2u(800, 600), "Snake"),
	m_world(sf::Vector2u(800, 600)), 
	m_snake(m_world.GetBlockSize()) 
{
	m_world.RespawnApple(m_snake);
	m_textbox.Setup(5, 14, 350, sf::Vector2f(225, 0));
	m_textbox.Add("Random number generator is initialized with: "
		+ std::to_string(time(NULL)));
}

Game::~Game()
{
}

void Game::InputHandler()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
		m_snake.GetPhysicalDirection() != Direction::Down)
		m_snake.SetDirection(Direction::Up);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
		m_snake.GetPhysicalDirection() != Direction::Up)
		m_snake.SetDirection(Direction::Down);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
		m_snake.GetPhysicalDirection() != Direction::Right)
		m_snake.SetDirection(Direction::Left);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
		m_snake.GetPhysicalDirection() != Direction::Left)
		m_snake.SetDirection(Direction::Right);
}

void Game::Update()
{
	m_window.Update();
	float timestep = 1.0f / m_snake.GetSpeed();

	InputHandler();

	if (m_elapsed.asSeconds() >= timestep)
	{
		m_snake.Tick();
		m_world.Update(m_snake, m_textbox);
		m_elapsed -= sf::seconds(timestep);
		if (m_snake.HasLost())
		{
			m_textbox.Add(
				"GAME OVER! Score: " 
				+ std::to_string(m_snake.GetScore()));
			m_snake.Reset();
		}
	}
}

void Game::Render()
{
	m_window.BeginDraw();
	m_world.Render(m_window.GetRenderWindow());
	m_snake.Render(m_window.GetRenderWindow());
	m_textbox.Render(m_window.GetRenderWindow());

	m_window.EndDraw();
}
