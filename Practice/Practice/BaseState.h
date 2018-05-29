#ifndef BASESTATE_H
#define BASESTATE_H

class StateManager;

class BaseState
{
public:
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void Create() = 0;
	virtual void Destroy() = 0;

private:
	StateManager * m_stateMgr;
	bool isTransparent;
	bool isTranscendent;
};

#endif
