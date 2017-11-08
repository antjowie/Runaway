//
// Template for all object classes
//

#pragma once
#include <SFML\Graphics.hpp>

class Object: public sf::Drawable
{
protected:
	bool m_isValid;	// Checks the valid condition

public:
	Object(const bool isValid = false);

	virtual void logic(const float elapsedTime);		// Runs the logic behind the object (the amount of hp)
	virtual void input(sf::RenderWindow& window);		// Checks if specific logic is allowed to run (changes m_isvalid)

	bool const isValid() const;						// Checks if object is valid (the hud)
	void setValid(const bool isValid);				// Sets the valid value
};