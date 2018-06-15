#ifndef CHARACTER_H
#define CHARACTER_H

#include "EntityBase.h"
#include "SpriteSheet.h"

class Character : public EntityBase
{
	friend class EntityManager;
public:
	Character(EntityManager * entityMgr);
	virtual ~Character() {}

	void Move(const Direction & dir);
	void Jump();
	void Attack();
	void GetHurt(const int damage);
	void Load(const std::string & path);
	SpriteSheet * GetSheet() { return &m_spriteSheet; }

	virtual void OnEntityCollision(EntityBase * collider, bool attack) = 0;
	virtual void Update(float dt);
	void Draw(sf::RenderWindow * wind);

protected:
	void UpdateAttackAABB();
	void Animate();

	SpriteSheet m_spriteSheet;
	float m_jumpVelocity;
	int m_hitpoints;
	sf::FloatRect m_attackAABB;
	sf::RectangleShape m_attackShape;
	sf::Vector2f m_attackAABBoffset;
};

#endif