#include "EventManager.h"

EventManager::EventManager() : m_hasFocus(true)
{
	LoadBindings();
}
EventManager::~EventManager()
{
	for (auto & itr : m_bindings)
	{
		delete itr.second;
		itr.second = nullptr;
	}
}

bool EventManager::AddBinding(Binding * binding)
{

}

bool EventManager::RemoveBinding(std::string name)
{

}

void EventManager::SetFocus(const bool & focus)
{

}

void EventManager::HandleEvent(sf::Event & event)
{

}

void EventManager::Update()
{

}
