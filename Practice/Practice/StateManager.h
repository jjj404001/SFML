#ifndef STATEMANAGER_H
#define STATEMANAGER_H
#include <vector>
#include "BaseState.h"

class Window;
class EventManager;

enum class StateType
{
	GLOBAL, INTRO, MENU, GAME, PAUSE
};

struct SharedContext
{
	SharedContext() : m_window(nullptr), m_evMgr(nullptr) {}
	Window * m_window;
	EventManager * m_evMgr;
};

using States = std::vector<std::pair<StateType, BaseState*>>;

//using StateFactory = std::unordered_map<StateType,
//			std::function<void(BaseState*)>>;

class StateManager
{
public:
	StateManager();
	~StateManager();

	void Update();
	void Draw();
	void SwitchTo(const StateType & type);

private:
	//helper
	template<typename T>
	void RegisterState(const StateType & type)
	{
		auto itr = m_states.find(type);
		if (itr != m_states.end())
			m_states.emplace(type, new T);
	}

	SharedContext * m_context;
	States m_states;

};

#endif