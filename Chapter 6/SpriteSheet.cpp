#include "SpriteSheet.h"
#include "Anim_Base.h"

SpriteSheet::SpriteSheet(TextureManager * textMgr)
	:	m_textureManager(textMgr), m_animationCurrent(nullptr),
		m_spriteScale(1.f, 1.f), m_direction(Direction::Right) { }

SpriteSheet::~SpriteSheet()	{ ReleaseSheet(); }

void SpriteSheet::CropSprite(const sf::IntRect & rect)
{
	m_sprite.setTextureRect(rect);
}

//... Basic setters & getters
void SpriteSheet::SetSpriteSize(const sf::Vector2i & size)
{
	m_spriteSize = size;
	m_sprite.setOrigin(m_spriteSize.x / 2, m_spriteSize.y / 2);
}

void SpriteSheet::SetSpritePosition(const sf::Vector2f & pos)
{
	m_sprite.setPosition(pos);
}

void SpriteSheet::SetDirection(const Direction & dir)
{
	if (dir == m_direction) return;
	m_direction = dir;
	m_animationCurrent->CropSprite();
}

bool SpriteSheet::LoadSheet(const std::string & file)
{

}

void SpriteSheet::ReleaseSheet()
{
	m_textureManager->ReleaseResource(m_texture);
	m_animationCurrent = nullptr;
	while (m_animations.begin() != m_animations.end())
	{
		delete m_animations.begin()->second;
		m_animations.erase(m_animations.begin());
	}
}

Anim_Base * SpriteSheet::GetCurrentAnim()
{
	return m_animationCurrent;
}

bool SpriteSheet::SetAnimation(const std::string & name,
	const bool & play = false,
	const bool & loop = false)
{
	auto itr = m_animations.find(name);
	if (itr == m_animations.end()) return false;
	if (itr->second == m_animationCurrent) return false;
	if (m_animationCurrent)
		m_animationCurrent->Stop();
	m_animationCurrent = itr->second;
	m_animationCurrent->SetLooping(loop);
	if (play)
		m_animationCurrent->Play();
	m_animationCurrent->CropSprite();
	return true;
}

void SpriteSheet::Update(const float & dt)
{
	m_animationCurrent->Update(dt);
}

void SpriteSheet::Draw(sf::RenderWindow * wd)
{
	wd->draw(m_sprite);
}
