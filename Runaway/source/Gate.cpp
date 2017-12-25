#include "Gate.h"
#include "DataManager.h"
#include "Config.h"
#include <SFML\Graphics\RenderTarget.hpp>

Gate::Gate(const int id, const float speed, const bool inverted):
	m_id(id),m_speed(speed),m_inverted(inverted)
{
}

void Gate::initSound(SoundManager & soundManager)
{
	// Init sound
	m_movingSound = new SoundObject(SoundType::Effect, soundManager);
	m_movingSound->setBuffer(DataManager::getInstance().getSound("doorMoving"));
	m_movingSound->setLoop(true);
	m_movingSound->setPosition(m_gate.getBottomTile()->getHitbox().left + m_gate.getBottomTile()->getHitbox().width / 2, 0.f, m_gate.getBottomTile()->getHitbox().top);
	m_movingSound->setMinDistance(4.f);
	m_movingSound->setAttenuation(0.3f);
	m_movingSound->setVolume(0);

	m_hitSound = new SoundObject(SoundType::Effect, soundManager);
	m_hitSound->setBuffer(DataManager::getInstance().getSound("doorHit"));
	m_hitSound->setLoop(false);
	m_hitSound->setPosition(m_gate.getBottomTile()->getHitbox().left + m_gate.getBottomTile()->getHitbox().width / 2, 0.f, m_gate.getBottomTile()->getHitbox().top);
	m_hitSound->setMinDistance(6.f);
	m_hitSound->setAttenuation(0.3f);
	m_hitSound->setVolume(0);
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
	if(m_inverted)
	m_timeline = m_speed;
}

void GatePart::update(const float percentage)
{
	// If percentage = 0 gates are closed
	// If percentage = 1 gates are open
	
	sf::Vector2f newSize{m_top->getHitbox().width,m_originalHeight- m_originalHeight * percentage};

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
	if (m_inverted)
		m_isOpen = !m_isOpen;

	m_isOpen ? m_timeline += elapsedTime : m_timeline -= elapsedTime;
	m_isOpen ? m_gate.setState(GateTile::State::Open) : m_gate.setState(GateTile::State::Closed);

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

	const float percentage{ m_timeline / m_speed };
	m_gate.update(percentage);

	if (m_inverted)
		m_isOpen = !m_isOpen;
}