#ifndef WINDOW_H
#define WINDOW_H
 
#include <SFML/Graphics.hpp>
#include <string>

class Window
{
public:
	Window() { Setup(sf::Vector2u(640, 480), "First Window"); }
	Window(sf::Vector2u size, std::string title) { Setup(size, title); }
	~Window() { Destroy(); }

	void BeginDraw() { m_window.clear(sf::Color::Black); }
	void EndDraw() { m_window.display(); }

	void SetFrame(unsigned int frame) 
		{ m_window.setFramerateLimit(frame); }
	void Update();

	bool IsDone() { return m_isDone; }
	bool IsFullscreen() { return m_isFullscreen; }
	sf::Vector2u GetWindowSize() { return m_windowSize; }
	sf::RenderWindow & GetRenderWindow() { return m_window; }

	void ToggleFullscreen();

	void Draw(sf::Drawable & drawable)
		{ m_window.draw(drawable); }

private:
	void Setup(sf::Vector2u size, std::string title);
	void Create();
	void Destroy() { m_window.close(); }

	sf::RenderWindow m_window;
	sf::Vector2u m_windowSize;
	std::string m_windowTitle;
	bool m_isDone;
	bool m_isFullscreen;
};

#endif