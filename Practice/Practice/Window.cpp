#include <SFML/Window/Event.hpp>
#include "Window.h"

//Default Ctor
Window::Window()
{
	Setup(sf::Vector2u(640, 480), std::string("Window"));
}

//Ctor
Window::Window(const sf::Vector2u & size, 
	const std::string & title)
{
	Setup(size, title);
}

//Window updating
void Window::Update()
{
	//Event variable
	sf::Event event;

	//Event polling
	while (m_window.pollEvent(event))
	{
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
		m_evMgr.HandleEvent(event);
	}
	m_evMgr.Update();
}

//Callback method
void Window::ToggleFullcreen(EventDetails * details)
{
	m_isFullscreen = !m_isFullscreen;
	Destroy();
	Create();
}

//////////Helper method
//Initialize the Window
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

//Create the window
void Window::Create()
{
	//Window style
	auto style = (IsFullcreen() ? 
		sf::Style::Fullscreen : sf::Style::Default);

	//Actual creating window
	m_window.create({m_windowSize.x, m_windowSize.y, 32},
		m_title, style);
}
