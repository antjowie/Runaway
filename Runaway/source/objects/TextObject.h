//
// Class made to show text
// Type of object
//

#pragma once
#include "Menu.h"

class TextObject :
	public Object
{
private:
	sf::Text m_text;

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
	TextObject(const bool isValid = false);
	TextObject(const std::string &string, const bool isValid = false);

	void setFont(const sf::Font &font);
	void setPos(const sf::Vector2f &pos);
	void setTextSize(const unsigned int newTextSize);
	void setString(const std::string &text);
	void setOriginToLeftMiddle();	// Odd name but sets the origin to the middle of the left edge
	void setOriginToRightMiddle();	// Odd name but sets the origin to the middle of the left edge

	sf::Vector2f const &getPos() const;
};