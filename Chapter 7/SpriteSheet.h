#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include <unordered_map>
#include "TextureManager.h"
#include "Direction.h"

class Anim_Base;
using Animations = std::unordered_map<std::string, Anim_Base*>;
using CharacterAnim = std::unordered_map<std::string, Animations>;
	//Part, Animations(animName, Anim_Base*)
using CurrentAnim = std::unordered_map<std::string, 
	std::pair<sf::Sprite, Anim_Base*>>;
	//Part, (sprite, currentAnim)

class SpriteSheet
{
public:
	SpriteSheet(TextureManager * textMgr);
	~SpriteSheet();

	void CropSprite(const std::string & part,
		const sf::IntRect & rect);

	//... Basic setters & getters
	void SetSpriteSize(sf::Sprite * sprite, const sf::Vector2i & size);
	void SetSpritePosition(const sf::Vector2f & pos);
	void SetDirection(const Direction & dir);
	sf::Vector2i GetSpriteSize() { return m_spriteSize; }
	Direction GetDirection() { return m_direction; }

	bool LoadSheet(const std::string & file);
	void ReleaseSheet();

	Anim_Base * GetCurrentAnim();
	bool SetAnimation(const std::string & part, const std::string & name,
		const bool & play = false, const bool & loop = false);

	void Update(const float & dt);
	void Draw(sf::RenderWindow * wd);

private:
	std::string m_texture;
	sf::Vector2i m_spriteSize;
	sf::Vector2f m_spriteScale;
	Direction m_direction;
	std::string m_animType;
	CharacterAnim m_charAnim;
	CurrentAnim m_currentAnims;
	TextureManager * m_textureManager;
};

#endif