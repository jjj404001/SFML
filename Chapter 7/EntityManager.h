#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

enum class EntityType{Base, Enemy, Player};

enum class EntityState{
	Idle, Walking, Jumping, Attacking, Hurt, Dying};

class EntityManager;
struct TileInfo;

class EntityBase
{
public:
	EntityBase(EntityManager * entityMgr);
	virtual ~EntityBase() {}

	//... Getters & Setters
	void SetPosition(const float & x, const float & y);
	void SetPosition(const sf::Vector2f & pos);
	void SetSize(const float & x, const float & y);
	void SetState(const EntityState & state);

	void Move(float x, float y);
	void AddVelocity(float x, float y);
	void Accelerate(float x, float y);
	void SetAccelerate(float x, float y);
	void ApplyFriction(float x, float y);

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow * wind) = 0;

protected:
	//Methods
	void UpdateAABB();
	void CheckCollisions();
	void ResolveCollisions();

	//Method for what THIS entity does to the collider entity
	virtual void OnEntityCollision(EntityBase * collider, bool attack) = 0;

	//Date mambers
	std::string m_name;
	EntityType m_type;
	unsigned int m_id; // Id in entity manager
	sf::Vector2f m_position;
	sf::Vector2f m_positionOld;
	sf::Vector2f m_velocity;
	sf::Vector2f m_maxVelocity;
	sf::Vector2f m_speed;
	sf::Vector2f m_acceleration;
	sf::Vector2f m_friction;

	TileInfo * m_referenceTile; // Tile underneath entity
	sf::Vector2f m_size; // Size of collision box
	sf::FloatRect m_AABB; // Bounding box for collisions
	EntityState m_state; // Current entity state

	//Flags for remembering axis collisions
	bool m_collidingOnX;
	bool m_collidingOnY;

	Collisions m_collisions;
	EntityManager * m_entityManager;
};

#endif