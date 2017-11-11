#include "Tile.h"
#include "TileHeader.h"
#include "DataManager.h"
#include <cassert>

void Tile::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

Tile::Tile(const TileType type, const float x, const float y, const float lightRadius, const bool isSolid)
{
	m_meta.m_tileType = type;
	m_meta.m_solid = isSolid;
	m_meta.m_light = lightRadius;

	m_sprite.setTexture(DataManager::getInstance().getTexture("tileset"));
	m_sprite.setPosition(x, y);
}

void Tile::update()
{
}

void Tile::setSolid(const bool isSolid)
{
	m_meta.m_solid = isSolid;
}

void Tile::setBrightness(const sf::Uint8 & brightness)
{
	m_sprite.setColor(sf::Color(brightness, brightness, brightness));
}

const sf::FloatRect Tile::getHitbox() const
{
	return m_sprite.getGlobalBounds();
}

const TileMeta & Tile::getTileMeta() const
{
	return m_meta;
}

Tile * const getTile(const int id, const float x, const float y)
{
	switch (static_cast<TileType>(id))
	{
	case TileType::Air:
		return new AirTile(x, y);
	case TileType::Block:
		return new GroundTile(x, y);
	case TileType::Top:
		return new GroundTopTile(x, y);
	case TileType::Light:
		return new LightTile(x, y);
	case TileType::Gate:
		return new GateTile(x, y);
	default:
		return new InvalidTile(x, y);
	}
}