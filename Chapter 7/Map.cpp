#include "Map.h"

Map::Map(SharedContext * context, BaseState * currentState)
	: m_context(context), m_defaultTile(context),
	  m_maxMapSize(32, 32), m_tileCount(0), m_tileSetCount(0),
	  m_mapGravity(512.f), m_loadNextMap(false),
	  m_currentState(currentState)
{
	m_context->m_gameMap = this;
	LoadTiles("tiles.cfg");
}

Map::~Map()
{
	PurgeMap();
	PurgeTileSet();
	m_context->m_gameMap = nullptr;
}

Tile * Map::GetTile(unsigned int x, unsigned int y)
{
	auto itr = m_tileMap.find(ConvertCoords(x, y));
	return(itr != m_tileMap.end() ? itr->second : nullptr);
}

TileInfo * Map::GetDefaultTile()
{

}

float Map::GetGravity() const
{

}

unsigned int Map::GetTileSize() const
{

}

const sf::Vector2u & Map::GetMapSize() const
{

}

const sf::Vector2f & Map::GetPlayerStart() const
{

}

void Map::LoadMap(const std::string & path)
{

}

void Map::LoadNext()
{

}

void Map::Update(float dt)
{
	if (m_loadNextMap)
	{
		PurgeMap();
		m_loadNextMap = false;
		if (m_nextMap != "")
			LoadMap("media/maps/" + m_nextMap);
		else
		{
			m_currentState->GetStateManager()->
				SwitchTo(StateType::GameOver);
		}
		m_nextMap = "";
	}
	sf::FloatRect viewSpace = m_context->m_wind->GetViewSpace();
	m_background.setPosition(viewSpace.left, viewSpace.top);
}

void Map::Draw()
{
	sf::RenderWindow * wind = m_context->m_wind->GetRenderWindow();
	wind->draw(m_background);
	sf::FloatRect viewSpace = m_context->m_wind->GetViewSpace();

	//culling
	sf::Vector2i tileBegin(
		floor(viewSpace.left / Sheet::Tile_Size),
		floor(viewSpace.top / Sheet::Tile_Size));
	sf::Vector2i tileEnd(
		ceil((viewSpace.left + viewSpace.width) / Sheet::Tile_Size),
		ceil((viewSpace.top + viewSpace.height) / Sheet::Tile_Size));

	unsigned int count = 0;
	for (int x = tileBegin.x; x <= tileEnd.x; ++x)
	{
		for (int y = tileBegin.y; y < tileEnd.y; ++y)
		{
			if (x < 0 || y < 0)
				continue;

			Tile * tile = GetTile(x, y);
			if (!tile)
				continue;

			sf::Sprite & sprite = tile->m_properties->m_sprite;
			sprite.setPosition(x * Sheet::Tile_Size,
				y * Sheet::Tile_Size);
			wind->draw(sprite);
			++count;
		}
	}
}

//Private methods
unsigned int Map::ConvertCoords(unsigned int x, unsigned int y)
{
	return (x * m_maxMapSize.x) + y;
}

void Map::LoadTiles(const std::string & path)
{

}

void Map::PurgeMap()
{

}

void Map::PurgeTileSet()
{

}
