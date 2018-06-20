#include "State_Game.h"
#include "StateManager.h"
#include "Map.h"
#include "SystemManager.h"
#include "C_Position.h"

void State_Game::OnCreate()
{
	EventManager * evMgr = m_stateMgr->GetContext()
		->m_eventManager;
	evMgr->AddCallback(StateType::Game, "Key_Escape",
		&State_Game::MainMenu, this);
	evMgr->AddCallback(StateType::Game, "Key_P",
		&State_Game::Pause, this);
	evMgr->AddCallback(StateType::Game, "Player_MoveLeft",
		&State_Game::PlayerMove, this);
	evMgr->AddCallback(StateType::Game, "Player_MoveRight",
		&State_Game::PlayerMove, this);
	evMgr->AddCallback(StateType::Game, "Player_MoveUp",
		&State_Game::PlayerMove, this);
	evMgr->AddCallback(StateType::Game, "Player_MoveDown",
		&State_Game::PlayerMove, this);

	sf::Vector2u size = m_stateMgr->
		GetContext()->m_wind->GetWindowSize();
	m_view.setSize(size.x, size.y);
	m_view.setCenter(size.x / 2, size.y / 2);
	m_view.zoom(0.6f);
	m_stateMgr->GetContext()->
		m_wind->GetRenderWindow()->setView(m_view);

	m_gameMap = new Map(m_stateMgr->GetContext(), this);
	m_gameMap->LoadMap("media/Maps/map1.map");

	m_player = m_gameMap->GetPlayerId();
}

void State_Game::OnDestroy()
{
	EventManager * evMgr = m_stateMgr->
		GetContext()->m_eventManager;
	evMgr->RemoveCallback(StateType::Game, "Key_Escape");
	evMgr->RemoveCallback(StateType::Game, "Key_P");

	delete m_gameMap;
	m_gameMap = nullptr;
}

void State_Game::Activate()
{

}

void State_Game::Deactivate()
{

}

void State_Game::Update(const sf::Time & time)
{
	SharedContext * context = m_stateMgr->GetContext();
	UpdateCamera();
	m_gameMap->Update(time.asSeconds());
	context->m_systemManager->Update(time.asSeconds());
}

void State_Game::Draw()
{
	
}

void State_Game::MainMenu(EventDetails * details)
{
	m_stateMgr->SwitchTo(StateType::MainMenu);
}

void State_Game::Pause(EventDetails * details)
{
	m_stateMgr->SwitchTo(StateType::Paused);
}

void State_Game::PlayerMove(EventDetails * details)
{

}

void State_Game::UpdateCamera()
{
	if (m_player == -1)
		return;
	SharedContext * context = m_stateMgr->GetContext();
	C_Position * pos = m_stateMgr->GetContext()->m_entityManager->
		GetComponent<C_Position>(m_player, Component::Position);

	m_view.setCenter(pos->GetPosition());
	context->m_wind->GetRenderWindow()->setView(m_view);

	sf::FloatRect viewSpace = context->m_wind->GetViewSpace();
	if (viewSpace.left <= 0)
	{
		m_view.setCenter(viewSpace.width / 2, m_view.getCenter().y);
		context->m_wind->GetRenderWindow()->setView(m_view);
	}
	else if (viewSpace.left + viewSpace.width > 
		(m_gameMap->GetMapSize().x * Sheet::Tile_Size))
	{
		m_view.setCenter((m_gameMap->GetMapSize().x * Sheet::Tile_Size) - 
			(viewSpace.width / 2), m_view.getCenter().y);
		context->m_wind->GetRenderWindow()->setView(m_view);
	}

	if (viewSpace.top <= 0)
	{
		m_view.setCenter(m_view.getCenter().x, viewSpace.height / 2);
		context->m_wind->GetRenderWindow()->setView(m_view);
	}
	else if(viewSpace.top + viewSpace.height > 
		(m_gameMap->GetMapSize().y) * Sheet::Tile_Size)
	{
		m_view.setCenter(m_view.getCenter().x,
			((m_gameMap->GetMapSize().y) * Sheet::Tile_Size) - 
			(viewSpace.height / 2));
		context->m_wind->GetRenderWindow()->setView(m_view);
	}
}
