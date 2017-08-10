#include "TextObject.h"
#include "DataManager.h"

TextObject::TextObject()
{
	m_text.setFont(DataManager::getInstance().getFont("pixel"));
	m_isValid = true;
}

void TextObject::draw(sf::RenderWindow & window)
{
	window.draw(m_text);
}

void TextObject::logic(const sf::Time & elapsedTime)
{
}

void TextObject::input(sf::RenderWindow & window)
{
}

void TextObject::setText(const sf::Vector2f pos)
{
	m_text.setPosition(pos);
}

sf::Vector2f const &TextObject::getText() const
{
	return m_text.getPosition();
}

void TextObject::setString(const std::string text)
{
	m_text.setString(text);
}