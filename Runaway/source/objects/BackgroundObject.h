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
	sf::Sprite m_background;
	void virtual draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
	BackgroundObject(const std::string &texture, bool isValid = false);
};