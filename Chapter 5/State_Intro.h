#include "BaseState.h"

struct EventDetails;

class State_Intro : public BaseState
{
public:
	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void Activate() {}
	virtual void Deactivate() {}

	virtual void Update(const sf::Time & time);
	virtual void Draw();

	void Continue(EventDetails * details);

private:
	sf::Texture m_introTexture;
	sf::Sprite m_introSprite;
	sf::Text m_text;
	sf::Font m_font;
	float m_timePassed;
};