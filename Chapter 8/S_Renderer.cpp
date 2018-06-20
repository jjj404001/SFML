#include "S_Renderer.h"
#include "SystemManager.h"
#include "EntityManager.h"
#include "Bitmask.h"
#include "Direction.h"
#include "C_Position.h"
#include "C_Drawable.h"
#include "C_SpriteSheet.h"
#include "Window.h"

//Ctor & Dtor
S_Renderer::S_Renderer(SystemManager * systemMgr)
	: S_Base(System::Renderer, systemMgr)
{
	Bitmask req;
	req.TurnOnBit((unsigned int)Component::Position);
	req.TurnOnBit((unsigned int)Component::SpriteSheet);
	m_requireComponents.push_back(req);
	req.Clear();

	m_systemManager->GetMessageHandler()->
		Subscribe(EntityMessage::Direction_Changed, this);
}

S_Renderer::~S_Renderer() {}

//S_Base methods
void S_Renderer::Update(float dt)
{
	EntityManager * entities = m_systemManager->GetEntityManager();
	for (auto & entity : m_entities)
	{
		C_Position * position = entities->
			GetComponent<C_Position>(entity, Component::Position);
		C_Drawable * drawable = nullptr;
		if (entities->HasComponent(entity, Component::SpriteSheet))
		{
			drawable = entities->
				GetComponent<C_Drawable>(entity, Component::SpriteSheet);
		}
		else
			continue;
		drawable->UpdatePosition(position->GetPosition());
	}
}

void S_Renderer::HandleEvent(const EntityId & entity,
	const EntityEvent & event)
{
	if (event == EntityEvent::Moving_Left ||
		event == EntityEvent::Moving_Right ||
		event == EntityEvent::Moving_Up ||
		event == EntityEvent::Moving_Down ||
		event == EntityEvent::Elevation_Change ||
		event == EntityEvent::Spawned)
	{
		SortDrawables();
	}
}

//Observer method
void S_Renderer::Notify(const Message & message)
{
	if (HasEntity(message.m_receiver))
	{
		EntityMessage m = (EntityMessage)message.m_type;
		switch (m)
		{
		case EntityMessage::Direction_Changed:
			SetSheetDirection(message.m_receiver,
				(Direction)message.m_int);
			break;
		}
	}
}

//S_Renderer method
void S_Renderer::Render(Window * wind, unsigned int layer)
{
	EntityManager * entities = m_systemManager->GetEntityManager();
	for (auto & entity : m_entities)
	{
		C_Position * position = entities->
			GetComponent<C_Position>(entity, Component::Position);

		// Z - order
		if (position->GetElevation() < layer)
			continue;
		if (position->GetElevation() > layer)
			break;
		C_Drawable * drawable = nullptr;

		//Getting Spritesheet component
		if (!entities->HasComponent(entity, Component::SpriteSheet))
			continue;
		drawable = entities->
			GetComponent<C_Drawable>(entity, Component::SpriteSheet);

		//Getting sprite bound
		sf::FloatRect drawableBounds;
		drawableBounds.left = 
			position->GetPosition().x - (drawable->GetSize().x / 2);
		drawableBounds.top = 
			position->GetPosition().y - drawable->GetSize().y;
		drawableBounds.width = drawable->GetSize().x;
		drawableBounds.height = drawable->GetSize().y;

		//"Entity Culling"
		if (!wind->GetViewSpace().intersects(drawableBounds))
			continue;

		drawable->Draw(wind->GetRenderWindow());
	}
}

//Private methods
void S_Renderer::SetSheetDirection(const EntityId & entity,
	const Direction & dir)
{
	EntityManager * entities = m_systemManager->GetEntityManager();
	if (!entities->HasComponent(entity, Component::SpriteSheet))
		return;
	C_SpriteSheet * sheet = entities->
		GetComponent<C_SpriteSheet>(entity, Component::SpriteSheet);
	sheet->GetSpriteSheet()->SetDirection(dir);
}

// Z - order sorting
void S_Renderer::SortDrawables()
{
	EntityManager * e_mgr = m_systemManager->GetEntityManager();

	std::sort(m_entities.begin(), m_entities.end(),
		[e_mgr](unsigned int lhs, unsigned int rhs)
		{
			auto pos1 = 
				e_mgr->GetComponent<C_Position>(lhs, Component::Position);
			auto pos2 = 
				e_mgr->GetComponent<C_Position>(rhs, Component::Position);
			if (pos1->GetElevation() == pos2->GetElevation())
				return pos1->GetPosition().y < pos2->GetPosition().y;
			return pos1->GetElevation() < pos2->GetElevation();
	});
}
