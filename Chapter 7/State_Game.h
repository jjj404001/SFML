#ifndef STATE_GAME_H
#define STATE_GAME_H

#include "BaseState.h"

class State_Game : public BaseState
{
public:
	State_Game(StateManager * stateMgr)
		: BaseState(stateMgr) {}

	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void Activate();
	virtual void Deactivate();

	virtual void Update(const sf::Time & time);
	virtual void Draw();

	void MainMenu(EventDetails * details);
	void Pause(EventDetails * details);

private:
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::Vector2f m_increment;

};

#endif