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
#include "LightObject.h"

class GameMenu :
	public Menu
{
private:
	enum class LevelName {
		Blank,
		Test,
		That
	};

	LightObject m_lightObjects;
	GameBackground m_background;
	PlayerObject *m_player; // I have 2 pointers toward player, one in the m_objects and one here, to access the unique functions
	Camera m_camera;
	LevelName m_levelId{ LevelName::Blank };
	Level *m_level; 

    void changeLevel(const LevelName level);

public:
	GameMenu(MenuStack* const menuStack);
	~GameMenu();

	virtual void input(sf::RenderWindow &window);
	virtual void update(const float elapsedTime);
	virtual void draw(sf::RenderWindow &window);
};