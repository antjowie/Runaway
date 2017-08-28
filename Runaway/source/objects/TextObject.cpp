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
	m_text.setString(text);
}

void TextObject::draw(sf::RenderWindow & window)
{
	m_text._draw(window);
}

void TextObject::logic(const float elapsedTime)
{
}

void TextObject::input(sf::RenderWindow & window)
{
}

void TextObject::Text::setFont(const sf::Font & font)
{
	m_text.setFont(font);
}

void TextObject::Text::setText(const sf::Vector2f &pos)
{
	m_text.setPosition(pos);
}

void TextObject::Text::setTextSize(const unsigned int newTextSize)
{
	m_text.setCharacterSize(newTextSize);
}

sf::Vector2f const &TextObject::Text::getText() const
{
	return m_text.getPosition();
}

void TextObject::Text::_draw(sf::RenderWindow & window)
{
	window.draw(m_text);
}

void TextObject::Text::setString(const std::string &text)
{
	m_text.setString(text);
}

void TextObject::Text::setOriginToLeftMiddle()
{
	m_text.setOrigin(0, m_text.getLocalBounds().top + m_text.getLocalBounds().height / 2);
}
 