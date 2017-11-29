#include "Elevator.h"

#include <iostream>

Elevator::Elevator(const int id, const float height, const float speed):
	m_id(id),m_height(height),m_speed(speed)
{
}

void Elevator::loadTopTile(ElevatorTile * const topElevatorTile)
{
	m_topElevatorTile = topElevatorTile;
}

void Elevator::loadBottomTile(ElevatorTile * const bottomElevatorTile)
{
	m_bottomElevatorTile = bottomElevatorTile;
}

void Elevator::loadElevator()
{
	if (m_bottomElevatorTile == nullptr && m_topElevatorTile == nullptr)
	{
		m_originalHeight = 0;
		return;
	}

	const float bottom{ m_bottomElevatorTile->getHitbox().top + m_bottomElevatorTile->getHitbox().height };
	const float top{ m_topElevatorTile->getHitbox().top };
	m_originalHeight = bottom - top;

	m_bottomElevatorTile->setSize(sf::Vector2f(m_bottomElevatorTile->getHitbox().width, m_originalHeight));
	m_bottomElevatorTile->setTextureType(ElevatorTile::State::Closed);

	if (m_topElevatorTile != m_bottomElevatorTile)
	{
		m_topElevatorTile->setSolid(false);
		m_topElevatorTile->setSize(sf::Vector2f(0, 0));
	}
}

void Elevator::update(const float elapsedTime)
{
	m_isOpen ? m_timeline += elapsedTime : m_timeline -= elapsedTime;
	m_isOpen ? m_bottomElevatorTile->setTextureType(ElevatorTile::State::Open) : m_bottomElevatorTile->setTextureType(ElevatorTile::State::Closed);
	if (m_timeline > m_speed)
		m_timeline = m_speed;
	else if (m_timeline < 0)
		m_timeline = 0;

	const float percentage(m_timeline / m_speed);
	float newHeight{ (1.f - percentage)*m_originalHeight + percentage * (m_originalHeight+ m_height* 32.f)};
	m_bottomElevatorTile->setSize(sf::Vector2f(m_bottomElevatorTile->getHitbox().width, newHeight));
}

const int Elevator::getId() const
{
	return m_id;
}
