#include "BaseState.h"

class State_Intro : public BaseState
{
public:
	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void Activate();
	virtual void Deactivate();

	virtual void Update(const sf::Time & time);
	virtual void Draw();

	void Continue();

private:
	sf::Texture m_introTexture;
	sf::Sprite m_introSprite;
	sf::Text m_text;
	float m_timePassed;
};