#ifndef STATE_INTRO_H
#define STATE_INTRO_H

#include <SFML/Graphics.hpp>
#include "BaseState.h"

class State_Intro : public BaseState
{
public:
	State_Intro(StateManager * stateMgr);

	void Update(const sf::Time & elapsed);
	void Draw();
	void Create();
	void Destroy();

	void SwitchTo(EventDetails * details);

private:
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::Vector2f m_increment;
};

#endif