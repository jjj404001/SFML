#ifndef BASESTATE_H
#define BASESTATE_H

#include <SFML/System/Time.hpp>

class StateManager;

//Abstract Base Class
class BaseState
{
public:
	BaseState(StateManager * stateMgr)
		: m_stateMgr(stateMgr) {}
	virtual ~BaseState() {}

	virtual void Update(const sf::Time & elapsed) = 0;
	virtual void Draw() = 0;

protected:
	StateManager * m_stateMgr;
};

#endif
