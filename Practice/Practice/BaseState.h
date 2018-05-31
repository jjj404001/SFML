#ifndef BASESTATE_H
#define BASESTATE_H

#include <SFML/System/Time.hpp>

class StateManager;

class BaseState
{
public:
	BaseState(StateManager * stateMgr)
		: m_stateMgr(stateMgr), isTransparent(false),
		  isTranscendent(false) {}
	virtual ~BaseState() {}

	virtual void Update(const sf::Time & elapsed) = 0;
	virtual void Draw() = 0;

	virtual void Create() = 0;
	virtual void Destroy() = 0;

protected:
	StateManager * m_stateMgr;
	bool isTransparent;
	bool isTranscendent;
};

#endif
