#include "Window.h"

////////////////
//Update Methods
void Window::Update()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::LostFocus)
		{
			m_isFocused = false;
			m_eventManager.SetFocus(false);
		}
		else if (event.type == sf::Event::GainedFocus)
		{
			m_isFocused = true;
			m_eventManager.SetFocus(true);
		}
		m_eventManager.HandleEvent(event);
	}
	m_eventManager.Update();
}

//////////
//Callback
void Window::ToggleFullscreen(EventDetails * details)
{
	m_isFullscreen = !m_isFullscreen;
	Destroy();
	Create();
}

////////////////
//Helper Methods
void Window::Setup(sf::Vector2u size, std::string title)
{
	m_windowSize = size;
	m_windowTitle = title;
	m_isDone = false;
	m_isFullscreen = false;
	m_isFocused = true;

	m_eventManager.AddCallback(
		"Fullscreen_toggle", &Window::ToggleFullscreen, this);
	m_eventManager.AddCallback(
		"Window_close", &Window::Close, this);

	Create();
}

void Window::Create()
{
	auto style = (IsFullscreen() ?
		sf::Style::Fullscreen : sf::Style::Default);
	m_window.create({m_windowSize.x, m_windowSize.y, 32},
		m_windowTitle, style);
}
