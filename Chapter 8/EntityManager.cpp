#include <fstream>
#include "EntityManager.h"
#include "C_Position.h"
#include "Utilities.h"

EntityManager::EntityManager(SystemManager * sysMgr,
	TextureManager * textureMgr) : m_idCounter(0),
	m_systems(sysMgr), m_texutreManager(textureMgr)
{
	AddComponentType<C_Position>(Component::Position);
	//AddComponentType<C_SpriteSheet>(Component::SpriteSheet);
	//AddComponentType<C_State>(Component::State);
	//AddComponentType<C_Movable>(Component::Movable);
	//AddComponentType<C_Controller>(Component::Controller);
	//AddComponentType<C_Collidable>(Component::Collidable);
}

EntityManager::~EntityManager() { Purge(); }

int EntityManager::AddEntity(const Bitmask & mask)
{
	unsigned int entity = m_idCounter;
	if (!m_entities.emplace(entity,
		EntityData(0, ComponentContainer())).second)
		return -1;

	//One empty entity is successfully added
	++m_idCounter;

	//Adding components
	for (unsigned int i = 0; i < N_COMPONENT_TYPES; ++i)
	{
		if (mask.GetBit(i))
			AddComponent(entity, (Component)i);
	}

	//Notifying the system manager of a modified entity
	m_systems->EntityModified(entity, mask);
	m_systems->AddEvent(entity, (EventID)EntityEvent::Spawned);
	return entity;
}

int EntityManager::AddEntity(const std::string & entityFile)
{
	int EntityId = -1;

	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() +
		"media/Entities/" + entityFile + ".entity");
	if (!file.is_open())
	{
		std::cerr << "! Failed to load entity: "
			<< entityFile << std::endl;
		return -1;
	}
	
	std::string line;
	while (std::getline(file, line))
	{
		if (line[0] == '|')
			continue;
		std::stringstream keystream(line);
		std::string type;
		keystream >> type;

		if (type == "Name")
		{

		}
		else if (type == "Attributes")
		{
			if (EntityId != -1)
				continue;

			Bitset set = 0;
			Bitmask mask;
			keystream >> set;
			mask.SetMask(set);
			EntityId = AddEntity(mask);

			if (EntityId == -1)
				return -1;
		}
		else if (type == "Component")
		{
			if (EntityId == -1)
				continue;

			unsigned int id = 0;
			keystream >> id;

			C_Base * component = GetComponent<C_Base>
				(EntityId, (Component)id);
			if (!component)
				continue;
			keystream >> *component;
			if (component->GetType() == Component::SpriteSheet)
			{
				C_SpriteSheet * sheet = (C_SpriteSheet*)component;
				sheet->Create(m_textureManager);
			}
		}
	}
	file.close();
	return EntityId;
}

bool EntityManager::RemoveEntity(const EntityId & id)
{
	auto itr = m_entities.find(id);
	if (itr == m_entities.end())
		return false;

	//Removing all components
	while (itr->second.second.begin() != itr->second.second.end())
	{
		delete itr->second.second.back();
		itr->second.second.pop_back();
	}
	m_entities.erase(itr);
	m_systems->RemoveEntity(id);
	return true;
}

bool EntityManager::AddComponent(const EntityId & entity,
	const Component & component)
{
	auto itr = m_entities.find(entity);
	if (itr == m_entities.end())
		return false;
	if (itr->second.first.GetBit((unsigned int)component))
		return false;
	//Component doesn't exist

	auto itr2 = m_cFactory.find(component);
	if (itr2 == m_cFactory.end())
		return false;
	//Component type does exist

	C_Base * component = itr2->second();
	itr->second.second.emplace_back(component);
	itr->second.first.TurnOnBit((unsigned int)component);

	//Notifying the system manager of a modified entity
	m_systems->EntityModifies(entity, itr->second.first);
	return true;

}

bool EntityManager::RemoveComponent(const EntityId & entity,
	const Component & component)
{
	auto itr = m_entities.find(entity);
	if (itr == m_entities.end())
		return false;
	//Found the entity

	if (!itr->second.first.GetBit((unsigned int)component))
		return false;
	//Component exist

	auto & container = itr->second.second;
	auto cp = std::find_if(container.begin(), container.end(),
		[&component](C_Base* c) {return c->GetType() == component; });

	if (cp == container.end())
		return false;

	delete (*cp);
	container.erase(cp);
	itr->second.first.ClearBit((unsigned int)component);

	m_systems->EntityModifies(entity, itr->second.first);
	return true;
}

bool EntityManager::HasComponent(const EntityId & entity,
	const Component & component)
{
	auto itr = m_entities.find(entity);
	if (itr == m_entities.end())
		return false;
	return itr->second.first.GetBit((unsigned int)component);
}

void EntityManager::Purge()
{
	m_systems->PurgeEntities();
	for (auto & entity : m_entities)
	{
		for (auto & component : entity.second.second)
			delete component;
		entity.second.second.clear();
		entity.second.first.Clear();
	}
	m_entities.clear();
	m_idCounter = 0;
}
