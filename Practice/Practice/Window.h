#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics/RenderWindow.hpp>
#include "EventManager.h"
#include <iostream>

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
	EventManager * GetEventManager() { return &m_evMgr; }
	sf::Vector2u GetWindowSize() { return m_windowSize; }
	bool IsFullcreen() const { return m_isFullscreen; }
	bool IsDone() const { return m_isDone; }

	//Callbacks
	void ToggleFullcreen(EventDetails * details);
	void Close(EventDetails * details) { m_isDone = true; }

private:
	//Helper methods
	void Setup(const sf::Vector2u & size, const std::string & title);
	void Create();
	void Destroy() { m_window.close(); }

	sf::RenderWindow m_window; //Actual window
	sf::Vector2u m_windowSize; //Size of window
	std::string m_title; //Name of window

	bool m_isDone; 
	bool m_isFullscreen;
	bool m_isFocus;
	EventManager m_evMgr;
};

#endif