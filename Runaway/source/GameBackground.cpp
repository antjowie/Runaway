#include "GameBackground.h"
#include "DataManager.h"

GameBackground::GameBackground(const std::string &levelPath, const float colorChangeRate):
	m_colorChangeRate(colorChangeRate)
{
	DataManager::getInstance().loadTexture("foreground", levelPath + "/foreground.png", true);
	m_foreground.setTexture(DataManager::getInstance().getTexture("foreground"));
	
	m_background.setSize(sf::Vector2f(10000, 10000));
	m_background.setFillColor(sf::Color::Cyan);
	
	m_deadBackground.setSize(sf::Vector2f(10000, 10000));
	m_deadBackground.setFillColor(sf::Color::Red);
}

GameBackground::GameBackground()
{
}

void GameBackground::update( float elapsedTime)
{
	elapsedTime /= m_colorChangeRate;
	
	m_alpha -= 255 * elapsedTime;
	if (m_alpha < 0)
		m_alpha = 0;

	sf::Color alpha{ m_deadBackground.getFillColor() };
	alpha.a = m_alpha;
	m_deadBackground.setFillColor(alpha);
}

void GameBackground::draw(sf::RenderWindow & window)
{
	window.draw(m_background);
	window.draw(m_deadBackground);
	window.draw(m_foreground);
}

void GameBackground::setAlpha(const sf::Uint8 & alpha)
{
	m_alpha = alpha;
}

void GameBackground::setTarget(const sf::Vector2f & target)
{
	m_target = target;
}
