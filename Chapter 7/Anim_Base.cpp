#include "Anim_Base.h"
#include "SpriteSheet.h"

Anim_Base::Anim_Base()
	: m_frameCurrent(0), m_frameStart(0), m_frameEnd(0),
	m_frameRow(0), m_frameTime(0.f), m_elapsedTime(0.f),
	m_frameActionStart(-1), m_frameActionEnd(-1),
	m_loop(false), m_playing(false) {}

//...Setters & getters
void Anim_Base::SetSpriteSheet(SpriteSheet * sheet)
{
	m_spriteSheet = sheet;
}

void Anim_Base::SetFrame(const unsigned int & frame)
{
	if ((frame >= m_frameStart && frame <= m_frameEnd) ||
		(frame >= m_frameEnd && frame <= m_frameStart))
		m_frameCurrent = frame;
}

bool Anim_Base::IsInAction()
{
	if (m_frameActionStart == -1 || m_frameActionEnd == -1)
		return true;
	return (m_frameCurrent >= m_frameActionStart 
		&& m_frameCurrent <= m_frameActionEnd);
}

void Anim_Base::SetLooping(const bool & loop)
{
	m_loop = loop;
}

void Anim_Base::SetName(const std::string & name)
{
	m_name = name;
}

void Anim_Base::SetPart(const std::string & part)
{
	m_part = part;
}

void Anim_Base::Play() { m_playing = true; }

void Anim_Base::Pause() { m_playing = false; }

void Anim_Base::Stop(){ m_playing = false; Reset(); }

void Anim_Base::Reset()
{
	m_frameCurrent = m_frameStart;
	m_elapsedTime = 0.0f;
	CropSprite();
}

void Anim_Base::Update(const float & dt)
{
	if (!m_playing)
		return;
	m_elapsedTime += dt;
	if (m_elapsedTime < m_frameTime)
		return;
	FrameStep();
	CropSprite();
	m_elapsedTime = 0;
}
