#include "Game.h"

//////
//Ctor
Game::Game() : m_window(sf::Vector2u(800, 600), "Chapter 8"),
	m_entityManager(&m_systemManager, &m_textureManager),
	m_stateManager(&m_context)
{
	//Initializing
	m_systemManager.SetEntityManager(&m_entityManager);

	//Context setting
	m_context.m_wind = &m_window;
	m_context.m_eventManager = m_window.GetEventManager();
	m_context.m_textureManager = &m_textureManager;
	m_context.m_systemManager = &m_systemManager;
	m_context.m_entityManager = &m_entityManager;

	//Starting state
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
