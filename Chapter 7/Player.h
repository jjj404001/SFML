#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "EventManager.h"
#include "EntityManager.h"

class Player : public Character
{
public:
	Player(EntityManager * entityMgr);
	~Player();

	void OnEntityCollision(EntityBase * collider, bool attack);
	void React(EventDetails * details);
};

#endif