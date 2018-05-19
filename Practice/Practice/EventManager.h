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
		: m_mousePos(sf::Vector2i(0, 0)), m_name("NULL"), c(0) {}

	void Clear()
	{
		m_mousePos = sf::Vector2i(0, 0);
		m_name = "NULL";
		c = 0;
	}

	sf::Vector2i m_mousePos;
	std::string m_name; // name of event
	int c; //Number of events that are "on"
};

struct Binding
{
	EventDetails m_details;
	Events m_events;
	std::string m_name;
};

using Bindings = std::unordered_map<std::string, Binding*>;

using Callbacks = std::unordered_map<
	std::string, std::function<void(EventDetails*)>>;

class EventManager
{
public:
	EventManager();
	~EventManager();

	template<typename T>
	bool AddCallback(void T::*func(EventDetails*), T* instance,
		const std::string & name)
	{
		auto temp = std::bind(func, *instance, std::placeholders::_1);
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

	void Update();
	void HandleEvent(sf::Event event);

private:
	void LoadBinginds();

	Bindings m_bindings;
	Callbacks m_callbacks;
};

#endif