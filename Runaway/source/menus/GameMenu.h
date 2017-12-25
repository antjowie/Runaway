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
#include "GameSelectMenu.h" // For one function and LevelName
#include "SoundManager.h"

class GameMenu :
public Menu
{
private:
	LevelName &m_levelName;		// The level the player is playing
	LevelName &m_levelProgress; // The level the player is at

	SoundManager m_soundManager;
	GameBackground m_background;
	Light m_light;
	PlayerObject *m_player; // I have 2 pointers toward player, one in the m_objects and one here, to access the unique functions
	Camera m_camera;
	Level *m_level;

	bool m_resetSound;

public:
	GameMenu(MenuStack* const menuStack,LevelName &levelName, LevelName &currentLevel);
	~GameMenu();

	virtual void input(sf::RenderWindow &window);
	virtual void update(const float elapsedTime);
	virtual void draw(sf::RenderWindow &window);
};