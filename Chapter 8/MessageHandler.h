#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <unordered_map>
#include "EntityMessage.h"
#include "Communicator.h"

using MessageType = unsigned int;
struct TwoFloats { float m_x; float m_y; };

struct Message
{
	Message(const MessageType & type) : m_type(type) {}

	MessageType m_type;
	int m_sender;
	int m_receiver;
	union
	{
		TwoFloats m_2f;
		bool m_bool;
		int m_int;
	};
};

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