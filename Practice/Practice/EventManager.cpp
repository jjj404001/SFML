#include <fstream>
#include <sstream>
#include <iostream>
#include <typeinfo>
#include "EventManager.h"


EventManager::EventManager()
{
	LoadBinginds();
}

EventManager::~EventManager()
{
	for (auto itr = m_bindings.begin();
		itr != m_bindings.end(); ++itr)
	{
		delete itr->second;
	}
	m_bindings.clear();
}

void EventManager::Update()
{
	for (auto itr = m_bindings.begin();
		itr != m_bindings.end(); ++itr)
	{
		auto temp = itr->second;

		for (auto itr2 = temp->m_events.begin();
			itr2 != temp->m_events.end(); ++itr2)
		{
			if (itr2->first == EventType::KeyDown && 
				sf::Keyboard::isKeyPressed(sf::Keyboard::Key
				(itr2->second.m_code)))
			{
				++temp->m_details.c;
			}
			else if (itr2->first == EventType::MButtonDown && 
				sf::Mouse::isButtonPressed(sf::Mouse::Button
				(itr2->second.m_code)))
			{
				++temp->m_details.c;
			}
		}

		if (temp->m_details.c == temp->m_events.size())
		{
			auto itr = m_callbacks.find(temp->m_name);
			if (itr != m_callbacks.end())
				itr->second(&temp->m_details);
		}

		temp->m_details.c = 0;
	}
}

void EventManager::HandleEvent(sf::Event event)
{
	for (auto itr = m_bindings.begin();
		itr != m_bindings.end(); ++itr)
	{
		auto temp = itr->second;
		for (auto itr2 = temp->m_events.begin();
			itr2 != temp->m_events.end(); ++itr2)
		{
			EventType type = itr2->first;
			if (type != EventType(event.type))
				continue;

			if (type == EventType::KeyDown &&
				event.key.code == itr2->second.m_code)
				++temp->m_details.c;
			else if (type == EventType::MButtonDown &&
				event.key.code == itr2->second.m_code)
				++temp->m_details.c;
		}
	}
}

void EventManager::LoadBinginds()
{
	std::ifstream inFile("keys.cfg");
	if (!inFile.is_open())
	{
		std::cout << "Cannot open the file\n" << std::endl;
		return;
	}
	std::stringstream sstr;

	std::string line;
	std::string name;
	std::string code;

	while (getline(inFile, line))
	{
		Binding * bind = new Binding;

		sstr.str(line);
		sstr >> name;
		bind->m_name = name;
		bind->m_details.m_name = name;

		while (sstr >> code)
		{
			//std::cout << typeid(code.find(":")).name() << std::endl;

			bind->m_events.emplace_back(
				EventType(std::stoi(code.substr(0, code.find(":")))),
				EventCode(std::stoi(code.substr(code.find(":") + 1,
					code.size() - code.find(":")))) );

		}

		m_bindings.emplace(name, bind);
		name.clear();
		code.clear();
		line.clear();
	}
}
