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
			keystream >> tileCoords.x >> tileCoords.y;
			if (tileCoords.x > m_maxMapSize.x || tileCoords.y > m_maxMapSize.y)
			{
				std::cout << "! Tile is out of range: " <<
					tileCoords.x << " " << tileCoords.y << std::endl;
				continue;
			}

			Tile * tile = new Tile();
			//Bind properties of a tile from a set
			tile->m_properties = itr->second;
			if (!m_tileMap.emplace(ConvertCoords(
				tileCoords.x, tileCoords.y), tile).second)
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
		else if (type == "PLAYER")
		{
			int playerId = -1;
			EntityManager * entityMgr = m_context->m_entityManager;
			if (playerId != -1)
				continue;
			playerId = entityMgr->Add(EntityType::Player);
			if (playerId < 0)
				continue;
			keystream >> m_playerStart.x >> m_playerStart.y;
			entityMgr->Find(playerId)->SetPosition(m_playerStart);
		}
		else if (type == "ENEMY")
		{
			EntityManager * entityMgr = m_context->m_entityManager;
			std::string enemyName;
			keystream >> enemyName;
			int enemyId = entityMgr->Add(EntityType::Enemy, enemyName);
			if (enemyId < 0)
				continue;
			float enemyX = 0;
			float enemyY = 0;
			keystream >> enemyX >> enemyY;
			entityMgr->Find(enemyId)->SetPosition(enemyX, enemyY);
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
