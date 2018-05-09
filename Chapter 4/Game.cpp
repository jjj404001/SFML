#include <iostream>
#include "Game.h"

//////
//Ctor
Game::Game() : m_window(sf::Vector2u(800, 600), "Chapter 4")
{
	m_texture.loadFromFile("Mushroom.png");
	m_sprite.setTexture(m_texture);

	m_window.GetEventManager()->
		AddCallback("Move", &Game::MoveSprite, this);
}

///////////////
//Game updating
void Game::Update()
{
	m_window.Update(); // Update window event
}

void Game::Render()
{
	m_window.BeginDraw(); //clear
	m_window.Draw(m_sprite);
	m_window.EndDraw();
}

//////////
//Callback
void Game::MoveSprite(EventDetails * details)
{
	sf::Vector2i mousepos =
		m_window.GetEventManager()->GetMousePos(
			&m_window.GetRenderWindow());
	m_sprite.setPosition(mousepos.x, mousepos.y);
	std::cout << "Moving sprite to: "
		<< mousepos.x << ":"
		<< mousepos.y << std::endl;
}
