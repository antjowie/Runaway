//
// Template for all menu classes
//

#pragma once
#include "Object.h"

#include <SFML\Graphics.hpp>
#include <vector>
#include <map>


class MenuStack;	// Declare that this class will be used

class Menu
{
protected:
	MenuStack *menuStack;			// Pointer towards the menuStack. Used to push new menu's
	std::vector<Object*> m_objects;	// Vector of all objects. self explanitory (I hope)

public:
	Menu(MenuStack* const menuStack);
	~Menu();

	virtual void input(sf::RenderWindow &window) = 0;	// Gets input
	virtual void update(const float elapsedTime);		// Updates all valid objects
	virtual void draw(sf::RenderWindow &window);		// Draws all valid objects
};