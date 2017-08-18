//
// Class made to show text
// Type of object
//

#pragma once
#include "Menu.h"

class Text {
private:
	sf::Text m_text;

public:
	void setFont(const sf::Font &font);
	void setText(const sf::Vector2f &pos);
	void setTextSize(const unsigned int newTextSize);
	void setString(const std::string &text);
	void setOriginToLeftMiddle();	// Odd name but sets the origin to the middle of the left edge

	sf::Vector2f const &getText() const;
	
	void draw(sf::RenderWindow &window);
};

class TextObject :
	public Object
{
public:
	Text m_text;

	TextObject(const bool isValid = false);
	TextObject(const std::string &text, const bool isValid = false);

	virtual void draw(sf::RenderWindow &window);
	virtual void logic(const float elapsedTime);
	virtual void input(sf::RenderWindow& window);
};