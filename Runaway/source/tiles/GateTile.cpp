#include "GateTile.h"
#include "DataManager.h"

void GateTile::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (!getTileMeta().m_solid) return;
	target.draw(m_sprite, states);
}

GateTile::GateTile(const float x, const float y):
	Tile(TileType::Gate, x,y,2.0f,true),m_animHandler(32,32)
{
	m_sprite.setTexture(DataManager::getInstance().getTexture("gate"));
	m_animHandler.addAnimation(Animation(0, 0, 1));
	m_animHandler.addAnimation(Animation(0, 0, 1));
	m_animHandler.changeAnimation(0);

	m_sprite.setTextureRect(m_animHandler.getFrame());
}

void GateTile::move(const float x, const float y)
{
	m_sprite.move(x, y);
}

void GateTile::setTextureType(const unsigned int textureType)
{
	m_textureType = textureType;
}

void GateTile::update(const float elapsedTime)
{
	// Update animation
	m_animHandler.update(elapsedTime);
	if (!m_isOpen)
		m_animHandler.changeAnimation(0);
	else
		m_animHandler.changeAnimation(1);

	// Fix animation texture
	sf::IntRect correct{ m_animHandler.getFrame() };
	if (m_isOpen)	// Not sure but apperantly a bug when scaling the game
		correct.top += 1;
	correct.left += correct.width * m_textureType;
	m_sprite.setTextureRect(correct);
}