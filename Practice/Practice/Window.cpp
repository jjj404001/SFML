#include <SFML/Window/Event.hpp>
#include "Window.h"

//Default Ctor
Window::Window()
	: m_isFullscreen(false), m_isDone(false)
{
	Create(sf::Vector2u(640, 480), "My Window");
}

//Ctor
Window::Window(const sf::Vector2u & size, 
	const std::string & title)
	: m_isFullscreen(false), m_isDone(false)
{
	Create(size, title);
}

//Window updating
void Window::Update()
{
	//Event variable
	sf::Event event;

	//Event polling
	while (m_window.pollEvent(event))
	{
		//When close button is clicked
		if (event.type == sf::Event::Closed)
			m_isDone = true;
		//When key F5 is pressed
		if (event.type == sf::Event::KeyPressed && 
			event.key.code == sf::Keyboard::F5)
		{
			//Recreate the window
			ToggleFullcreen();
			Destroy();
			Create(m_windowSize, m_title);
		}
	}
}

//Helper method in ctor
void Window::Create(const sf::Vector2u & size, 
	const std::string & title)
{
	//Data initialize
	m_windowSize = size;
	m_title = title;

	//Window style
	auto style = (IsFullcreen() ? 
		sf::Style::Fullscreen : sf::Style::Default);

	//Actual creating window
	m_window.create({m_windowSize.x, m_windowSize.y, 32},
		title, style);
}