#include "LightPool.h"
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RectangleShape.hpp>

void LightPool::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	sf::RectangleShape temp;
	temp.setSize(sf::Vector2f(m_sprite.getTextureRect().width, m_sprite.getTextureRect().height));
	temp.setOrigin(temp.getSize().x / 2, temp.getSize().y / 2);
	temp.setPosition(m_sprite.getPosition());
	temp.setFillColor(sf::Color::Yellow);
	
	target.draw(temp, states);
}

void LightPool::update(const float elapsedTime)
{
}

void LightPool::setPos(const sf::Vector2f & pos)
{
	m_sprite.setPosition(pos);
}

LightPool::LightPool()
{
	m_sprite.setTextureRect(sf::IntRect(0, 0, 32, 50));
	m_sprite.setColor(sf::Color::Yellow);
}

LightPool::~LightPool()
{
}
