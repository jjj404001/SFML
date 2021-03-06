#include "Map.h"
#include "C_Position.h"

Map::Map(SharedContext * context, BaseState * currentState)
	: m_context(context), m_defaultTile(context),
	  m_maxMapSize(32, 32), m_tileCount(0), m_tileSetCount(0),
	  m_mapGravity(512.f), m_loadNextMap(false),
	  m_currentState(currentState), m_playerId(-1)
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

Tile * Map::GetTile(unsigned int x, unsigned int y, unsigned int layer)
{
	if (x < 0 || y < 0 || x >= m_maxMapSize.x ||
		y >= m_maxMapSize.y || layer < 0 ||
		layer >= Sheet::Num_Layers)
	{
		return nullptr;
	}
	auto itr = m_tileMap.find(ConvertCoords(x, y, layer));
	return(itr != m_tileMap.end() ? itr->second : nullptr);
}

TileInfo * Map::GetDefaultTile()
{
	return &m_defaultTile;
}

float Map::GetGravity() const
{
	return m_mapGravity;
}

unsigned int Map::GetTileSize() const
{
	return Sheet::Tile_Size;
}

const sf::Vector2u & Map::GetMapSize() const
{
	return m_maxMapSize;
}

const sf::Vector2f & Map::GetPlayerStart() const
{
	return m_playerStart;
}

void Map::LoadMap(const std::string & path)
{
	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() + path);
	if (!file.is_open())
	{
		std::cout << "! Cannot open map file: "
			<< path << std::endl;
		return;
	}

	std::string line;
	while (getline(file, line))
	{
		if (line[0] == '|')
			continue;
		std::stringstream keystream(line);
		std::string type;
		keystream >> type;
		if (type == "TILE")
		{
			int tileId = 0;
			keystream >> tileId;
			if (tileId < 0)
			{
				std::cout << "! Bad tile id: "
					<< tileId << std::endl;
				continue;
			}
			auto itr = m_tileSet.find(tileId);
			if (itr == m_tileSet.end())
			{
				std::cout << "! Tile id(" << tileId
					<< ") was not found in tileset.\n";
				continue;
			}

			sf::Vector2i tileCoords;
			unsigned int tileLayer = 0;
			unsigned int tileSolidity = 0;
			keystream >> tileCoords.x >> tileCoords.y
				>> tileLayer >> tileSolidity;
			if (tileCoords.x > m_maxMapSize.x || tileCoords.y > m_maxMapSize.y ||
				tileLayer >= Sheet::Num_Layers)
			{
				std::cout << "! Tile is out of range: " <<
					tileCoords.x << " " << tileCoords.y << std::endl;
				continue;
			}

			Tile * tile = new Tile();
			//Bind properties of a tile from a set
			tile->m_properties = itr->second;
			tile->m_soild = (bool)tileSolidity;
			if (!m_tileMap.emplace(ConvertCoords(
				tileCoords.x, tileCoords.y, tileLayer), tile).second)
			{
				//Duplicate tile detected!
				std::cout << "! Duplicate tile! : " << tileCoords.x
					<< "" << tileCoords.y << std::endl;
				delete tile;
				tile = nullptr;
				continue;
			}
			std::string warp;
			keystream >> warp;
			tile->m_warp = false;
			if (warp == "WARP")
				tile->m_warp = true;
		}
		else if (type == "BACKGROUND")
		{
			if (m_backgroundTexture != "")
				continue;
			keystream >> m_backgroundTexture;
			if (!m_context->m_textureManager->
				RequireResource(m_backgroundTexture))
			{
				m_backgroundTexture = "";
				continue;
			}
			sf::Texture * texture = m_context->m_textureManager->
				GetResource(m_backgroundTexture);
			m_background.setTexture(*texture);
			sf::Vector2f viewSize = m_currentState->GetView().getSize();
			sf::Vector2u textureSize = texture->getSize();
			sf::Vector2f scaleFactors;
			scaleFactors.x = viewSize.x / textureSize.x;
			scaleFactors.y = viewSize.y / textureSize.y;
			m_background.setScale(scaleFactors);
		}
		else if (type == "SIZE")
			keystream >> m_maxMapSize.x >> m_maxMapSize.y;
		else if (type == "GRAVITY")
			keystream >> m_mapGravity;
		else if (type == "DEFAULT_FRICTION")
			keystream >> m_defaultTile.m_friction.x
			>> m_defaultTile.m_friction.y;
		else if (type == "NEXTMAP")
			keystream >> m_nextMap;
		else if (type == "ENTITY")
		{
			std::string name;
			keystream >> name;
			if (name == "Player" && m_playerId != -1)
				continue;
			int entityId = m_context->m_entityManager->AddEntity(name);
			if (entityId < 0)
				continue;
			if (name == "Player")
				m_playerId = entityId;
			C_Base * position = m_context->m_entityManager->
				GetComponent<C_Position>(entityId, Component::Position);
			if (position)
				keystream >> *position;
		}
	}
}

void Map::LoadNext()
{
	m_loadNextMap = true;
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

void Map::Draw(unsigned int layer)
{
	if (layer >= Sheet::Num_Layers)
		return;
	sf::RenderWindow * wind = m_context->m_wind->GetRenderWindow();
	sf::FloatRect viewSpace = m_context->m_wind->GetViewSpace();

	sf::Vector2i tileBegin(
		floor(viewSpace.left / Sheet::Tile_Size),
		floor(viewSpace.top / Sheet::Tile_Size));
	sf::Vector2i tileEnd(
		ceil(viewSpace.left + viewSpace.width) / Sheet::Tile_Size,
		ceil(viewSpace.top + viewSpace.height) / Sheet::Tile_Size);

	unsigned int count = 0;
	for (int x = tileBegin.x; x <= tileEnd.x; ++x)
	{
		for (int y = tileBegin.y; y <= tileEnd.y; ++y)
		{
			Tile * tile = GetTile(x, y, layer);
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
unsigned int Map::ConvertCoords(unsigned int x, unsigned int y,
	unsigned int layer) const
{
	return ((layer * m_maxMapSize.y + y) * m_maxMapSize.x + x);
}

void Map::LoadTiles(const std::string & path)
{
	std::ifstream file;

	std::string p(Utils::GetWorkingDirectory() + path);

	file.open(Utils::GetWorkingDirectory() + path);
	if (!file.is_open())
	{
		std::cout << "! Failed loading tile set file: "
			<< path << std::endl;
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		if (line[0] == '|')
			continue;
		std::stringstream keystream(line);
		int tileId;
		keystream >> tileId;
		if (tileId < 0)
			continue;

		TileInfo * tile = new TileInfo(m_context, "TileSheet", tileId);
		keystream >> tile->m_name >> tile->m_friction.x
			>> tile->m_friction.y >> tile->m_deadly;
		if (!m_tileSet.emplace(tileId, tile).second)
		{
			//Duplicate tile detected!
			std::cout << "! Duplicate tile type: "
				<< tile->m_name << std::endl;
			delete tile;
		}
	}
	file.close();
}

void Map::PurgeMap()
{
	m_tileCount = 0;
	for (auto & itr : m_tileMap)
		delete itr.second;
	m_tileMap.clear();
	//m_context->m_entityManager->Purge();

	if (m_backgroundTexture == "")
		return;
	m_context->m_textureManager->
		ReleaseResource(m_backgroundTexture);
	m_backgroundTexture = "";
}

void Map::PurgeTileSet()
{
	for (auto & itr : m_tileSet)
		delete itr.second;
	m_tileSet.clear();
	m_tileSetCount = 0;
}
