#ifndef STATE_GAME_H
#define STATE_GAME_H

#include "BaseState.h"

class State_Game : public BaseState
{
public:
	State_Game(StateManager * stateMgr);

	void Update(const sf::Time & elapsed);
	void Draw();
	void Create();
	void Destroy();

private:

};

#endif
