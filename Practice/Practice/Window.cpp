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
		evMgr.HandleEvent(event);

	evMgr.Update();
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
void Window::Setup(const sf::Vector2u & size, const std::string & title)
{
	m_windowSize = size;
	m_title = title;
	m_isDone = false;
	m_isFullscreen = false;

	evMgr.AddCallback(&Window::Close, this, "Window_Close");
	evMgr.AddCallback(&Window::ToggleFullcreen, this,
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
