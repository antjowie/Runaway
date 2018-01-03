//
// BossLevelMenu is the same as GameMenu
// but needs special conditions
//
// Next time when making things like this
// I should use GameMenu as parent class instead of level
//

#pragma once
#include "Menu.h"
#include "Level.h"
#include "Core.h"

class BossLevelMenu:
	public Menu
{
private:
	SoundManager m_soundManager;
	GameBackground m_background;
	Light m_light;
	PlayerObject *m_player;
	Camera m_camera;
	Level *m_level;
	Core *m_core;

public:
	virtual void input(sf::RenderWindow &window) override final;
	virtual void update(const float elapsedTime) override final;
	virtual void draw(sf::RenderWindow &window) override final;

	BossLevelMenu(MenuStack * const menuStack);
	virtual ~BossLevelMenu();
};