#include "EntityManager.h"

EntityManager::EntityManager(SystemManager * sysMgr,
	TextureManager * textureMgr)
{

}

EntityManager::~EntityManager()
{

}

int EntityManager::AddEntity(const Bitmask & mask)
{

}

int EntityManager::AddEntity(const std::string & entityFile)
{

}

bool EntityManager::RemoveEntity(const EntityId & entity)
{

}

bool EntityManager::AddComponent(const EntityId & entity,
	const Component & component)
{

}

bool EntityManager::RemoveComponent(const EntityId & entity,
	const Component & component)
{

}

bool EntityManager::HasComponent(const EntityId & entity,
	const Component & component)
{

}

void EntityManager::Purge()
{

}
