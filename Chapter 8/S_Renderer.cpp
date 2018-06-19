#include "S_Renderer.h"
#include "SystemManager.h"
#include "Bitmask.h"

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

}

void S_Renderer::HandleEvent(const EntityId & entity,
	const EntityEvent & event)
{

}

//Observer method
void S_Renderer::Notify(const Message & message)
{

}

//S_Renderer method
void S_Renderer::Render(Window * wind, unsigned int layer)
{

}

void S_Renderer::SetSheetDirection(const EntityId & entity,
	const Direction & dir)
{

}

void S_Renderer::SortDrawables()
{

}
