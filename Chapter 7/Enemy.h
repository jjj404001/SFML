#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"

class Enemy : public Character
{
public:
	Enemy(EntityManager * entityMgr);
	~Enemy();

	void OnEntityCollision(EntityBase * collider, bool attack);
	void Update(float dt);

private:
	sf::Vector2f m_destination;
	bool m_hasDestination;
};

#endif