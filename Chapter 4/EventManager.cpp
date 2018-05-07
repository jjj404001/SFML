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
	if (m_bindings.find(binding->m_name) != m_bindings.end())
		return false;

	return m_bindings.emplace
		(binding->m_name, binding).second;
}

bool EventManager::RemoveBinding(std::string name)
{
	auto itr = m_bindings.find(name);
	if (itr == m_bindings.end())
		return false;

	delete itr->second;
	m_bindings.erase(itr);
	return true;
}

void EventManager::SetFocus(const bool & focus)
{

}

void EventManager::HandleEvent(sf::Event & event)
{
	for (auto & b_itr : m_bindings)
	{
		Binding * bind = b_itr.second;
		for (auto & e_itr : bind->m_events)
		{
			EventType sfmlEvent = (EventType)event.type;
			if (e_itr.first != sfmlEvent)
				continue;
			if (sfmlEvent == EventType::KeyDown ||
				sfmlEvent == EventType::KeyUp)
			{
				if (e_itr.second.m_code == event.key.code)
				{
					//Matching event/keystroke
					//Increase count
					if (bind->m_details.m_keyCode != -1)
					{
						bind->m_details.m_keyCode = e_itr.second.m_code;
					}
					++(bind->c);
					break;
				}
			}
			else if (sfmlEvent == EventType::MButtonDown ||
					 sfmlEvent == EventType::MButtonUp)
			{
				if (e_itr.second.m_code == event.mouseButton.button)
				{
					//Matching event/keystroke
					//Increase count
					bind->m_details.m_mouse.x = event.mouseButton.x;
					bind->m_details.m_mouse.y = event.mouseButton.y;
					if (bind->m_details.m_keyCode != -1)
						bind->m_details.m_keyCode = e_itr.second.m_code;
					++(bind->c);
					break;	
				}
			}
			else
			{
				//No need for additional checking
				if (sfmlEvent == EventType::MouseWheel)
				{
					bind->m_details.m_mouseWheelDelta =
						event.mouseWheel.delta;
				}
				else if (sfmlEvent == EventType::WindowResized)
				{
					bind->m_details.m_size.x = event.size.width;
					bind->m_details.m_size.y = event.size.height;
				}
				else if (sfmlEvent == EventType::TextEntered)
				{
					bind->m_details.m_textEntered = event.text.unicode;
				}
				++(bind->c);
			}
		}
	}
}

void EventManager::Update()
{
	if (!m_hasFocus)
		return;
	for (auto & b_itr : m_bindings)
	{
		Binding * bind = b_itr.second;
		for (auto & e_itr : bind->m_events)
		{
			switch (e_itr.first)
			{
			case(EventType::Keyboard):
				if (sf::Keyboard::isKeyPressed
					(sf::Keyboard::Key(e_itr.second.m_code)))
				{
					if (bind->m_details.m_keyCode != -1)
						bind->m_details.m_keyCode = e_itr.second.m_code;
					++(bind->c);
				}
				break;
			case(EventType::Mouse):
				if (sf::Mouse::isButtonPressed
					(sf::Mouse::Button(e_itr.second.m_code)))
				{
					if (bind->m_details.m_keyCode != -1)
						bind->m_details.m_keyCode = e_itr.second.m_code;
					++(bind->c);
				}
				break;
			case(EventType::Joystick):
				//Up for expansion
				break;
			}
		}

		if (bind->m_events.size() == bind->c)
		{
			auto callItr = m_callbacks.find(bind->m_name);
			if (callItr != m_callbacks.end())
				callItr->second(&bind->m_details);
		}
		bind->c = 0;
		bind->m_details.Clear();
	}
}
