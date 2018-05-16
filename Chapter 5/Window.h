#ifndef WINDOW_H
#define WINDOW_H
 
#include "EventManager.h"

class Window
{
public:
	//Ctor & Dtor
	Window() { Setup(sf::Vector2u(640, 480), "First Window"); }
	Window(sf::Vector2u size, std::string title) { Setup(size, title); }
	~Window() { Destroy(); }

	//Update
	void Update();

	//Drawing
	void BeginDraw() { m_window.clear(sf::Color::Black); }
	void Draw(sf::Drawable & drawable) { m_window.draw(drawable); }
	void EndDraw() { m_window.display(); }

	//Callbacks
	void ToggleFullscreen(EventDetails * details = nullptr);
	void Close(EventDetails * details = nullptr) { m_isDone = true; }

	//Data getting
	sf::RenderWindow * GetRenderWindow() { return &m_window; }
	EventManager * GetEventManager() { return &m_eventManager; }
	sf::Vector2u GetWindowSize() { return m_windowSize; }

	//Data checking
	bool IsDone() const { return m_isDone; }
	bool IsFullscreen() const { return m_isFullscreen; }
	bool IsFocused() const { return m_isFocused; }

private:
	//Helper methods
	void Setup(sf::Vector2u size, std::string title);
	void Create();
	void Destroy() { m_window.close(); }

	//Data
	sf::RenderWindow m_window;
	sf::Vector2u m_windowSize;
	std::string m_windowTitle;
	bool m_isDone;
	bool m_isFullscreen;

	EventManager m_eventManager;
	bool m_isFocused;
};

#endif