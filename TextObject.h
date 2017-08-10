//
// Class made to show text
//  Type of object
//

#pragma once
#include "Menu.h"

class TextObject :
	public Object
{
private:
	sf::Text m_text;

public:
	TextObject();

	virtual void draw(sf::RenderWindow &window);
	virtual void logic(const float elapsedTime);
	virtual void input(sf::RenderWindow& window);

	void setText(const sf::Vector2f pos);
	void setTextSize(const unsigned int newTextSize);
	void setString(const std::string text);

	sf::Vector2f const &getText() const;
};