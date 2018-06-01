#ifndef STATE_GAME_H
#define STATE_GAME_H

#include <SFML/Graphics.hpp>
#include "StateManager.h"

class State_Game : public BaseState
{
public:
	State_Game(StateManager * stateMgr);

	void Update(const sf::Time & elapsed);
	void Draw();

	void MoveMushroom(const sf::Time & elapsed);

	void TogglePaused(EventDetails * details)
	{
		m_isPaused = !m_isPaused;
	}

private:
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::RectangleShape m_rect;
	sf::Font m_font;
	sf::Text m_text;
	sf::Vector2i m_increment;
	
	bool m_isPaused;
};

#endif
