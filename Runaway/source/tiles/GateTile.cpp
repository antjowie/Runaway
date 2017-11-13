#include "GateTile.h"
#include "DataManager.h"
#include <iostream>

void GateTile::fixTextureRect()
{
	if (m_sprite.getTextureRect().height == 0)
		m_sprite.setTextureRect(sf::IntRect(0, 0, m_sprite.getGlobalBounds().width, 1));
}

void GateTile::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (getTileMeta().m_solid) target.draw(m_sprite, states);
}

GateTile::GateTile(const float x, const float y):
	Tile(TileType::Gate, x,y,2.0f,true)
{
	m_sprite.setTexture(DataManager::getInstance().getTexture("gate"));
	m_sprite.setTextureRect(sf::IntRect(0,0,32,32));
}

void GateTile::setRect(const sf::FloatRect newRect)
{
	if (part == Part::Top)
		m_sprite.rotate(180);
	m_sprite.setPosition(newRect.left + newRect.width/2, newRect.top);
	setSize(sf::Vector2f(newRect.width, newRect.height));
}

void GateTile::setSize(const sf::Vector2f newSize)
{
	fixTextureRect();
	sf::Vector2f scale{ newSize.x / m_sprite.getGlobalBounds().width, newSize.y / m_sprite.getGlobalBounds().height };
	sf::Vector2f size{ m_sprite.getGlobalBounds().width * scale.x, m_sprite.getGlobalBounds().height * scale.y };

	sf::IntRect newTextureRect{ m_sprite.getTextureRect() };
	// Happens if 0/0
	if (size.y != size.y)
		size.y = 0;

	newTextureRect.height = size.y;		
	m_sprite.setTextureRect(newTextureRect);
	
	m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2, m_sprite.getLocalBounds().height);
}

void GateTile::setTextureType(const State state)
{
	sf::IntRect temp{ m_sprite.getTextureRect() };
	temp.left = state*temp.width;
	m_sprite.setTextureRect(temp);
}
