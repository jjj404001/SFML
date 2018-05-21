#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Window.h"

class Game
{
public:
	//Ctor
	Game();
	Game(sf::Vector2u size, std::string & title);

	//Game updating
	void Update();
	void Draw();

	//Data getting method
	Window * GetWindow() { return &m_window; }

	void SetFrameRate(int frame)
	{
		m_window.GetRenderWindow()->
			setFramerateLimit(frame);
	}

	void MoveMushroom();

	//Timing
	void RestartClock() { m_elapsed = m_clock.restart(); }
	sf::Time GetElapsed() const { return m_elapsed; }

private:
	//Init texture members
	void TextureLoading();

	Window m_window;

	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::Vector2i m_increment;

	sf::Clock m_clock;
	sf::Time m_elapsed;
};

#endif