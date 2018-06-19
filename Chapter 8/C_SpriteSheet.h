#ifndef C_SPRITESHEET_H
#define C_SPRITESHEET_H

#include "C_Drawable.h"
#include "SpriteSheet.h"

class C_SpriteSheet : public C_Drawable
{
public:
	C_SpriteSheet() : C_Drawable(Component::SpriteSheet),
		m_spriteSheet(nullptr) {}

	~C_SpriteSheet()
	{
		if (m_spriteSheet) { delete m_spriteSheet; }
	}

	//C_Base method
	void ReadIn(std::stringstream & stream)
	{
		stream >> m_sheetName;
	}

	//C_Drawable methods
	SpriteSheet * GetSpriteSheet() { return m_spriteSheet; }

	void UpdatePosition(const sf::Vector2f & vec)
	{
		m_spriteSheet->SetSpritePosition(vec);
	}

	const sf::Vector2i & GetSize()
	{
		return m_spriteSheet->GetSpriteSize();
	}

	void Draw(sf::RenderWindow * wind)
	{
		if (!m_spriteSheet)
			return;
		m_spriteSheet->Draw(wind);
	}

	//C_SpriteSheet method
	void Create(TextureManager * textureMgr,
		const std::string & name = "")
	{
		if (m_spriteSheet)
			return;
		m_spriteSheet = new SpriteSheet(textureMgr);
		m_spriteSheet->LoadSheet("media/Spritesheets/" +
			(name != "" ? name : m_sheetName) + ".sheet");
	}

private:
	SpriteSheet * m_spriteSheet;
	std::string m_sheetName;
};

#endif