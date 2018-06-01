#include "Game.h"

void Game::Update()
{
	//Event polling
	m_window.Update();

	//State updating
	m_stateMgr.Update(m_elapsed);
}

void Game::Draw()
{
	m_window.StartDraw(); // window.clear(sf::Color::Black);
	m_stateMgr.Draw(); // state drawing
	m_window.EndDraw(); // window.display();
}

