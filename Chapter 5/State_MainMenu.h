#include "BaseState.h"



class State_MainMenu : public BaseState
{
public:
	State_MainMenu(StateManager * stateMgr)
		: BaseState(stateMgr) {}

	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void Activate();
	virtual void Deactivate();

	virtual void Update(const sf::Time & time);
	virtual void Draw();

	void MouseClick(EventDetails * details);

private:
	sf::Text m_text;
	sf::Font m_font;

	sf::Vector2f m_buttonSize;
	sf::Vector2f m_buttonPos;
	unsigned int m_buttonPadding;

	sf::RectangleShape m_rects[3];
	sf::Text m_labels[3];

};