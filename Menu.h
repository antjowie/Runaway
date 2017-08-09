//
// Template for all menu classes
//

#pragma once
#include <SFML\Graphics.hpp>
#include <vector>

class Object
{
private:
	sf::Rect<int> m_body;	// Body of the object. Used for logic or textures

public:
	Object(sf::RenderWindow &window);

	virtual void draw(sf::RenderWindow &window) = 0;		// Draws the object
	virtual void logic(const sf::Time &elapsedTime) = 0;	// Runs the logic behind the object (the amount of hp)
	virtual bool isValid() = 0;								// Checks if object is valid (the hud)
	virtual void resize(sf::RenderWindow &window) = 0;		// Resizes textures
};

class MenuStack;	// Declare that this class will be used

class Menu
{
private:
	MenuStack *menuStack;			// Pointer towards the menuStack. Used to push new menu's
	std::vector<Object> m_objects;	// Vector of all objects. self explanitory (I hope)

public:
	Menu(MenuStack* const menuStack);

	virtual void input(sf::RenderWindow &window) = 0;		// Gets input
	virtual void update(const sf::Time elapsedTime)	= 0;	// Updates all valid objects
	virtual void draw(sf::RenderWindow &window) = 0;		// Draws all valid objects
};