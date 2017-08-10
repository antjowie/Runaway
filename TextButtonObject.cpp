#include "TextButtonObject.h"
#include "DataManager.h"
#include <iostream>

TextButtonObject::TextButtonObject() :
	TextObject()
{
}

void TextButtonObject::draw(sf::RenderWindow & window)
{
	window.draw(m_bodyTexture);
	TextObject::draw(window);
}

void TextButtonObject::logic(const sf::Time & elapsedTime)
{
	const int fadeIn = 25;		// Fade in speed
	const int fadeOut = 5;		// Fade out speed
	const int maxFade = 150;	// Max opacity of fade

	m_fade -= fadeOut;

	if (m_hover)
		m_fade += fadeIn;

	// Bounds check
	if (m_fade > maxFade)
		m_fade = maxFade;
	else if (m_fade <= 0)
		m_fade = 0;

	m_bodyTexture.setFillColor(sf::Color(255, 100, 100, static_cast<sf::Uint8>(m_fade)));

	if (m_isClicked)
		std::cout << "click";
}

void TextButtonObject::input(sf::RenderWindow & window)
{
	sf::Vector2f mousePositions = window.mapPixelToCoords(sf::Mouse::getPosition(window));	// For resizing purposes
	if (m_body.contains(static_cast<sf::Vector2i>(mousePositions)))
		m_hover = true;
	else
		m_hover = false;

	if (m_hover && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		m_isClicked = true;
	else
		m_isClicked = false;
}

void TextButtonObject::resize(sf::RenderWindow & window)
{

}

void TextButtonObject::setBody(const sf::IntRect & bodyPos)
{
	m_body = bodyPos;
	m_bodyTexture.setPosition(sf::Vector2f(static_cast<float>(bodyPos.left), static_cast<float>(bodyPos.top)));
	m_bodyTexture.setSize(sf::Vector2f(static_cast<float>(bodyPos.width), static_cast<float>(bodyPos.height)));
}

sf::IntRect const & TextButtonObject::getBody() const
{
	return m_body;
}