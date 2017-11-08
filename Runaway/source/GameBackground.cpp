#include "GameBackground.h"
#include "DataManager.h"
#include <iostream>

void GameBackground::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_background);
	target.draw(m_darkBackground);
	target.draw(m_deadBackground);
	target.draw(m_foreground);
}

GameBackground::GameBackground(const std::string &levelPath, const float colorChangeRate):
	m_colorChangeRate(colorChangeRate)
{
	DataManager::getInstance().loadTexture("background", levelPath + "/background.png", true);
	m_foreground.setTexture(DataManager::getInstance().getTexture("background"));

	m_background.setPosition(0, 0);
	m_deadBackground.setPosition(0, 0);
	m_darkBackground.setPosition(0, 0);
	m_darkOverlay.setPosition(0, 0);

	m_background.setFillColor(sf::Color(16, 90, 135));
	m_deadBackground.setFillColor(sf::Color::Red);
	m_darkBackground.setFillColor(sf::Color::Black);
	m_darkOverlay.setFillColor(sf::Color::Black);
}

GameBackground::GameBackground()
{
}

void GameBackground::update(const float elapsedTime)
{
	// Update locations of foreground
	sf::Vector2f movement{ m_target - m_originalPos};
	movement.x /= 4;
	movement.y /= 4;

	m_foreground.setPosition(movement+m_originalPos);
	
	// Change color when player is dead
	m_deadAlpha -= 255.f * elapsedTime;
	if (m_deadAlpha < 0)
		m_deadAlpha = 0;

	sf::Color tempAlpha{ m_deadBackground.getFillColor() };
	tempAlpha.a = static_cast<sf::Uint8>(m_deadAlpha);
	m_deadBackground.setFillColor(tempAlpha);

	// Change color when player is in a darkZone
	if (m_inDarkZone)
	{
		m_darkAlpha += 255.f / m_colorChangeRate * elapsedTime;
		if (m_darkAlpha > 175)
			m_darkAlpha = 175;

		m_overlayAlpha += 255.f / m_colorChangeRate * elapsedTime;
		if (m_overlayAlpha > 175)
			m_overlayAlpha = 175;
	}
	else
	{
		if (m_deadAlpha == 0)
		{
			m_darkAlpha -= 255.f / m_colorChangeRate * elapsedTime;
			if (m_darkAlpha < 0)
				m_darkAlpha = 0;
		}
	
		m_overlayAlpha -= 255.f / m_colorChangeRate * elapsedTime;
		if (m_overlayAlpha < 0)
			m_overlayAlpha = 0;
	}

	tempAlpha = m_darkBackground.getFillColor();
	tempAlpha.a = static_cast<sf::Uint8>(m_darkAlpha);
	m_darkBackground.setFillColor(tempAlpha);

	tempAlpha = m_darkBackground.getFillColor();
	tempAlpha.a = static_cast<sf::Uint8>(m_overlayAlpha);
	m_darkOverlay.setFillColor(tempAlpha);
}

void GameBackground::died()
{
	m_deadAlpha = 255;
	m_overlayAlpha = 0;
}

void GameBackground::setDarkZone(const bool isInDarkZone)
{
	m_inDarkZone = isInDarkZone;
}

void GameBackground::setTarget(const sf::Vector2f & target)
{
	m_target = target;
}

void GameBackground::init(const sf::Vector2f & size)
{
	
	m_foreground.setTextureRect(sf::IntRect{ 0,0,static_cast<int>(size.x),static_cast<int>(size.y) });
	m_foreground.setOrigin(m_foreground.getLocalBounds().width / 2, m_foreground.getLocalBounds().height / 2);

	m_foreground.setPosition(size.x/2,size.y/2);
	m_originalPos = m_foreground.getPosition();
	m_target = m_foreground.getPosition();

	m_background.setSize(size);
	m_deadBackground.setSize(size);
	m_darkBackground.setSize(size);
	m_darkOverlay.setSize(size);
}

const sf::RectangleShape & GameBackground::getDarkOverlay() const
{
	return m_darkOverlay;
}
