#include "TextObject.h"
#include "DataManager.h"

void TextObject::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_text,states);
}

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

void TextObject::setFont(const sf::Font & font)
{
	m_text.setFont(font);
}

void TextObject::setPos(const sf::Vector2f &pos)
{
	m_text.setPosition(pos);
}

void TextObject::setTextSize(const unsigned int newTextSize)
{
	m_text.setCharacterSize(newTextSize);
}

sf::Vector2f const &TextObject::getPos() const
{
	return m_text.getPosition();
}

void TextObject::setString(const std::string &text)
{
	m_text.setString(text);
}

void TextObject::setOriginToLeftMiddle()
{
	m_text.setOrigin(0, m_text.getLocalBounds().top + m_text.getLocalBounds().height / 2);
}

void TextObject::setOriginToRightMiddle()
{
	m_text.setOrigin(m_text.getLocalBounds().width, m_text.getLocalBounds().top + m_text.getLocalBounds().height / 2);
}
 