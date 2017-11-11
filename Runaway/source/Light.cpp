#include "Light.h"
#include <cassert>
#include <iostream>

void Light::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.blendMode = sf::BlendMultiply;
	target.draw(m_light, states);
}

Light::Light()
{
	m_overlay.setPosition(0, 0);
	m_light.setPosition(0, 0);
}

void Light::addTile(const std::vector<const Tile*>& tiles)
{	
	for (const auto &iter : tiles)
		addTile(iter);
}

void Light::addTile(const Tile * const tile)
{
	if (tile->getTileMeta().m_light != 0.f)
		m_tiles.push_back(tile);
}

void Light::update(const float elapsedTime)
{
	m_elapsedTime += elapsedTime;
	if (m_elapsedTime > m_fadeTime)
		m_elapsedTime = m_fadeTime;

	// Update overlay brightness
	const float percentage{ m_elapsedTime / m_fadeTime }; // Should give a number between 0 and 1
	assert(percentage >= 0 && percentage <= 1 && "Percentage is above or under bounds");
	
	// Lerp function
	sf::Uint8 newColor{ static_cast<sf::Uint8>((1.f - percentage)*static_cast<float>(m_oldBrightness) + percentage*static_cast<float>(m_brightnessTarget)) };
	m_overlay.setFillColor(sf::Color(newColor, newColor, newColor));
	
	// I'd rather like this into the draw function, but it is a const function
	m_lightTex.clear();
	m_lightTex.draw(m_overlay);
	
	for (const auto &iter : m_tiles)
		m_lightTex.draw(*iter, sf::BlendAdd);
	
	m_lightTex.display();

	m_light.setTexture(m_lightTex.getTexture());
}

void Light::setBrightness(const sf::Uint8 brightness)
{
	if (m_brightnessTarget == brightness) return;
	m_elapsedTime = 0;
	m_oldBrightness = m_overlay.getFillColor().b; // b, g and r should be the same
	m_brightnessTarget = brightness;
}

void Light::setFadeTime(const float time)
{
	m_fadeTime = time;
}

void Light::setSize(const sf::Vector2f size)
{
	m_lightTex.create(size.x,size.y);
	m_overlay.setSize(size);
}

void Light::setSize(const float width, const float height)
{
	setSize(sf::Vector2f(width,height));
}