//
// Class for the options menu
//

#pragma once
#include "Menu.h"
#include "TextButtonObject.h"

class OptionsMenu :
	public Menu
{
private:
	std::vector<TextButtonObject*> m_buttons;

public:
	OptionsMenu(MenuStack* const menuStack);
	~OptionsMenu();

	virtual void input(sf::RenderWindow &window);
	virtual void update(const float elapsedTime);
	virtual void draw(sf::RenderWindow &window);
};

