#include "Game.h"

//////
//Ctor
Game::Game() : m_window(sf::Vector2u(800, 600), "Chapter 5"),
	m_stateManager(&m_context), m_entityManager(&m_context, 100)
{
	m_context.m_wind = &m_window;
	m_context.m_eventManager = m_window.GetEventManager();
	m_context.m_entityManager = &m_entityManager;
	m_context.m_textureManager = &m_textureManager;
	m_stateManager.SwitchTo(StateType::Game);
}

///////////////
//Game updating
void Game::Update()
{
	m_window.Update();
	m_stateManager.Update(m_elapsed);
}

void Game::Render()
{
	m_window.BeginDraw();
	m_stateManager.Draw();
	m_window.EndDraw();
}

void Game::LateUpdate()
{
	m_stateManager.ProcessRequest();
	RestartClock();
}
