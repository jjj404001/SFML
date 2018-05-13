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
	if (!m_states.empty()) { return; }
	if (m_states.back().second->IsTranscendent() &&
		m_states.size() > 1)
	{
		auto itr = m_states.end();
		while (itr != m_states.begin())
		{
			if (itr != m_states.end())
			{
				if (itr->second->IsTranscendent())
					break;
			}
			--itr;
		}

		for (; itr != m_states.end(); ++itr)
			itr->second->Update(time);
	}
	else
		m_states.back().second->Update(time);
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
				if (!itr->second->IsTransparent())
					break;
			}
			--itr;
		}

		for (; itr != m_states.end(); ++itr)
			itr->second->Draw();
	}
	else
		m_states.back().second->Draw();
}

void StateManager::ProcessRequest()
{
	while (m_toRemove.begin() != m_toRemove.end())
	{
		RemoveState(*m_toRemove.begin());
		m_toRemove.erase(m_toRemove.begin());
	}
}

SharedContext * StateManager::GetContext()
{
	return m_shared;
}

bool StateManager::HasState(const StateType & type)
{
	for (auto itr = m_states.begin(); 
		itr != m_states.end(); ++itr)
	{
		if (itr->first == type)
		{
			auto removed = std::find(
				m_toRemove.begin(), m_toRemove.end(), type);
			if (removed == m_toRemove.end())
				return true;
			return false;
		}
	}
	return false;
}

void StateManager::SwitchTo(const StateType & type)
{
	m_shared->m_eventManager->SetCurrentState(type);
	for (auto itr = m_states.begin();
		itr != m_states.end(); ++itr)
	{
		if (itr->first == type)
		{
			m_states.back().second->Deactivate();
			BaseState * tempState = itr->second;
			StateType tempType = itr->first;
			m_states.erase(itr);
			m_states.emplace_back(tempType, tempState);
			tempState->Activate();
			return;
		}
	}

	if (!m_states.empty())
		m_states.back().second->Deactivate();
	CreateState(type);
	m_states.back().second->Activate();
}

void StateManager::Remove(const StateType & type)
{
	m_toRemove.push_back(type);
}

void StateManager::CreateState(const StateType & type)
{
	auto newState = m_stateFactory.find(type);
	if (newState == m_stateFactory.end())
		return;
	BaseState * tempState = newState->second();
	m_states.emplace_back(type, tempState);
	tempState->OnCreate();
}

void StateManager::RemoveState(const StateType & type)
{
	for (auto itr = m_states.begin(); itr != m_states.end(); ++itr)
	{
		if (itr->first == type)
		{
			itr->second->OnDestroy();
			delete itr->second;
			m_states.erase(itr);
			return;
		}
	}

}
