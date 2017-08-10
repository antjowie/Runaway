//
// Template for all object classes
//

#pragma once
#include <SFML\Graphics.hpp>

class Object
{
protected:
	bool m_isValid = false;	// Checks the valid condition

public:
	Object();

	virtual void logic(const sf::Time &elapsedTime) = 0;	// Runs the logic behind the object (the amount of hp)
	virtual void input(sf::RenderWindow& window) = 0;		// Checks if specific logic is allowed to run (changes m_isvalid)
	virtual void draw(sf::RenderWindow &window) = 0;		// Draws the object

	bool const isValid() const;						// Checks if object is valid (the hud)
};