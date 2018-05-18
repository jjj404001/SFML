#ifndef STATE_PAUSED_H
#define STATE_PAUSED_H

#include "BaseState.h"

class State_Paused :public BaseState
{
public:
	State_Paused(StateManager * stateMgr)
		: BaseState(stateMgr) {}

	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void Activate() {}
	virtual void Deactivate() {}

	virtual void Update(const sf::Time & time) {}
	virtual void Draw();

	void Unpaused(EventDetails * details);

private:
	sf::Text m_text;
	sf::Font m_font;
	sf::RectangleShape m_rect;

};

#endif