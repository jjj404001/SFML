#include "State_MainMenu.h"
#include "StateManager.h"

void State_MainMenu::OnCreate()
{
	//Font & text setting
	m_font.loadFromFile("endomca.ttf");
	m_text.setFont(m_font);
	m_text.setString(sf::String("MAIN MENU:"));
	m_text.setCharacterSize(18);

	//Setting text origin & position
	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	m_text.setPosition(400, 100);

	//Some initialize
	m_buttonSize = sf::Vector2f(300.0f, 32.0f);
	m_buttonPos = sf::Vector2f(400, 200);
	m_buttonPadding = 4; // 4px

	//string for text
	std::string str[3];
	str[0] = "PLAY";
	str[1] = "CREDITS";
	str[2] = "EXIT";

	for (int i = 0; i < 3; ++i)
	{
		//Setting 3 button position
		sf::Vector2f buttonPos(m_buttonPos.x, m_buttonPos.y +
			(i * (m_buttonSize.y + m_buttonPadding)));

		//Rectangle setting
		m_rects[i].setSize(m_buttonSize);
		m_rects[i].setFillColor(sf::Color::Red);

		//Setting rectangle origin & position
		m_rects[i].setOrigin(m_buttonSize.x / 2.0f,
			m_buttonSize.y / 2.0f);
		m_rects[i].setPosition(buttonPos);

		//Label setting
		m_labels[i].setFont(m_font);
		m_labels[i].setString(sf::String(str[i]));
		m_labels[i].setCharacterSize(12);

		//Setting label origin & position
		sf::FloatRect rect = m_labels[i].getLocalBounds();
		m_labels[i].setOrigin(rect.left + rect.width / 2.0f,
			rect.top + rect.height / 2.0f);
		m_labels[i].setPosition(buttonPos);
	}

	EventManager * evMgr = m_stateMgr->
		GetContext()->m_eventManager;
	evMgr->AddCallback(StateType::MainMenu, "Mouse_Left",
		&State_MainMenu::MouseClick, this);
}

void State_MainMenu::OnDestroy()
{
	EventManager * evMgr = m_stateMgr->
		GetContext()->m_eventManager;
	evMgr->RemoveCallback(StateType::MainMenu, "Mouse_left");
}

void State_MainMenu::Activate()
{

}

void State_MainMenu::Deactivate()
{

}

void State_MainMenu::Update(const sf::Time & time)
{

}

void State_MainMenu::Draw()
{

}

void State_MainMenu::MouseClick(EventDetails * details)
{

}
