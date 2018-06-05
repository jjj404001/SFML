#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <functional>
#include "EventManager.h"
#include "BaseState.h"
#include "Window.h"


enum class StateType{
	Intro = 1, MainMenu, Game, Paused, GameOver, Credits};

class TextureManager;
class Map;

struct SharedContext {
	SharedContext() : 
		m_wind(nullptr), m_eventManager(nullptr) {}

	Window * m_wind;
	EventManager * m_eventManager;
	TextureManager * m_textureManager;
	Map * m_gameMap;
};

using StateContainer 
	= std::vector <std::pair<StateType, BaseState*>>;

using TypeContainer = std::vector<StateType>;

using StateFactory = std::unordered_map<
	StateType, std::function<BaseState*(void)>>;

class StateManager 
{
public:
	StateManager(SharedContext * shared);
	~StateManager();

	void Update(const sf::Time & time);
	void Draw();

	void ProcessRequest();

	SharedContext * GetContext();
	bool HasState(const StateType & type);

	void SwitchTo(const StateType & type);
	void Remove(const StateType & type);

private:
	void CreateState(const StateType & type);
	void RemoveState(const StateType & type);

	template<class T>
	void RegisterState(const StateType & type) 
	{
		m_stateFactory[type] = [this]() -> BaseState*
		{
			return new T(this);
		};
	}

	SharedContext * m_shared;
	StateContainer m_states;
	TypeContainer m_toRemove;
	StateFactory m_stateFactory;
};

#endif