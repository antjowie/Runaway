#include "Tile.h"
#include "TileHeader.h"
#include "DataManager.h"
#include <cassert>

Tile::Tile(const TileType type, const float x, const float y, const std::string tilesetName, const bool isSolid):
	m_TileType(type), m_solid(isSolid)
{
	m_sprite.setTexture(DataManager::getInstance().getTexture(tilesetName));
	m_sprite.setPosition(x, y);
}

void Tile::update()
{
}

void Tile::draw(sf::RenderWindow &window)
{
	window.draw(m_sprite);
}

const sf::FloatRect Tile::getHitbox() const
{
	return m_sprite.getGlobalBounds();
}

const bool Tile::isSolid() const
{
	return m_solid;
}

const TileType Tile::getType() const
{
	return m_TileType;
}

Tile * const getTile(const int id, const float x, const float y, const std::string tilesetName)
{
	switch (static_cast<TileType>(id))
	{
	case TileType::Air:
		return new AirTile(x, y,tilesetName);
	case TileType::Block:
		return new GroundTile(x, y, tilesetName);
	case TileType::Top:
		return new GroundTopTile(x, y, tilesetName);
	case TileType::Light:
		return new LightTile(x, y, tilesetName);
	case TileType::Gate:
		return new GateTile(x, y, tilesetName);
	default:
		return new InvalidTile(x, y, tilesetName);
	}
}