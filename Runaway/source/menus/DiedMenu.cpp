#include "DiedMenu.h"
#include "TextButtonObject.h"

DiedMenu::DiedMenu(MenuStack *const menuStack):
	Menu::Menu(menuStack,"Runaway - You died")
{
	

	std::vector<Object* > buttons;
	buttons.push_back(new TextButtonObject("CONTINUE", Function::Continue, true));
	buttons.push_back(new TextButtonObject("EXIT", Function::Back, true));

	for (size_t i{ 0 }; i < buttons.size(); i++)
	{
		TextButtonObject &iter{ buttons[i] };
		iter.setBody(sf::IntRect(0, 220, 1280, 250));
	}

	pushObject(buttons);
}
