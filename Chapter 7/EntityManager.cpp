#include "EntityManager.h"
#include "StateManager.h"

EntityManager::EntityManager(SharedContext * context,
	unsigned int maxEntities)
	: m_context(context), m_maxEntities(maxEntities),
	  m_idCounter(0)
{
	LoadEnemyTypes("EnemyList.list");
	RegisterEntity<Player>(EntityType::Player);
	RegisterEntity<Enemy>(EntityType::Enemy);
}

EntityManager::~EntityManager()
{
	Purge();
}

int EntityManager::Add(const EntityType & type,
	const std::string & name = "")
{
	auto itr = m_entityFactory.find(type);
	if (itr == m_entityFactory.end())
		return -1;
	EntityBase * entity = itr->second();

	entity->m_id = m_idCounter;
	if (name != "")
		entity->m_name = name;
	m_entities.emplace(m_idCounter, entity);

	if (type == EntityType::Enemy)
	{
		auto itr = m_enemyTypes.find(name);
		if (itr != m_enemyTypes.end())
		{
			Enemy * enemy = (Enemy*)entity;
			enemy->Load(itr->second());
		}
	}

	++m_idCounter;
	return m_idCounter - 1;
}

EntityBase * EntityManager::Find(unsigned int id)
{
	auto itr = m_entities.find(id);
	if (itr == m_entities.end())
		return nullptr;
	return itr->second;
}

EntityBase * EntityManager::Find(const std::string & name)
{
	for (auto & itr : m_entities)
	{
		if (itr.second->GetName() == name)
			return itr.second;
	}
	return nullptr;
}

void EntityManager::Remove(unsigned int id)
{
	m_entitiesToRemove.emplace_back(id);
}

void EntityManager::Update(float dt)
{
	for (auto & itr : m_entities)
		itr.second->Update(dt);
	EntityCollisionCheck();
	ProcessRemovals();
}

void EntityManager::Draw()
{
	sf::RenderWindow * wd = m_context->m_wind->GetRenderWindow();
	sf::FloatRect viewSpace = m_context->m_wind->GetViewSpace();

	for (auto & itr : m_entities)
	{
		if (!viewSpace.intersects(itr.second->m_AABB))
			continue;
		itr.second->Draw(wd);
	}
}

void EntityManager::Purge()
{
	for (auto & itr : m_entities)
		delete itr.second;
	m_entities.clear();
	m_idCounter = 0;
}

void EntityManager::ProcessRemovals()
{

}

void EntityManager::LoadEnemyTypes(const std::string & name)
{

}

void EntityManager::EntityCollisionCheck()
{

}
