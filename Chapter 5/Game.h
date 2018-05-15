#ifndef GAME_H
#define GAME_H

#include "Window.h"
#include "StateManager.h"

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
	//Data
	Window m_window;
	StateManager m_stateManager;
	SharedContext m_context;

	sf::Clock m_clock;
	sf::Time m_elapsed;
};

#endif