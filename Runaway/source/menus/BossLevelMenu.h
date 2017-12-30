//
// BossLevelMenu is the same as GameMenu
// but needs special conditions
//

#pragma once
#include "Menu.h"

class BossLevelMenu:
	public Menu
{
public:
	virtual void input(sf::RenderWindow &window) override final;
	virtual void update(const float elapsedTime) override final;
	virtual void draw(sf::RenderWindow &window) override final;

	BossLevelMenu(MenuStack * const menuStack);
	~BossLevelMenu();
};

