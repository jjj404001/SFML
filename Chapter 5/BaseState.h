#ifndef BASESTATE_H
#define BASESTATE_H

#include <SFML/Graphics.hpp>

struct EventDetails;
class StateManager;

class BaseState 
{
	friend class StateManager;

public:
	BaseState(StateManager * stateManager)
		: m_stateMgr(stateManager),
		  m_transparent(false),
		  m_transcendent(false) {}
	virtual ~BaseState() {}

	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;

	virtual void Update(const sf::Time & time) = 0;
	virtual void Draw() = 0;

	void SetTransparent(const bool & transparent)
	{
		m_transparent = transparent;
	}
	bool IsTransparent() const { return m_transparent; }

	void SetTrancendent(const bool & transcendent)
	{
		m_transcendent = transcendent;
	}
	bool IsTranscendent() const { return m_transcendent; }

	StateManager * GetStateManager() { return m_stateMgr; }
	sf::View & GetView() { return m_view; }

protected:
	StateManager * m_stateMgr;
	bool m_transparent;
	bool m_transcendent;
	sf::View m_view;
};

#endif