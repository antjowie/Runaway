#include "Menu.h"

Object::Object(sf::RenderWindow & window)
{
	resize(window);
}

Menu::Menu(MenuStack* const menuStack):
	menuStack(menuStack)
{
}
