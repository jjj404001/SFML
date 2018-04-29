#ifndef WORLD_H
#define WORLD_H
 
#include <SFML/Graphics.hpp>

class Snake;

class World
{
public:
	World(sf::Vector2u windSize);
	~World();

	int GetBlockSize() const { return m_blockSize; }

	void RespawnApple();

	void Update(Snake & player);
	void Render(sf::RenderWindow & window);

private:
	sf::Vector2u m_windowSize;
	sf::Vector2i m_item; //apple position
	int m_blockSize;

	sf::CircleShape m_appleShape; //apple
	sf::RectangleShape m_bounds[4]; //boundary
};

#endif