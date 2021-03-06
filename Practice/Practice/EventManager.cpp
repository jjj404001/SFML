#include <fstream>
#include <sstream>
#include <iostream>
#include "EventManager.h"

//Ctor & Dtor
EventManager::EventManager()
	: m_isFocus(true), m_currentState(StateType(0))
{
	//Initialize member 'm_bindings'
	LoadBinginds();
}

EventManager::~EventManager()
{
	for (auto &itr : m_bindings)
	{
		delete itr.second;
		itr.second = nullptr;
	}
	m_bindings.clear();
}

//Event Handler
void EventManager::HandleEvent(sf::Event event)
{
	for (auto &itr : m_bindings)
	{
		Binding * bind = itr.second;

		for (auto &itr2 : bind->m_events)
		{
			EventType type = itr2.first;
			//Skip itr that does not match EventType
			if (type != EventType(event.type))
				continue;

			//Check the specific EventType & EventCode
			//KeyboardDown
			if (type == EventType::KeyDown || type == EventType::KeyUp)
			{
				if (event.key.code == itr2.second.m_code)
				{
					++bind->c;
					break;
				}
			}
			//MouseButtonDown
			else if (type == EventType::MButtonDown ||
					 type == EventType::MButtonUp)
			{
				if (event.key.code == itr2.second.m_code)
				{
					++bind->c;
					break;
				}
			}
			//Other events
			else
				++bind->c;
		}
	}
}

//Realtime input checking
void EventManager::Update()
{
	//Check window focus
	if (!m_isFocus) { return; }

	for (auto &itr : m_bindings)
	{
		Binding * bind = itr.second;

		for (auto &itr2 : bind->m_events)
		{
			//Check the EventType & EventCode
			if (itr2.first == EventType::Keyboard &&
				sf::Keyboard::isKeyPressed(sf::Keyboard::Key
				(itr2.second.m_code)))
				++bind->c;
			else if (itr2.first == EventType::Mouse &&
				sf::Mouse::isButtonPressed(sf::Mouse::Button
				(itr2.second.m_code)))
				++bind->c;
		}

		//Consider that sf::event is already dealt with!!
		//If all Event are "on"
		if (bind->c == bind->m_events.size())
		{
			//Check global callbacks first 
			auto itr = m_callbacks.find(StateType(0));
			auto itr2 = itr->second.find(bind->m_name);
			if (itr2 != itr->second.end())
			{

				itr2->second(&bind->m_details);
			}
			else //Check callbacks in current state
			{
				auto itr = m_callbacks.find(m_currentState);
				if (itr != m_callbacks.end())
				{
					auto itr2 = itr->second.find(bind->m_name);
					if (itr2 != itr->second.end())
						itr2->second(&bind->m_details);
				}
			}
		}

		//Reset
		bind->c = 0;
	}
}

///////////////////////Helper method
//Initialize the member 'm_bindings'
void EventManager::LoadBinginds()
{
	//Open file
	std::ifstream inFile("keys.cfg");

	//Check if the file is opened properly
	if (!inFile.is_open())
	{
		std::cout << "Cannot open the file\n";
		return;
	}

	//string variabls
	std::istringstream sstr; 
	std::string line; //Temporary string
	std::string name; //Name of binding
	std::string code; //EventType & EventCode

	//Read a line
	while (getline(inFile, line)) // ex) Window_Close 0:0
	{
		sstr.str(line); //Set stringstream
		sstr >> name; //Get the name of binding

		Binding * bind = new Binding(name);

		//Get a pair of code
		while (sstr >> code) // ex) 0:0
		{
			//Add a 'Event'
			bind->m_events.emplace_back(
				EventType(std::stoi(code.substr(0, code.find(":")))),
				EventCode(std::stoi(code.substr(code.find(":") + 1,
					code.size() - code.find(":")))) );
		}

		//Add a binding if 'name' is unique
		if (!m_bindings.emplace(name, bind).second)
			delete bind;
		// Clear sstr for next code pair
		sstr.clear();
	}

	//Close File
	inFile.close();
}
