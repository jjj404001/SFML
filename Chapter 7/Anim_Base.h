#ifndef ANIM_BASE_H
#define ANIM_BASE_H

#include <iostream>

class SpriteSheet;
using Frame = unsigned int;

class Anim_Base
{
	friend class SpriteSheet;

public:
	Anim_Base();
	virtual ~Anim_Base() {}

	//...Setters & getters
	void SetSpriteSheet(SpriteSheet * sheet);
	void SetFrame(const unsigned int & frame);
	bool IsInAction();
	void SetLooping(const bool & loop);
	void SetName(const std::string & name);
	void SetPart(const std::string & part);
	const std::string & GetName();
	bool IsPlaying() { return m_playing; }

	void Play();
	void Pause();
	void Stop();
	void Reset();

	virtual void Update(const float & dt);

	friend std::stringstream & operator>>(
		std::stringstream & stream, Anim_Base & a)
	{
		a.ReadIn(stream);
		return stream;
	}

protected:
	virtual void FrameStep() = 0;
	virtual void CropSprite() = 0;
	virtual void ReadIn(std::stringstream & stream) = 0;

	Frame m_frameCurrent;
	Frame m_frameStart;
	Frame m_frameEnd;
	Frame m_frameRow;
	int m_frameActionStart;
	int m_frameActionEnd;
	float m_frameTime;
	float m_elapsedTime;
	bool m_loop;
	bool m_playing;
	std::string m_animName;
	std::string m_part;
	SpriteSheet * m_spriteSheet;
};

#endif