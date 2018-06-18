#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <vector>
#include <algorithm>
#include "Observer.h"

using ObserverContainer = std::vector<Observer*>;

class Communicator
{
public:
	~Communicator() { m_observers.clear(); }

	bool AddObserver(Observer * observer)
	{
		if (HasObserver(observer))
			return false;
		m_observers.emplace_back(observer);
		return true;
	}

	bool RemoveObserver(Observer * observer)
	{
		auto itr = std::find_if(m_observers.begin(),
			m_observers.end(),
			[&observer](Observer * o) {return o == observer; });
		if (itr == m_observers.end())
			return false;
		m_observers.erase(itr);
		return true;
	}

	bool HasObserver(const Observer* observer)
	{
		return std::find_if(m_observers.begin(), m_observers.end(),
			[&observer](Observer* o) {return o == observer; })
			!= m_observers.end();
	}

	void Broadcast(const Message & msg)
	{
		for (auto & itr : m_observers)
			itr->Notify(msg);
	}

private:
	ObserverContainer m_observers;
};

#endif