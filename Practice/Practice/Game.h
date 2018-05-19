#ifndef GAME_H
#define GAME_H

#include "Window.h"
#include "EventManager.h"

class Game
{
public:
	//Ctor
	Game() : m_window() {}
	Game(sf::Vector2u size, std::string & title)
		: m_window(size, title) {}

	//Game updating
	void Update();
	void Draw();

	//Data getting method
	Window * GetWindow() { return &m_window; }

private:
	Window m_window;
	EventManager evMgr;
};

#endif