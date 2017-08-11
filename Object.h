//
// Template for all object classes
//

#pragma once
#include <SFML\Graphics.hpp>

class Object
{
protected:
	bool m_isValid;	// Checks the valid condition

public:
	Object(const bool isValid = false);

	virtual void logic(const float elapsedTime) = 0;		// Runs the logic behind the object (the amount of hp)
	virtual void input(sf::RenderWindow& window) = 0;		// Checks if specific logic is allowed to run (changes m_isvalid)
	virtual void draw(sf::RenderWindow &window) = 0;		// Draws the object

	bool const isValid() const;						// Checks if object is valid (the hud)
	void setValid(const bool isValid);				// Sets the valid value
};