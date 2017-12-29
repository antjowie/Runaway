#include "Elevator.h"
#include "DataManager.h"
#include "Config.h"

Elevator::Elevator(const int id, const float height, const float speed, const bool inverted):
	m_id(id),m_height(height),m_speed(speed), m_inverted(inverted)
{
}

void Elevator::initSound(SoundManager & soundManager)
{
	m_movingSound = new SoundObject(SoundType::Effect, soundManager);
	m_movingSound->setBuffer(DataManager::getInstance().getSound("doorMoving"));
	m_movingSound->setLoop(true);
	m_movingSound->setPosition(m_bottomElevatorTile->getHitbox().left + m_bottomElevatorTile->getHitbox().width / 2, 0.f, m_bottomElevatorTile->getHitbox().top);
	m_movingSound->setMinDistance(4.f);
	m_movingSound->setAttenuation(0.3f);
	m_movingSound->setVolume(0);

	m_hitSound = new SoundObject(SoundType::Effect, soundManager);
	m_hitSound->setBuffer(DataManager::getInstance().getSound("doorHit"));
	m_hitSound->setLoop(false);
	m_hitSound->setPosition(m_bottomElevatorTile->getHitbox().left + m_bottomElevatorTile->getHitbox().width / 2, 0.f, m_bottomElevatorTile->getHitbox().top);
	m_hitSound->setMinDistance(6.f);
	m_hitSound->setAttenuation(0.3f);
	m_hitSound->setVolume(0); 
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

	if (m_inverted)
		m_timeline = m_speed;
}

void Elevator::update(const float elapsedTime)
{
	if (m_inverted)
		m_isOpen = !m_isOpen;
	m_isOpen ? m_timeline += elapsedTime : m_timeline -= elapsedTime;
	m_isOpen ? m_bottomElevatorTile->setTextureType(ElevatorTile::State::Open) : m_bottomElevatorTile->setTextureType(ElevatorTile::State::Closed);

	m_movingSound->setPosition(m_bottomElevatorTile->getHitbox().left + m_bottomElevatorTile->getHitbox().width / 2, 0.f, m_bottomElevatorTile->getHitbox().top);
	m_hitSound->setPosition(m_bottomElevatorTile->getHitbox().left + m_bottomElevatorTile->getHitbox().width / 2, 0.f, m_bottomElevatorTile->getHitbox().top);
	// Sound related
	if (m_timeline > m_speed)
	{
		m_moving = false;
		m_timeline = m_speed;
	}
	else if (m_timeline < 0)
	{
		m_moving = false;
		m_timeline = 0;
	}
	else
	{
		m_moving = true;
		m_hasHit = false;
	}

	if (!m_moving)
	{
		m_soundTimeline -= elapsedTime * 5 * Config::getInstance().getConfig("effects").integer;
		if (m_soundTimeline < 0)
			m_soundTimeline = 0;
		m_movingSound->setVolume(m_soundTimeline);
		if (!m_hasHit)
		{
			m_hitSound->setVolume(Config::getInstance().getConfig("effects").integer);
			m_hitSound->play();
			m_hasHit = true;
		}
	}
	else
	{
		m_soundTimeline = Config::getInstance().getConfig("effects").integer;
		m_movingSound->setVolume(m_soundTimeline);
	}

	if (m_topElevatorTile != m_bottomElevatorTile)
	{
		m_topElevatorTile->setSolid(false);
		m_topElevatorTile->setSize(sf::Vector2f(0, 0));
	}

	if (m_timeline > m_speed)
		m_timeline = m_speed;
	else if (m_timeline < 0)
		m_timeline = 0;

	const float percentage(m_timeline / m_speed);
	float newHeight{ (1.f - percentage)*m_originalHeight + percentage * (m_originalHeight+ m_height* 32.f)};
	m_bottomElevatorTile->setSize(sf::Vector2f(m_bottomElevatorTile->getHitbox().width, newHeight));

	if (m_inverted)
		m_isOpen = !m_isOpen;
}

const int Elevator::getId() const
{
	return m_id;
}
