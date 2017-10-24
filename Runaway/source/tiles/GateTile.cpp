#include "GateTile.h"
#include "DataManager.h"

GateTile::GateTile(const float x, const float y):
	Tile(TileType::Gate, x,y,true),m_animHandler(32,32)
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

void GateTile::setTextureRect(const int textureRect)
{
	m_textureRect = textureRect;
}

void GateTile::setSolid(const bool isSolid)
{
	m_solid = isSolid;
	sf::Color transparent{ m_sprite.getColor() };
	transparent.a = m_solid ? 225: 0;
	m_sprite.setColor(transparent);
}

void GateTile::update(const float elapsedTime)
{
	m_animHandler.update(elapsedTime);
	if (!m_isOpen)
		m_animHandler.changeAnimation(0);
	else
		m_animHandler.changeAnimation(1);
}

void GateTile::draw(sf::RenderWindow & window)
{
	sf::IntRect correct{ m_animHandler.getFrame() };
	if (m_isOpen)	// Not sure but apperantly a bug when scaling the game
		correct.top += 1;
	correct.left += correct.width * m_textureRect;
	m_sprite.setTextureRect(correct);

	window.draw(m_sprite);
}
