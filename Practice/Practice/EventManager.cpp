#include <fstream>
#include <sstream>
#include "EventManager.h"

//Ctor & Dtor
EventManager::EventManager()
{
	//Initialize member 'm_bindings'
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

//Event Handler
void EventManager::HandleEvent(sf::Event event)
{
	//For every Binding in evMgr
	for (auto itr = m_bindings.begin();
		itr != m_bindings.end(); ++itr)
	{
		Binding * bind = itr->second;

		//For every Event in current Binding
		for (auto itr2 = bind->m_events.begin();
			itr2 != bind->m_events.end(); ++itr2)
		{
			EventType type = itr2->first;
			//Skip itr that does not match EventYype
			if (type != EventType(event.type))
				continue;

			//Check the specific EventType & EventCode
			//KeyboardDown
			if (type == EventType::KeyDown &&
				event.key.code == itr2->second.m_code)
				++bind->c;
			//MouseButtonDown
			else if (type == EventType::MButtonDown &&
				event.key.code == itr2->second.m_code)
				++bind->c;
			//WindowCloseButton
			else if (type == EventType::Closed)
				++bind->c;
		}
	}
}

//Realtime input checking
void EventManager::Update()
{
	//For every Binding in evMgr
	for (auto itr = m_bindings.begin();
		itr != m_bindings.end(); ++itr)
	{
		Binding * bind = itr->second;

		//For every Event in current Binding
		for (auto itr2 = bind->m_events.begin();
			itr2 != bind->m_events.end(); ++itr2)
		{
			//Check the EventType & EventCode
			if (itr2->first == EventType::Keyboard &&
				sf::Keyboard::isKeyPressed(sf::Keyboard::Key
				(itr2->second.m_code)))
				++bind->c;
			else if (itr2->first == EventType::Mouse &&
				sf::Mouse::isButtonPressed(sf::Mouse::Button
				(itr2->second.m_code)))
				++bind->c;
		}

		//Consider that sf::event is already dealt with!!
		//If all Event are "on"
		if (bind->c == bind->m_events.size())
		{
			//Find matching callback
			auto itr = m_callbacks.find(bind->m_name);
			//Call the function
			if (itr != m_callbacks.end())
				itr->second(&bind->m_details);
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

	//string values
	std::istringstream sstr; 
	std::string line; //Temporary string
	std::string name; //Name of binding
	std::string code; //EventType & EventCode

	//Read a line
	while (getline(inFile, line)) // ex) Window_Close 0:0
	{
		//Make a new binding
		Binding * bind = new Binding;

		sstr.str(line); //Set stringstream
		sstr >> name; //Get the name of binding

		//Init the name data
		bind->m_name = name;
		bind->m_details.m_name = name;

		//Get a pair of code
		while (sstr >> code) // ex) 0:0
		{
			//Adding a 'Event'
			bind->m_events.emplace_back(
				EventType(std::stoi(code.substr(0, code.find(":")))),
				EventCode(std::stoi(code.substr(code.find(":") + 1,
					code.size() - code.find(":")))) );
		}

		//Add a binding
		m_bindings.emplace(name, bind);
		// Clear sstr for next code pair
		sstr.clear();
	}
}
