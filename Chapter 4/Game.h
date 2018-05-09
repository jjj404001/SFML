#ifndef GAME_H
#define GAME_H
#include "Window.h"

class Game
{
public:
	//Ctor & Dtor
	Game();
	~Game() {}

	//Game Updating
	void Update();
	void Render();

	//Data getting
	Window * GetWindow() { return &m_window; }

private:
	//Callback
	void MoveSprite(EventDetails * details);

	//Data
	Window m_window;
	sf::Texture m_texture;
	sf::Sprite m_sprite;
};

#endif