#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <unordered_map>
#include "MessageHandler.h"
#include "S_Base.h"

using SystemContainer = std::unordered_map<System, S_Base*>;
using EntityEventContainer = std::unordered_map<EntityId, EventQueue>;

class EntityManager;
class Window;
class SystemManager
{
public:
	SystemManager();
	~SystemManager();

	void SetEntityManager(EntityManager * entityMgr);
	EntityManager * GetEntityManager();
	MessageHandler * GetMessageHandler();

	template<class T>
	T* GetSystem(const System & system)
	{
		auto itr = m_systems.find(system);
		return (itr != m_systems.end() ?
			dynamic_cast<T*>(itr->second) : nullptr);
	}

	void AddEvent(const EntityId & entity,
		const EventID & evnet);

	void Update(float dt);
	void HandleEvents();
	void Draw(Window * wind, unsigned int elevation);

	void EntityModified(const EntityId & entity,
		const Bitmask & bits);
	void RemoveEntity(const EntityId & entity);

	void PurgeEntities();
	void PurgeSystems();

private:
	SystemContainer m_systems;
	EntityManager * m_entityManager;
	EntityEventContainer m_events;
	MessageHandler m_message;
};

#endif