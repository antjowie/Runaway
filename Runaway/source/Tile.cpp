#include "Tile.h"
#include "DataManager.h"

Tile::Tile(const int id, const float x, const float y):
	m_type(tileType(id))
{
	m_sprite.setPosition(x, y);
	m_sprite.setTexture(DataManager::getInstance().getTexture("tileset"));
	m_sprite.setTextureRect(sf::IntRect(32 * (id - 1), 0, 32, 32));

	switch (id)
	{
	case tileType::Air:
		m_sprite.setColor(sf::Color::Cyan);
		m_solid = false;
		break;
	case tileType::Block:
		break;
	case tileType::Top:
		break;
	case tileType::Light:
		break;
	default:
		m_sprite.setColor(sf::Color::Transparent);
		m_solid = false;
		break;
	}
}

void Tile::draw(sf::RenderWindow &window)
{
	window.draw(m_sprite);
}

const tileType Tile::getType() const
{
	return m_type;
}

const sf::FloatRect Tile::getHitbox() const
{
	return m_sprite.getGlobalBounds();
}

const bool Tile::isSolid() const
{
	return m_solid;
}
