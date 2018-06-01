#ifndef GAME_H
#define GAME_H

#include "StateManager.h"

class Game
{
public:
	//Ctor
	Game() : m_window(), m_stateMgr(&m_window) {}

	//Main game looping methods
	void Update();
	void Draw();
	void RestartClock() { m_elapsed = m_clock.restart(); }

	void SetFrameRate(int frame)
	{
		m_window.GetRenderWindow()->
			setFramerateLimit(frame);
	}

	//Data getting methods
	Window * GetWindow() { return &m_window; }
	const sf::Time & GetElapsed() const { return m_elapsed; }

private:
	Window m_window;
	StateManager m_stateMgr;

	sf::Clock m_clock;
	sf::Time m_elapsed;
};

#endif