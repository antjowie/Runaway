//
// BossLevelMenu is the same as GameMenu
// but needs special conditions
//

#pragma once
#include "Menu.h"
#include "Level.h"

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

	bool m_resetLevel;

public:
	virtual void input(sf::RenderWindow &window) override final;
	virtual void update(const float elapsedTime) override final;
	virtual void draw(sf::RenderWindow &window) override final;

	BossLevelMenu(MenuStack * const menuStack);
};