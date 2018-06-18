#include "SystemManager.h"

SystemManager::SystemManager() : m_entityManager(nullptr)
{/*
	m_systems[System::State] = new S_State(this);
	m_systems[System::Control] = new S_Control(this);
	m_systems[System::Movement] = new S_Movement(this);
	m_systems[System::Collision] = new S_Collision(this);
	m_systems[System::SheetAnimation] = new S_SheetAnimation(this);
	m_systems[System::Renderer] = new S_Renderer(this);*/
}

SystemManager::~SystemManager()
{
	PurgeSystems();
}

void SystemManager::SetEntityManager(EntityManager * entityMgr)
{
	if(!m_entityManager)
		m_entityManager = entityMgr;
}

EntityManager * SystemManager::GetEntityManager()
{
	return m_entityManager;
}

MessageHandler * SystemManager::GetMessageHandler()
{
	return &m_message;
}

void SystemManager::AddEvent(const EntityId & entity,
	const EventID & event)
{
	m_events[entity].AddEvent(event);
}

void SystemManager::Update(float dt)
{
	for (auto & itr : m_systems)
		itr.second->Update(dt);
	HandleEvents();
}

void SystemManager::HandleEvents()
{
	for (auto & event : m_events)
	{
		EventID id = 0;
		while (event.second.ProcessEvents(id))
		{
			for (auto & system : m_systems)
			{
				if (system.second->HasEntity(event.first))
					system.second->HandleEvent(event.first, (EntityEvent)id);
			}
		}
	}
}

void SystemManager::Draw(Window * wind, unsigned int elevation)
{

}

void SystemManager::EntityModified(const EntityId & entity,
	const Bitmask & bits)
{

}

void SystemManager::RemoveEntity(const EntityId & entity)
{

}

void SystemManager::PurgeEntities()
{

}

void SystemManager::PurgeSystems()
{

}
