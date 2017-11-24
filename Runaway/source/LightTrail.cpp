#include "LightTrail.h"
#include "DataManager.h"
#include "PlayerObject.h"

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <iostream>

void LightTrail::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (const auto &iter : m_trail)
		target.draw(iter.m_sprite, states);
}

LightTrail::LightTrail()
{
}

void LightTrail::update(const float elapsedTime)
{
	for (auto &iter : m_trail)
	{
		iter.m_life -= elapsedTime;
		sf::Color color{ iter.m_sprite.getColor() };
		color.a = 50 * (iter.m_life / m_life);
		iter.m_sprite.setColor(color);
		
	}
	m_trail.remove_if([this](Trail &trail) 
	{
		return trail.m_life < 0; 
	});
}

void LightTrail::pushTrail()
{
	Trail temp;
	temp.m_life = m_life;
	temp.m_sprite.setTexture(DataManager::getInstance().getTexture("playerLight"));
	temp.m_sprite.setTextureRect(m_sprite->getTextureRect());
	temp.m_sprite.setOrigin(temp.m_sprite.getLocalBounds().width / 2, temp.m_sprite.getLocalBounds().height / 2);
	temp.m_sprite.setPosition(m_sprite->getPos());
	m_trail.push_back(temp);
}

void LightTrail::loadSprite(Sprite *sprite)
{
	m_sprite = sprite;
}

void LightTrail::loadLife(const float life)
{
	m_life = life;
}

const LightTrail & LightTrail::getLightTrail() const
{
	return *this;
}
