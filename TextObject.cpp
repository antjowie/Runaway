#include "TextObject.h"
#include "DataManager.h"

TextObject::TextObject(const bool isValid):
	Object(isValid)
{
	m_text.setFont(DataManager::getInstance().getFont("pixel"));
}

TextObject::TextObject(const std::string &text, const bool isValid) : 
	TextObject(isValid)
{
	setString(text);
}

void TextObject::draw(sf::RenderWindow & window)
{
	window.draw(m_text);
}

void TextObject::logic(const float elapsedTime)
{
}

void TextObject::input(sf::RenderWindow & window)
{
}

void TextObject::setText(const sf::Vector2f pos)
{
	m_text.setPosition(pos);
}

void TextObject::setTextSize(const unsigned int newTextSize)
{
	m_text.setCharacterSize(newTextSize);
}

sf::Vector2f const &TextObject::getText() const
{
	return m_text.getPosition();
}

void TextObject::setString(const std::string text)
{
	m_text.setString(text);
}

void TextObject::setOriginToLeftMiddle()
{
	m_text.setOrigin(0, m_text.getLocalBounds().height / 2);
}
 