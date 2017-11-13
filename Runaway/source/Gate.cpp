#include "Gate.h"
#include <SFML\Graphics\RenderTarget.hpp>
#include <iostream>

Gate::Gate(const int id, const float speed):
	m_id(id),m_speed(speed)
{
}

void Gate::loadTopTile(GateTile* const topGateTile)
{
	m_gate.loadTopPart(topGateTile);
}

void Gate::loadBottomTile(GateTile* const bottomGateTile)
{
	m_gate.loadBottomPart(bottomGateTile);
}

void Gate::loadGate()
{
	m_gate.loadGate();
}

void GatePart::update(const float percentage)
{
	// If percentage = 0 gates are closed
	// If percentage = 1 gates are open
	
	sf::Vector2f newSize{m_top->getHitbox().width,m_originalHeight- m_originalHeight * percentage};
	if (m_top->getHitbox().height == 0);
	m_top->setSize(newSize);
	m_bottom->setSize(newSize);
}

void GatePart::loadTopPart(GateTile * const topGateTile)
{
	m_top = topGateTile;
	m_top->part = m_top->Top;
}

void GatePart::loadBottomPart(GateTile * const bottomGatePart)
{
	m_bottom = bottomGatePart;
	m_bottom->part = m_bottom->Bottom;
}

void GatePart::loadGate()
{
	sf::Vector2f size{ m_top->getHitbox().width, m_top->getHitbox().height};
	// Since the bottom tile has a higher y coordinate then upper tile.
	float middle{ (m_bottom->getHitbox().top + m_bottom->getHitbox().height- m_top->getHitbox().top)/ size.y /2.f};
	m_originalHeight = m_top->getHitbox().height * middle;
	
	sf::FloatRect top(m_top->getHitbox().left, m_top->getHitbox().top, m_top->getHitbox().width, m_originalHeight);
	sf::FloatRect bottom(m_bottom->getHitbox().left, m_bottom->getHitbox().top + m_bottom->getHitbox().height, m_bottom->getHitbox().width, m_originalHeight);
	
	m_top->setRect(top);
	m_bottom->setRect(bottom);
}

const GateTile * const GatePart::getTopTile() const
{
	return m_top;
}

const GateTile * const GatePart::getBottomTile() const
{
	return m_bottom;
}

void GatePart::setState(const GateTile::State state)
{
	m_top->setTextureType(state);
	m_bottom->setTextureType(state);
}

const int Gate::getId()
{
	return m_id;
}

const GateTile * const Gate::getTopTile() const
{
	return m_gate.getTopTile();
}

const GateTile * const Gate::getBottomTile() const
{
	return m_gate.getBottomTile();
}

void Gate::update(const float elapsedTime)
{
	m_isOpen ? m_timeline += elapsedTime : m_timeline -= elapsedTime;
	m_isOpen ? m_gate.setState(GateTile::State::Open) : m_gate.setState(GateTile::State::Closed);
	if (m_timeline > m_speed)
		m_timeline = m_speed;
	else if (m_timeline < 0)
		m_timeline = 0;

	const float percentage{ m_timeline / m_speed };
	m_gate.update(percentage);
}