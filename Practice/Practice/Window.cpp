#include <SFML/Window/Event.hpp>
#include "Window.h"

//Ctors
Window::Window()
{
	Setup(sf::Vector2u(640, 480), std::string("Window"));
}

Window::Window(const sf::Vector2u & size, 
	const std::string & title)
{
	Setup(size, title);
}

//Window updating
void Window::Update()
{
	sf::Event event;

	//Event polling
	while (m_window.pollEvent(event))
	{
		//Focus setting
		if (event.type == sf::Event::LostFocus)
		{
			m_isFocus = false;
			m_evMgr.SetFocus(false);
		}
		else if (event.type == sf::Event::GainedFocus)
		{
			m_isFocus = true;
			m_evMgr.SetFocus(true);
		}
		//Window event handler
		m_evMgr.HandleEvent(event);
	}
	//Real-time input handler
	m_evMgr.Update();
}

//Callback method
void Window::ToggleFullcreen(EventDetails * details)
{
	m_isFullscreen = !m_isFullscreen;
	Destroy();
	Create();
}

////////Helper method
//Initializing window
void Window::Setup(const sf::Vector2u & size,
	const std::string & title)
{
	m_windowSize = size;
	m_title = title;
	m_isDone = false;
	m_isFullscreen = false;
	m_isFocus = true;

	m_evMgr.AddCallback(StateType(0), &Window::Close, this, "Window_Close");
	m_evMgr.AddCallback(StateType(0), &Window::ToggleFullcreen, this,
		"Toggle_Fullscreen");

	Create();
}

//Creating window
void Window::Create()
{
	//Window style
	auto style = (IsFullcreen() ? 
		sf::Style::Fullscreen : sf::Style::Default);

	//Actual window creating
	m_window.create({m_windowSize.x, m_windowSize.y, 32},
		m_title, style);
}
