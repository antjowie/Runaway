//
// The game menu class will manage all the levels and saving
//

#pragma once
#include "Menu.h"
#include "Camera.h"
#include "Level.h"
#include "Config.h"

class GameMenu :
	public Menu
{
private:
	Camera m_camera;
	Level *m_level;

    void changeLevel(const int level);

public:
	GameMenu(MenuStack* const menuStack);
	~GameMenu();

	virtual void input(sf::RenderWindow &window);
	virtual void update(const float elapsedTime);
	virtual void draw(sf::RenderWindow &window);
};