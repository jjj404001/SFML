#include "World.h"
#include "Snake.h"
 
bool IsOverlap(const SnakeContainer & snakeBody, int itemX, int itemY);

World::World(sf::Vector2u windSize) 
	: m_windowSize(windSize), m_blockSize(32)
{
	srand(time(0));

	m_appleShape.setFillColor(sf::Color::Red);
	m_appleShape.setRadius(m_blockSize / 2);

	for (int i = 0; i < 4; ++i)
	{
		m_bounds[i].setFillColor(sf::Color(150, 0, 0));
		if (!((i + 1) % 2))
		{
			m_bounds[i].setSize(sf::Vector2f(m_windowSize.x,
				m_blockSize));
		}
		else
		{
			m_bounds[i].setSize(sf::Vector2f(m_blockSize,
				m_windowSize.y));
		}

		if (i < 2)
			m_bounds[i].setPosition(0, 0);
		else 
		{
			m_bounds[i].setOrigin(m_bounds[i].getSize());
			m_bounds[i].setPosition(sf::Vector2f(m_windowSize));
		}
	}
}

World::~World() {}

void World::RespawnApple(const Snake & snake)
{
	int maxX = (m_windowSize.x / m_blockSize) - 2;
	int maxY = (m_windowSize.y / m_blockSize) - 2;

	int itemX = rand() % maxX + 1;
	int itemY = rand() % maxY + 1;

	while (IsOverlap(snake.GetContainer(), itemX, itemY))
	{
		itemX = rand() % maxX + 1;
		itemY = rand() % maxY + 1;
	}

	m_item.x = itemX;
	m_item.y = itemY;

	m_appleShape.setPosition(
		m_item.x * m_blockSize,
		m_item.y * m_blockSize);
}

void World::Update(Snake & player, Textbox & textbox)
{
	if (player.GetPhysicalDirection() == Direction::None)
		RespawnApple(player);

	if (player.GetPosition() == m_item)
	{
		player.Extend();
		player.IncreaseScore(textbox);
		RespawnApple(player);
	}

	int gridSize_x = m_windowSize.x / m_blockSize;
	int gridSize_y = m_windowSize.y / m_blockSize;

	if (player.GetPosition().x <= 0 ||
		player.GetPosition().y <= 0 ||
		player.GetPosition().x >= gridSize_x - 1 ||
		player.GetPosition().y >= gridSize_y - 1)
	{
		player.Lose();
	}
}

void World::Render(sf::RenderWindow & window)
{
	for (int i = 0; i < 4; ++i)
		window.draw(m_bounds[i]);
	window.draw(m_appleShape);
}

bool IsOverlap(const SnakeContainer & snakeBody, int itemX, int itemY)
{
	for (auto itr = snakeBody.begin(); itr != snakeBody.end(); ++itr)
	{
		if (itr->position.x == itemX && itr->position.y == itemY)
			return true;
	}
	return false;
}
