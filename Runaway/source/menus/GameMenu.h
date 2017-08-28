//
// The game menu class will manage all the levels and saving
//

#pragma once
#include "Menu.h"
#include "Camera.h"
#include "Level.h"
#include "Config.h"
#include "PlayerObject.h"

class GameMenu :
	public Menu
{
private:
	PlayerObject *m_player; // I have 2 pointers toward player, one in the m_objects and one here.
	Camera m_camera;
	int m_levelId;
	Level *m_level;

    void changeLevel(const int level);

public:
	GameMenu(MenuStack* const menuStack);
	~GameMenu();

	virtual void input(sf::RenderWindow &window);
	virtual void update(const float elapsedTime);
	virtual void draw(sf::RenderWindow &window);
};