#include "StateManager.h"
#include "State_Intro.h"
#include "State_Game.h"

StateManager::StateManager(Window * window)
	: m_context(window), m_currentState(StateType::INTRO)
{
	m_context.m_evMgr->SetState(m_currentState);

	RegisterState<State_Intro>(StateType::INTRO);
	RegisterState<State_Game>(StateType::GAME);
	//Other states ...
}

StateManager::~StateManager()
{
	for (auto & itr : m_states)
		delete itr.second;
}

void StateManager::Update(const sf::Time & elapsed)
{
	auto itr = m_states.find(m_currentState);
	if (itr != m_states.end())
		itr->second->Update(elapsed);
}

void StateManager::Draw()
{
	auto itr = m_states.find(m_currentState);
	if (itr != m_states.end())
		itr->second->Draw();
}

void StateManager::SwitchTo(const StateType & type)
{
	auto itr = m_states.find(m_currentState);
	if (itr != m_states.end())
	{
		m_currentState = type;
		m_context.m_evMgr->SetState(type);
	}
}
