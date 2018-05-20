#include "Game.h"

void Game::Update()
{
	m_window.Update();
}

void Game::Draw()
{
	m_window.StartDraw();
	//Draw something...
	//...
	m_window.EndDraw();
}
