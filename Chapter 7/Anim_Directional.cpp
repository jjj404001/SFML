#include "Anim_Directional.h"
#include "SpriteSheet.h"

void Anim_Directional::FrameStep()
{
	//std::cout << m_part << ", " << m_animName << ", " << m_frameCurrent << std::endl;

	if (m_frameStart < m_frameEnd)
		++m_frameCurrent;
	else
		--m_frameCurrent;

	if ((m_frameStart < m_frameEnd && m_frameCurrent > m_frameEnd) ||
		(m_frameStart > m_frameEnd && m_frameCurrent < m_frameEnd))
	{
		if (m_loop)
		{
			m_frameCurrent = m_frameStart;
			return;
		}
		m_frameCurrent = m_frameEnd;
		Pause();
	}
}

void Anim_Directional::CropSprite()
{
	//std::cout << "Cropping Sprite: " << m_part << ", " << m_name << std::endl;
	sf::IntRect rect(
		m_spriteSheet->GetSpriteSize().x * m_frameCurrent,
		m_spriteSheet->GetSpriteSize().y * 
		(m_frameRow + (short)m_spriteSheet->GetDirection()),
		m_spriteSheet->GetSpriteSize().x,
		m_spriteSheet->GetSpriteSize().y);
	m_spriteSheet->CropSprite(m_part, rect);
	//std::cout << "x: " << rect.left << ", " << "y: " << rect.top << std::endl;
}

void Anim_Directional::ReadIn(std::stringstream & stream)
{
	stream >> m_frameStart >> m_frameEnd >> m_frameRow
		>> m_frameTime >> m_frameActionStart >> m_frameActionEnd;
}
