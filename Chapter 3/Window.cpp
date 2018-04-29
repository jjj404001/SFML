#include "Window.h"
 
void Window::Setup(sf::Vector2u size, std::string title)
{
	m_windowSize = size;
	m_windowTitle = title;
	m_isDone = false;
	m_isFullscreen = false;
	Create();
}

void Window::Update()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_isDone = true;
		else if (event.type == sf::Event::KeyPressed &&
			event.key.code == sf::Keyboard::F5)
			ToggleFullscreen();
	}
}

void Window::Create()
{
	auto style = (IsFullscreen() ?
		sf::Style::Fullscreen : sf::Style::Default);
	m_window.create({m_windowSize.x, m_windowSize.y, 32},
		m_windowTitle, style);
}

void Window::ToggleFullscreen()
{
	m_isFullscreen = !m_isFullscreen;
	Destroy();
	Create();
}

