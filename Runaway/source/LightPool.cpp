#include "LightPool.h"
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <iostream>

#include "DataManager.h"

void LightPool::setSize(const float width, const float height)
{
	// The radius value represents the original diameter
	const sf::Vector2f size{ m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height };
	
	m_sprite.scale(width / size.x, height / size.y);
	if (size.x != size.x)
		m_sprite.setScale(0.001f, m_sprite.getScale().y);
	if (size.y != size.y)
		m_sprite.setScale(m_sprite.getScale().x, 0.001f);

	std::cout << size.x << '\n';
}

void LightPool::setSize(const float diameter)
{
	setSize(diameter, diameter);
}

void LightPool::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

void LightPool::update(const float elapsedTime)
{
	m_pool += m_rate * elapsedTime;
	if (m_pool < 0)
		m_pool = 0;
	else if (m_pool > m_cap)
		m_pool = m_cap;

	// Update texture
	setSize(m_pool);
}

void LightPool::setPos(const sf::Vector2f & pos)
{
	m_sprite.setPosition(pos);
}

void LightPool::setRate(const float rate)
{
	m_rate = rate;
}

const bool LightPool::isCapped() const
{
	return m_pool == m_cap;
}

const bool LightPool::isEmpty() const
{
	return m_pool == 0;
}

LightPool::LightPool()
{
	m_sprite.setTexture(DataManager::getInstance().getTexture("lightCircle"));
	m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2, m_sprite.getLocalBounds().height / 2);
	m_cap = 32 * 16 * 2;
	m_pool = 32 * 5 * 2;
	//setSize(32 * 5 * 2);

}