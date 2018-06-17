#ifndef S_BASE_H
#define S_BASE_H

#include <vector>

using System = unsigned int;
using EntityId = unsigned int;
class Bitmask;

using EntityList = std::vector<EntityId>;
using Requirements = std::vector<Bitmask>;

class SystemManager;
class S_Base
{
public:
	S_Base(const System & id, SystemManager * systemMgr);
	virtual ~S_Base();

	bool AddEntity(const EntityId & entity);
	bool HasEntity(const EntityId & entity);
	bool RemoveEntity(const EntityId & entity);

	System GetId();

	bool FitsRequirements(const Bitmask & entity);
	void Purge();

protected:
	System m_id;
	Requirements m_requireComponents;
	EntityList m_entities;

	SystemManager * m_systemManager;
};
#endif