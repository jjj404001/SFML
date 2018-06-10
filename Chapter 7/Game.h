#ifndef GAME_H
#define GAME_H

#include "Window.h"
#include "StateManager.h"
#include "EntityManager.h"
#include "TextureManager.h"

class Game
{
public:
	//Ctor & Dtor
	Game();
	~Game() {}

	//Game Updating
	void Update();
	void Render();
	void LateUpdate();

	//Data getting
	Window * GetWindow() { return &m_window; }

	//Time
	sf::Time GetElapsed() { return m_elapsed; }
	void RestartClock() { m_elapsed = m_clock.restart(); }

private:
	Window m_window;

	//ORDER IS IMPORTANT/////////////
	TextureManager m_textureManager;
	EntityManager m_entityManager;
	StateManager m_stateManager;
	/////////////////////////////////

	sf::Clock m_clock;
	sf::Time m_elapsed;

	SharedContext m_context;
};

#endif