#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <SFML/Window.hpp>
#include <unordered_map>
#include <functional>

enum class EventType
{
	KeyDown = sf::Event::KeyPressed,
	KeyUp = sf::Event::KeyReleased,
	MButtonDown = sf::Event::MouseButtonPressed,
	MButtonUp = sf::Event::MouseButtonReleased,
	MouseWheel = sf::Event::MouseWheelMoved,
	WindowResized = sf::Event::Resized,
	GainedFocus = sf::Event::GainedFocus,
	LostFocus = sf::Event::LostFocus,
	MouseEntered = sf::Event::MouseEntered,
	MouseLeft = sf::Event::MouseLeft,
	Closed = sf::Event::Closed,
	TextEntered = sf::Event::TextEntered,
	Keyboard = sf::Event::Count + 1,
	Mouse
};

struct EventCode
{
	EventCode(int code) : m_code(0) 
	{
		m_code = code;
	}
	int m_code;
};

using Events = std::vector<std::pair<EventType, EventCode>>;

struct EventDetails
{
	EventDetails()
		: m_name("NULL") {}

	std::string m_name;
	//Other details...
	//...
};

struct Binding
{
	Binding() : m_name("NULL") {}

	std::string m_name;
	EventDetails m_details;
	Events m_events;
	int c; //Number of events that are on
};

using Bindings = std::unordered_map<std::string, Binding*>;

using Callbacks = std::unordered_map<
	std::string, std::function<void(EventDetails*)>>;

class EventManager
{
public:
	//Ctor & Dtor
	EventManager();
	~EventManager();

	template<typename T>
	bool AddCallback(void (T::*func)(EventDetails*), T* instance,
		const std::string & name)
	{
		auto temp = std::bind(func, instance, std::placeholders::_1);
		return m_callbacks.emplace(name, temp).second;
	}

	bool RemoveCallback(const std::string & name)
	{
		auto itr = m_callbacks.find(name);
		if (itr != m_callbacks.end())
		{
			m_callbacks.erase(name);
			return true;
		}
		return false;
	}

	//Event handling
	void HandleEvent(sf::Event event);
	//Realtime input checking
	void Update();

private:
	//Init the member 'm_bindings'
	void LoadBinginds();

	Bindings m_bindings;
	Callbacks m_callbacks;
};

#endif