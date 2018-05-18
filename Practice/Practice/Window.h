#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics/RenderWindow.hpp>

class Window
{
public:
	Window() { Create(); }
	Window(sf::Vector2u size, std::string & title) 
	{ Create(size, title); }

	void Update(sf::Event & event);

	void StartDraw() { m_window.clear(sf::Color::Black); }
	void EndDraw() { m_window.display(); }

	sf::RenderWindow * GetRenderWindow() { return &m_window; }
	sf::Vector2u GetWindowSize() { return m_windowSize; }

private:
	void Create(sf::Vector2u size = 
		sf::Vector2u(640, 480), 
		std::string title = "Window");

	void Destroy() { m_window.close(); }

	sf::RenderWindow m_window;
	sf::Vector2u m_windowSize;
};

#endif