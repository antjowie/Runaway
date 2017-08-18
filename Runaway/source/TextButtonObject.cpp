#include "TextButtonObject.h"
#include "DataManager.h"

TextButtonObject::TextButtonObject(const bool isValid) :
	TextObject(isValid)
{
}

TextButtonObject::TextButtonObject(const std::string &text,const bool isValid) :
	TextObject(text, isValid)
{
}

TextButtonObject::TextButtonObject(const std::string & text, const Function function, const bool isValid):
	TextButtonObject(text, isValid)
{
	m_textButtonBody.setFunction(function);
}

void TextButtonObject::draw(sf::RenderWindow & window)
{
	TextObject::draw(window);
	m_textButtonBody.draw(window);
}

void TextButtonObject::logic(const float elapsedTime)
{
	m_textButtonBody.logic(elapsedTime);
}

void TextButtonObject::input(sf::RenderWindow & window)
{
	m_textButtonBody.input(window);
}

void TextButtonBody::setBody(const sf::IntRect & bodyPos)
{
	m_body = bodyPos;
	m_bodyTexture.setPosition(sf::Vector2f(static_cast<float>(bodyPos.left), static_cast<float>(bodyPos.top)));
	m_bodyTexture.setSize(sf::Vector2f(static_cast<float>(bodyPos.width), static_cast<float>(bodyPos.height)));
}

void TextButtonBody::setFunction(const Function function)
{
	m_function = function;
}

Function TextButtonBody::getFunction() const
{
	return m_isClicked ? m_function : Function::Nothing;
}

sf::IntRect const & TextButtonBody::getBody() const
{
	return m_body;
}

sf::RectangleShape const & TextButtonBody::getBodyTexture() const
{
	return m_bodyTexture;
}

void TextButtonBody::draw(sf::RenderWindow & window)
{
	window.draw(m_bodyTexture);
}

void TextButtonBody::logic(const float elapsedTime)
{
	// The middle rvalue represents the ammount in 1 second (8 means that the maxfade will be reached in 1/8 of a second)
	const int maxFade{ 150 };							// Max opacity of fade
	const float fadeIn{ maxFade * 8 * elapsedTime };	// Fade in speed
	const float fadeOut{ maxFade * 2 * elapsedTime };	// Fade out speed

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

void TextButtonBody::input(sf::RenderWindow & window)
{
	sf::Vector2f mousePositions = window.mapPixelToCoords(sf::Mouse::getPosition(window));	// For resizing purposes

	m_hover = m_body.contains(static_cast<sf::Vector2i>(mousePositions)) ? true : false;
	m_isClicked = (m_hover && sf::Mouse::isButtonPressed(sf::Mouse::Left)) ? true : false;
}
