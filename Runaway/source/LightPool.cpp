#include "LightPool.h"
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <iostream>

#include "DataManager.h"

void LightPool::setSize(const float width, const float height)
{
	m_sizeTarget.x = width;
	m_sizeTarget.y = height;
	m_resizeEvent = true;
}

void LightPool::setSize(const float diameter)
{
	setSize(diameter, diameter);
}

void LightPool::depleteLight(const float radius)
{
	m_pool = m_pool - radius;
}

void LightPool::changeSize(const float elapsedTime)
{
	// The radius value represents the original diameter
	sf::Vector2f size{ m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height };
	sf::Vector2f deltaSize{ m_sizeTarget.x - size.x, m_sizeTarget.y - size.y };
	deltaSize *= elapsedTime;
	deltaSize.x += size.x;
	deltaSize.y += size.y;
	
	m_sprite.scale(deltaSize.x / size.x, deltaSize.y / size.y);
	if (size.x != size.x || size.x == 0)
		m_sprite.setScale(0.001f, m_sprite.getScale().y);
	if (size.y != size.y || size.y == 0)
		m_sprite.setScale(m_sprite.getScale().x, 0.001f);
}

void LightPool::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

void LightPool::update(const float elapsedTime)
{
	m_shouldAppear ? m_timeline += elapsedTime / m_fadeTime : m_timeline -= elapsedTime / m_fadeTime;
	if (m_timeline > 1.f) m_timeline = 1.f;
	if (m_timeline < 0.f) m_timeline = 0.f;

	sf::Color orig{ m_sprite.getColor() };
	orig.a = m_timeline * 255.f;
	m_sprite.setColor(orig);

	m_pool += m_rate * elapsedTime;
	if (m_pool < 0)
		m_pool = 0;
	else if (m_pool > m_cap)
		m_pool = m_cap;

	// Update texture
	if (!m_resizeEvent)	setSize(m_pool);
	changeSize(elapsedTime);
	m_resizeEvent = false;
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

LightPool::LightPool():
	m_cap(32 * 16 * 2),
	m_pool(32 * 5 * 2),
	m_timeline(0),
	m_fadeTime(3),
	m_shouldAppear(true),
	m_resizeEvent(false)
{
	m_sprite.setTexture(DataManager::getInstance().getTexture("lightCircle"));
	m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2, m_sprite.getLocalBounds().height / 2);
}