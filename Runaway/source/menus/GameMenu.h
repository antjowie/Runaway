//
// The game menu class will manage all the levels and saving
//

#pragma once
#include "Menu.h"
#include "Camera.h"
#include "Level.h"
#include "Config.h"
#include "PlayerObject.h"
#include "GameBackground.h"
#include "Light.h"

enum class LevelName {
	Test,
	That,
	Blank
};

class GameMenu :
public Menu
{
private:

	GameBackground m_background;
	Light m_light;
	PlayerObject *m_player; // I have 2 pointers toward player, one in the m_objects and one here, to access the unique functions
	Camera m_camera;
	Level *m_level;

public:
	GameMenu(MenuStack* const menuStack,const LevelName levelName);
	~GameMenu();

	virtual void input(sf::RenderWindow &window);
	virtual void update(const float elapsedTime);
	virtual void draw(sf::RenderWindow &window);
};