//
// The game menu class will manage all the levels and saving
//

#pragma once
#include "Menu.h"
#include "Camera.h"
#include "Level.h"

#include <vector>

class GameMenu :
	public Menu
{
private:
	std::vector<std::vector<Tile*>> *m_tileMap;
	Camera m_camera;
	Level *m_level;
	int m_levelCount;

    void changeLevel();

public:
	GameMenu(MenuStack* const menuStack);
	~GameMenu();

	virtual void input(sf::RenderWindow &window);
	virtual void update(const float elapsedTime);
	virtual void draw(sf::RenderWindow &window);
};

