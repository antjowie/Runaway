#include "Menu.h"

class DiedMenu : public Menu
{
public: 
	DiedMenu(MenuStack *const menuStack);

	virtual void input(sf::RenderWindow &window);
	virtual void update(const float elapsedTime);
	virtual void draw(sf::RenderWindow &window);
};