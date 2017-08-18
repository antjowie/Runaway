//
// The game menu class will manage all the levels and saving
//

#pragma once
#include "Menu.h"
#include "Camera.h"
#include "Level.h"

class GameMenu :
	public Menu
{
private:
	Level *m_level;
	int m_levelCount;

	//void changeLevel();

public:
	GameMenu(MenuStack* const menuStack);
	~GameMenu();

	virtual void input(sf::RenderWindow &window);
	virtual void update(const float elapsedTime);
	virtual void draw(sf::RenderWindow &window);
};

