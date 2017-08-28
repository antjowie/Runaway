//
// Class for backgrounds, this background object is static
// Type of object
//

#pragma once
#include "Object.h"


class BackgroundObject :
	public Object
{
private:
	class Background
	{
	private:
		sf::Sprite m_background;

	public:
		Background(const std::string& texture);

		void _draw(sf::RenderWindow &window);
	};

public:
	Background m_background;

	BackgroundObject(const std::string &texture, bool isValid = false);

	virtual void draw(sf::RenderWindow &window);
	virtual void logic(const float elapsedTime);
	virtual void input(sf::RenderWindow& window);
};