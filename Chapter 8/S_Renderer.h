#ifndef S_RENDERER_H
#define S_RENDERER_H

#include "S_Base.h"

class Window;
class S_Renderer : public S_Base
{
public:
	S_Renderer(SystemManager * systemMgr);
	~S_Renderer();

	//S_Base methods
	void Update(float dt);
	void HandleEvent(const EntityId & entity,
		const EntityEvent & event);

	//Observer method
	void Notify(const Message & message);

	//S_Renderer method
	void Render(Window * wind, unsigned int layer);

private:
	void SetSheetDirection(const EntityId & entity,
		const Direction & dir);
	void SortDrawables();
};

#endif