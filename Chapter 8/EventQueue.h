#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <queue>

enum class EntityEvent {
	Spawned, Despawned, Colliding_X, Colliding_Y,
	Moving_Left, Moving_Right, Moving_Up, Moving_Down,
	Elevation_Change, Became_Idle, Began_Moving
};

using EventID = unsigned int;

class EventQueue
{
public:
	void AddEvent(const EventID & event)
	{
		m_queue.push(event);
	}

	bool ProcessEvents(EventID & id)
	{
		if (m_queue.empty())
			return false;
		id = m_queue.front();
		m_queue.pop();
		return true;
	}

	void Clear()
	{
		while (!m_queue.empty())
			m_queue.pop();
	}

private:
	std::queue<EventID> m_queue;
};

#endif