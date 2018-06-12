#include "SpriteSheet.h"
#include "Anim_Base.h"
#include "Anim_Directional.h"

SpriteSheet::SpriteSheet(TextureManager * textMgr)
	:	m_textureManager(textMgr),
		m_spriteScale(1.f, 1.f), m_direction(Direction::Right) { }

SpriteSheet::~SpriteSheet()	{ ReleaseSheet(); }

void SpriteSheet::CropSprite(
	const std::string & part, const sf::IntRect & rect)
{
	auto itr = m_charAnim.find(part);
	if (itr == m_charAnim.end())
		return;

	itr->second.first.setTextureRect(rect);
}

//... Basic setters & getters
void SpriteSheet::SetSpriteSize(
	sf::Sprite * sprite, const sf::Vector2i & size)
{
	m_spriteSize = size;
	sprite->setOrigin(m_spriteSize.x / 2, m_spriteSize.y / 2);
}

void SpriteSheet::SetSpritePosition(
	const sf::Vector2f & pos)
{
	auto itr = m_charAnim.find("Body");
	if (itr == m_charAnim.end())
		return;

	itr->second.first.setPosition(pos);
}

void SpriteSheet::SetDirection(const Direction & dir)
{
	if (dir == m_direction) return;
	m_direction = dir;
	for(auto & itr : m_currentAnims)
		itr.second->CropSprite();
}

bool SpriteSheet::LoadSheet(const std::string & file)
{
	std::ifstream sheet;
	sheet.open(Utils::GetWorkingDirectory() + file);
	if (sheet.is_open())
	{
		ReleaseSheet();
		std::string line;
		while (std::getline(sheet, line))
		{
			if (line[0] == '|')
				continue;
			std::stringstream keystream(line);

			std::string type;
			keystream >> type;
			
			std::string part;

			if (type == "Texture")
			{
				if (m_texture != "")
				{
					std::cerr << "! Duplicate texture entries in: "
						<< file << std::endl;
					continue;
				}
				std::string texture;
				keystream >> texture;
				if (!m_textureManager->RequireResource(texture))
				{
					std::cerr << "! Could not set up the texture: "
						<< texture << std::endl;
					continue;
				}
				m_texture = texture;
				//sf::Sprite sprite;
				//sprite.setTexture(*m_textureManager->GetResource(m_texture));
			}
			else if (type == "Size")
			{
				keystream >> m_spriteSize.x >> m_spriteSize.y;
				//SetSpriteSize(m_spriteSize);
			}
			else if (type == "Scale")
			{
				keystream >> m_spriteScale.x >> m_spriteScale.y;
				//m_sprite.setScale(m_spriteScale);
			}
			else if (type == "AnimationType")
			{
				keystream >> m_animType;
			}
			else if (type == "Part")
			{
				keystream >> part;
				sf::Sprite tempSprite;
				Animations tempAnim;
				m_charAnim.emplace(part, std::make_pair(tempSprite, tempAnim));
				m_currentAnims.emplace(part, nullptr);
			}
			else if (type == "Animation")
			{
				std::string name;
				keystream >> name;

				auto itr = m_charAnim.find(part);
				if (itr != m_charAnim.end())
				{
					if (itr->second.second.find(name) != itr->second.second.end())
					{
						std::cerr << "! Duplicate animation(" << name
							<< ") in: " << file << std::endl;
						continue;
					}
				}

				Anim_Base * anim = nullptr;
				if (m_animType == "Directional")
					anim = new Anim_Directional();
				else
				{
					std::cerr << "! Unknown animation type: "
						<< m_animType << std::endl;
					continue;
				}

				keystream >> *anim;
				anim->SetSpriteSheet(this);
				anim->SetName(name);
				anim->Reset();
				itr->second.second.emplace(name, anim);

				sf::Sprite * temp = &itr->second.first;
				temp->setTexture(
					*m_textureManager->GetResource(m_texture));
				SetSpriteSize(temp, m_spriteSize);
				temp->setScale(m_spriteScale);


				if (m_currentAnims.find(part) == m_currentAnims.end())
					continue;
				if (m_currentAnims.find(part)->second)
					continue;

				m_currentAnims.find(part)->second = anim;
				m_currentAnims.find(part)->second->Play();
			}
		}
		sheet.close();
		return true;
	}
	std::cerr << "! Failed loading spritesheet: "
		<< file << std::endl;
	return false;
}

void SpriteSheet::ReleaseSheet()
{
	m_textureManager->ReleaseResource(m_texture);

	for(auto & itr : m_currentAnims)
		itr.second = nullptr;

	for (auto & itr : m_charAnim)
	{
		Animations & temp = itr.second.second;
		while (temp.begin() != temp.end())
		{
			delete temp.begin()->second;
			temp.erase(temp.begin());
		}
	}
	m_charAnim.clear();

}

Anim_Base * SpriteSheet::GetCurrentAnim()
{
	auto itr = m_currentAnims.find("Body");
	return (itr == m_currentAnims.end() ? itr->second : nullptr);
}

bool SpriteSheet::SetAnimation(const std::string & part,
	const std::string & name,
	const bool & play, const bool & loop)
{
	auto body = m_charAnim.find("Body");
	if (body == m_charAnim.end())
		return false;

	Animations & temp = body->second.second;

	auto itr = temp.find(name);
	if (itr == temp.end()) return false;

	auto currentAnim = m_currentAnims.find("Body");
	if (currentAnim == m_currentAnims.end())
		return false;

	Anim_Base * tempAnim = currentAnim->second;

	if (itr->second == tempAnim) return false;
	if (tempAnim)
		tempAnim->Stop();
	tempAnim = itr->second;
	tempAnim->SetLooping(loop);
	if (play)
		tempAnim->Play();
	tempAnim->CropSprite();
	return true;
}

void SpriteSheet::Update(const float & dt)
{
	for(auto & itr : m_currentAnims)
		itr.second->Update(dt);
}

void SpriteSheet::Draw(sf::RenderWindow * wd)
{
	for (auto & itr : m_charAnim)
		wd->draw(itr.second.first);
}
