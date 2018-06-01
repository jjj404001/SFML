#ifndef STATEMANAGER_H
#define STATEMANAGER_H
#include <unordered_map>
#include "BaseState.h"
#include "Window.h"

enum class StateType
{
	GLOBAL, INTRO, MENU, GAME, PAUSE
};

struct SharedContext
{
	SharedContext(Window * window)
	{
		m_window = window;
		m_evMgr = window->GetEventManager();
	}
	Window * m_window;
	EventManager * m_evMgr;
};

using States = std::unordered_map<StateType, BaseState*>;

class StateManager
{
public:
	StateManager(Window * window);
	~StateManager();

	void Update(const sf::Time & elapsed);
	void Draw();
	void SwitchTo(const StateType & type);

	SharedContext * GetContext() { return &m_context; }

private:
	template<typename T>
	void RegisterState(const StateType & type)
	{
		auto itr = m_states.find(type);
		if (itr == m_states.end())
			m_states.emplace(type, new T(this));
	}

	SharedContext m_context;
	States m_states;
	StateType m_currentState;
};

#endif