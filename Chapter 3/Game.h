#ifndef GAME_H
#define GAME_H
 
#include "Window.h"
#include "Snake.h"
#include "World.h"
#include "Textbox.h"

class Game
{
public:
	Game();
	~Game();
	
	void InputHandler();
	void Update();
	void Render();
	void RestartClock() { m_elapsed += m_clock.restart(); }
	sf::Time ElapsedTime() { return m_elapsed; }

	Window* GetWindow() { return &m_window; }

private:
	Window m_window;
	World m_world;
	Snake m_snake;
	Textbox m_textbox;

	sf::Clock m_clock;
	sf::Time m_elapsed;
};

#endif