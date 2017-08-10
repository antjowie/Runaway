//
// Class made for interactive buttons
// Type of object
//

#pragma once
#include "TextObject.h"

class TextButtonObject :
	public TextObject
{
private:
	short m_fade = 0;
	bool m_hover = false;
	bool m_isClicked = false;

protected:
	sf::IntRect m_body;
	sf::RectangleShape m_bodyTexture;

public:
	TextButtonObject();

	virtual void draw(sf::RenderWindow &window);
	virtual void logic(const sf::Time &elapsedTime);
	virtual void input(sf::RenderWindow& window);

	virtual void resize(sf::RenderWindow &window);

	void setBody(const sf::IntRect &bodyPos);
	sf::IntRect const &getBody() const;
};