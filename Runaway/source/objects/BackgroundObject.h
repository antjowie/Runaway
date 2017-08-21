//
// Class for backgrounds, this background object is static
// Type of object
//

#pragma once
#include "Object.h"

class BackgroundObject :
	public Object
{
protected:
	sf::Sprite m_background;

public:
	BackgroundObject(const bool isValid = false);
	BackgroundObject(const std::string &texture, bool isValid = false);

	virtual void draw(sf::RenderWindow &window);
	virtual void logic(const float elapsedTime);
	virtual void input(sf::RenderWindow& window);

	void setTexture(const std::string &texture);
};