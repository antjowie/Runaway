//
// Template for all menu classes
//

#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include <map>

class Object
{
protected:
	sf::Rect<int> m_body;	// Body of the object. Used for logic or textures
	bool m_isValid = false;	// Checks the valid condition

public:
	Object();

	virtual void logic(const sf::Time &elapsedTime) = 0;	// Runs the logic behind the object (the amount of hp)
	virtual void input(sf::RenderWindow& window) = 0;		// Checks if specific logic is allowed to run (changes m_isvalid)
	virtual void draw(sf::RenderWindow &window) = 0;		// Draws the object
	
	bool isValid() const;					// Checks if object is valid (the hud)
	void resize(sf::RenderWindow &window);	// Resizes textures
};

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
	void update(const sf::Time &elapsedTime);			// Updates all valid objects
	void draw(sf::RenderWindow &window);				// Draws all valid objects
};