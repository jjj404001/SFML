#ifndef MAP_H
#define MAP_H

#include "StateManager.h"
#include "TextureManager.h"
#include "EntityManager.h"

enum Sheet{
	Tile_Size = 32, Sheet_Width = 256,
	Sheet_Height = 256, Num_Layers = 4
};

using TileID = unsigned int;

//The flyweight pattern
struct TileInfo
{
	TileInfo(SharedContext * context,
		const std::string & texture = "", TileID id = 0)
		: m_context(context), m_id(0), m_deadly(false)
	{
		TextureManager * tmgr = context->m_textureManager;
		if (texture == "")
		{
			m_id = id;
			return;
		}
		if (!tmgr->RequireResource(texture))
			return;

		m_texture = texture;
		m_id = id;
		m_sprite.setTexture(*tmgr->GetResource(m_texture));
		sf::IntRect tileBoundaries(m_id % 
		(Sheet::Sheet_Width / Sheet::Tile_Size) * Sheet::Tile_Size,
			m_id / (Sheet::Sheet_Height / Sheet::Tile_Size) * Sheet::Tile_Size,
			Sheet::Tile_Size, Sheet::Tile_Size);
		m_sprite.setTextureRect(tileBoundaries);
	}

	~TileInfo()
	{
		if (m_texture == "")
			return;
		m_context->m_textureManager->ReleaseResource(m_texture);
	}

	sf::Sprite m_sprite;

	TileID m_id;
	std::string m_name;
	sf::Vector2f m_friction;
	bool m_deadly;

	SharedContext * m_context;
	std::string m_texture;
};

struct Tile //Incredibly lightweight!
{
	TileInfo * m_properties;
	bool m_warp; // Is the tile  a warp
	// Other flags unique to each tile
	bool m_soild; // Is the tile a solid
};

using TileMap = std::unordered_map<TileID, Tile*>;
using TileSet = std::unordered_map<TileID, TileInfo*>;

class Map
{
public:
	Map(SharedContext * context, BaseState * currentState);
	~Map();
	Tile * GetTile(unsigned int x, unsigned int y, unsigned int layer);
	TileInfo * GetDefaultTile();
	float GetGravity() const;
	unsigned int GetTileSize() const;
	const sf::Vector2u & GetMapSize() const;
	const sf::Vector2f & GetPlayerStart() const;
	void LoadMap(const std::string & path);
	void LoadNext();
	void Update(float dt);
	void Draw(unsigned int layer);
	int GetPlayerId() { return m_playerId; }

private:
	//Method for converting 2D coordinates to 1D ints
	unsigned int ConvertCoords(unsigned int x, unsigned int y,
		unsigned int layer) const;
	void LoadTiles(const std::string & path);
	void PurgeMap();
	void PurgeTileSet();

	TileSet m_tileSet;
	TileMap m_tileMap;
	sf::Sprite m_background;
	TileInfo m_defaultTile;
	sf::Vector2u m_maxMapSize;
	sf::Vector2f m_playerStart;
	unsigned int m_tileCount;
	unsigned int m_tileSetCount;
	int m_playerId;
	float m_mapGravity;
	std::string m_nextMap;
	bool m_loadNextMap;
	std::string m_backgroundTexture;
	BaseState * m_currentState;
	SharedContext * m_context;
};

#endif