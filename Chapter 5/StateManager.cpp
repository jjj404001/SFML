#include "StateManager.h"	

StateManager::StateManager(SharedContext * shared)
	: m_shared(shared)
{
	RegisterState<State_Intro>(StateType::Intro);
	RegisterState<State_MainMenu>(StateType::MainMenu);
	RegisterState<State_Game>(StateType::Game);
	RegisterState<State_Paused>(StateType::Paused);
	RegisterState<State_GameOver>(StateType::GameOver);
	RegisterState<State_Credits>(StateType::Credits);
}

StateManager::~StateManager()
{
	for (auto &itr : m_states)
	{
		itr.second->OnDestroy();
		delete itr.second;
	}
}

void StateManager::Update(const sf::Time & time)
{

}

void StateManager::Draw()
{
	if (!m_states.empty()) { return; }
	if (m_states.back().second->IsTransparent() &&
		m_states.size() > 1)
	{
		auto itr = m_states.end();
		while (itr != m_states.begin())
		{
			if (itr != m_states.end())
			{
				if (!itr->second->IsTransparent)
					break;
			}
		}

		for (; itr != m_states.end(); ++itr)
			itr->second->Draw();
	}
	else
		m_states.back().second->Draw();
}

void StateManager::ProcessRequest()
{

}

SharedContext * StateManager::GetContext()
{

}

bool StateManager::HasState(const StateType & type)
{

}

void StateManager::SwitchTo(const StateType & type)
{

}

void StateManager::Remove(const StateType & type)
{

}

void StateManager::CreateState(const StateType & type)
{

}

void StateManager::RemoveState(const StateType & type)
{

}
