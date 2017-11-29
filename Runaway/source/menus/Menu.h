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
private:
	std::vector<Object*> m_objects;	// Vector of all objects. self explanitory (I hope)
	std::string m_title;			// Title of the menu, shown in the bar above
	bool m_titleSwap{ true };		// Bool so that title wont be updated endlessly

protected:
	bool m_isPop{ false };			// If true the menu will pop from the stack
	MenuStack *m_menuStack;			// Pointer towards the m_menuStack. Used to push new menu's

public:
	Menu(MenuStack* const menuStack, const std::string &title = "Runaway");
	virtual ~Menu();

	virtual void input(sf::RenderWindow &window) ;	// Gets input
	virtual void update(const float elapsedTime);	// Updates all valid objects
	virtual void draw(sf::RenderWindow &window);	// Draws all valid objects

	bool const isPop() const;								// Checks if this menu has to be popped
	void pushObject(Object* const object);					// Pushes an object to the vector
	void pushObject(const std::vector<Object*> objects);	// Overloaded to reduce code
	void clearObject();										// Clears all the objects in the vector

	void changeTitle(const std::string &title);
	void defaultTitle();
};