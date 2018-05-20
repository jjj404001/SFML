#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics/RenderWindow.hpp>
#include "EventManager.h"

class Window
{
public:
	//Ctor & Dtor
	Window();
	Window(const sf::Vector2u & size, const std::string & title);
	~Window() { Destroy(); }

	//Event polling
	void Update();

	//Drawing
	void StartDraw() { m_window.clear(sf::Color::Black); }
	void EndDraw() { m_window.display(); }

	//Data getting methods
	sf::RenderWindow * GetRenderWindow() { return &m_window; }
	sf::Vector2u GetWindowSize() { return m_windowSize; }

	//Fullscreen
	bool IsFullcreen() { return m_isFullscreen; }
	void ToggleFullcreen(EventDetails * details);


	//Closing
	bool IsDone() { return m_isDone; }
	void Close(EventDetails * details) { m_isDone = true; }

private:
	//Helper methods
	void Create(const sf::Vector2u & size, const std::string & title);
	void Destroy() { m_window.close(); }

	sf::RenderWindow m_window;
	sf::Vector2u m_windowSize; //Size of window
	std::string m_title; //Name of window
	EventManager evMgr;

	bool m_isDone;
	bool m_isFullscreen;
};

#endif