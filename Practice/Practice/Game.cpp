#include "Game.h"

void Game::Update()
{
	m_window.Update();
}

void Game::Draw()
{
	m_window.StartDraw();
	m_window.EndDraw();
}
