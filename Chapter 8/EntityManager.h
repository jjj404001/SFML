#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <unordered_map>
#include <vector>
#include <functional>
#include <algorithm>
#include "Bitmask.h"
#include "C_Base.h"

using EntityId = unsigned int;

using ComponentContainer = std::vector<C_Base*>;
using EntityData = std::pair<Bitmask, ComponentContainer>;
using EntityContainer = std::unordered_map<EntityId, EntityData>;
using ComponentFactory = std::unordered_map<
	Component, std::function<C_Base*(void)>>;

class TextureManager;
class SystemManager;
class EntityManager 
{
public:
	EntityManager(SystemManager * sysMgr, 
		TextureManager * textureMgr);
	~EntityManager();

	int AddEntity(const Bitmask & mask);
	int AddEntity(const std::string & entityFile);
	bool RemoveEntity(const EntityId & id);

	bool AddComponent(const EntityId & entity,
		const Component & component);

	template<class T>
	T* GetComponent(const EntityId & entity,
		const Component & component)
	{
		auto itr = m_entities.find(entity);
		if (itr == m_entities.end())
			return false;

		//Found entity.
		if (!itr->second.first.GetBit((unsigned int)component))
			return nullptr;

		//Component exists.
		auto & container = itr->second.second;
		auto cp = std::find_if(container.begin(), container.end(),
			[&component](C_Base* c) 
			{ return c->GetType() == component; });

		return (cp != container.end() ?
			dynamic_cast<T*>(*cp) : nullptr);
	}

	bool RemoveComponent(const EntityId & entity,
		const Component & component);
	bool HasComponent(const EntityId & entity,
		const Component & component);

	void Purge();

private:
	template<class T>
	void AddComponentType(const Component & id)
	{
		m_cFactory[id] = [] ()->C_Base* { return new T(); };
	}
	
	//Data members
	unsigned int m_idCounter;
	EntityContainer m_entities;
	ComponentFactory m_cFactory;

	SystemManager * m_systems;
	TextureManager * m_textureManager;
};

#endif