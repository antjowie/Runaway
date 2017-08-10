#include "TextButtonObject.h"
#include "DataManager.h"

TextButtonObject::TextButtonObject() :
	TextObject()
{
}

void TextButtonObject::draw(sf::RenderWindow & window)
{
	window.draw(m_bodyTexture);
	TextObject::draw(window);
}

void TextButtonObject::logic(const float elapsedTime)
{
	const int fadeIn = 25;		// Fade in speed
	const int fadeOut = 5;		// Fade out speed
	const int maxFade = 150;	// Max opacity of fade

	m_fade -= fadeOut;

	if (m_hover)
		m_fade += fadeIn;

	// Fade bounds check
	if (m_fade > maxFade)
		m_fade = maxFade;
	else if (m_fade <= 0)
		m_fade = 0;

	m_bodyTexture.setFillColor(sf::Color(255, 100, 100, static_cast<sf::Uint8>(m_fade)));
}

void TextButtonObject::input(sf::RenderWindow & window)
{
	sf::Vector2f mousePositions = window.mapPixelToCoords(sf::Mouse::getPosition(window));	// For resizing purposes

	m_hover = m_body.contains(static_cast<sf::Vector2i>(mousePositions)) ? true : false;
	m_isClicked = (m_hover && sf::Mouse::isButtonPressed(sf::Mouse::Left)) ? true : false;
}

void TextButtonObject::setBody(const sf::IntRect & bodyPos)
{
	m_body = bodyPos;
	m_bodyTexture.setPosition(sf::Vector2f(static_cast<float>(bodyPos.left), static_cast<float>(bodyPos.top)));
	m_bodyTexture.setSize(sf::Vector2f(static_cast<float>(bodyPos.width), static_cast<float>(bodyPos.height)));
}

void TextButtonObject::setFunction(const Function function)
{
	m_function = function;
}

Function TextButtonObject::getFunction() const
{
	return m_isClicked ? m_function : Function::Nothing;
}

sf::IntRect const & TextButtonObject::getBody() const
{
	return m_body;
}