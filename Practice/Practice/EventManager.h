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
	EventCode(int code = 0)
		: m_code(code) {}
	int m_code;
};

using Events = std::vector<std::pair<EventType, EventCode>>;

struct EventDetails
{
	EventDetails(const std::string & name)
		: m_name(name) {}

	std::string m_name;
	//Other details...
	//...
};

struct Binding
{
	Binding(const std::string & name)
		: m_name(name), m_details(name), c(0) {}

	std::string m_name;
	EventDetails m_details;
	int c; //Number of events that are on
	Events m_events;
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

	void RemoveCallback(const std::string & name)
	{
			m_callbacks.erase(name);
	}

	//Event handling
	void HandleEvent(sf::Event event);
	//Realtime input checking
	void Update();
	
	void SetFocus(bool focus) { m_isFocus = focus; }

private:
	//Init the member 'm_bindings'
	void LoadBinginds();

	bool m_isFocus;
	Bindings m_bindings;
	Callbacks m_callbacks;
};

#endif