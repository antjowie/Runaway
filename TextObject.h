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
	virtual void logic(const sf::Time &elapsedTime);
	virtual void input(sf::RenderWindow& window);

	sf::Vector2f const &getText() const;
	void setText(const sf::Vector2f pos);
	void setString(const std::string text);
};