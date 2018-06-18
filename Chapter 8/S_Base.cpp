#include "SystemManager.h"
#include "S_Base.h"
#include "Bitmask.h"

S_Base::S_Base(const System & id, SystemManager * systemMgr)
	: m_id(id), m_systemManager(systemMgr) { }

S_Base::~S_Base() { Purge(); }

bool S_Base::AddEntity(const EntityId & entity)
{
	if (HasEntity(entity))
		return false;
	m_entities.emplace_back(entity);
	return true;
}

bool S_Base::HasEntity(const EntityId & entity)
{
	return std::find(m_entities.begin(), m_entities.end(), entity)
		!= m_entities.end();
}

bool S_Base::RemoveEntity(const EntityId & entity)
{
	auto itr = std::find_if(m_entities.begin(), m_entities.end(),
		[&entity](EntityId & id) {return id == entity; });
	if (itr == m_entities.end())
		return false;
	m_entities.erase(itr);
	return true;
}

System S_Base::GetId()
{
	return m_id;
}

bool S_Base::FitsRequirements(const Bitmask & bits)
{
	return std::find_if(m_requireComponents.begin(),
		m_requireComponents.end(), 
		[&bits](Bitmask & b) { return b.Matches(bits, b.GetMask()); })
		!= m_requireComponents.end();
}

void S_Base::Purge()
{
	m_entities.clear();
}
