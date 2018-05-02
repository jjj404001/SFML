#ifndef SNAKE_H
#define SNAKE_H
 
#include <SFML/Graphics.hpp>
#include <vector>
#include "Textbox.h"

struct SnakeSegment {
	SnakeSegment(int x, int y) : position(x, y) {}
	sf::Vector2i position;
};

using SnakeContainer = std::vector<SnakeSegment>;

enum class Direction{None, Up, Down, Left, Right};

class Snake {
public:
	Snake(int blockSize);
	~Snake();

	//Helper methods
	void SetDirection(Direction dir) { m_dir = dir; }
	//void SetOldDirection(Direction oldDir) { m_oldDir = oldDir; }
	Direction GetPhysicalDirection();
	//Direction GetOldDirection() { return m_oldDir; }
	int GetSpeed() {return m_speed; }

	sf::Vector2i GetPosition()
	{
		return (!m_snakeBody.empty() ?
			m_snakeBody.front().position : sf::Vector2i(1, 1));
	}

	int GetLives() { return m_lives; }
	int GetScore() { return m_score; }
	void IncreaseScore(Textbox & textbox);
	bool HasLost() { return m_lost; }

	//Handle losing
	void Lose() { m_lost = true; }
	void ToggleLost() { m_lost = !m_lost; }

	void Extend(); //Grow the snake
	void Reset(); //Reset to starting position

	void Move(); //Movement method
	void Tick(); //Update method
	void Cut(int segments); //method for cutting snake
	void Render(sf::RenderWindow & window);

private:
	void CheckCollision(); //Checking for cutting snake

	SnakeContainer m_snakeBody;
	int m_size;
	Direction m_dir;
	//Direction m_oldDir;
	int m_speed;
	int m_lives;
	int m_score;
	bool m_lost;
	sf::RectangleShape m_bodyRect;
};

#endif
