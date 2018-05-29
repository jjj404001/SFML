#include "StateManager.h"
#include "State_Intro.h"

StateManager::StateManager()
{
	RegisterState<State_Intro>(StateType::INTRO);
}

StateManager::~StateManager()
{
	for (auto itr = m_states.begin(); itr != m_states.end(); ++itr)
		delete itr->second;
}

void StateManager::Update()
{
	
}

void StateManager::Draw()
{

}

void StateManager::SwitchTo(const StateType & type)
{

}
