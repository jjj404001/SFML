#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <unordered_map>
#include "EntityMessage.h"
#include "Communicator.h"

//Entity holds data
//System(Observer) holds entities
//Communicator gathers systems
//Each communicator has specific task(or message)

using Subscribtion = std::unordered_map<EntityMessage, Communicator>;

class MessageHandler
{
public:
	bool Subscribe(const EntityMessage & type,
		Observer * observer)
	{
		return m_commmunicators[type].AddObserver(observer);
	}

	bool Unsubscribe(const EntityMessage & type,
		Observer * observer)
	{
		return m_commmunicators[type].RemoveObserver(observer);
	}

	void Dispatch(const Message & msg)
	{
		auto itr = m_commmunicators.find((EntityMessage)msg.m_type);
		if (itr == m_commmunicators.end())
			return;
		itr->second.Broadcast(msg);
	}

private:
	Subscribtion m_commmunicators;
};

#endif