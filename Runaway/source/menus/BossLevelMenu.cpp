#include "BossLevelMenu.h"
#include "GameSelectMenu.h"

void BossLevelMenu::input(sf::RenderWindow & window)
{
}

void BossLevelMenu::update(const float elapsedTime)
{
}

void BossLevelMenu::draw(sf::RenderWindow & window)
{
}

BossLevelMenu::BossLevelMenu(MenuStack *const menuStack):
	Menu(menuStack,"Runaway - The Core"),
	m_resetLevel(false)
{
	m_player = new PlayerObject(m_soundManager, true);

	std::string levelPath("Runaway/data/levels/");
	std::string lower(getLevelName(LevelName::TheCore));
	std::transform(lower.begin(), lower.end(), lower.begin(), tolower);
	m_level = new Level(levelPath + lower + '/', getLevelName(LevelName::TheCore), 1.f, "test");

	m_level->loadLevel(m_camera, m_player, m_background, m_light, m_soundManager) && "Load level failed";

	m_soundManager.play();
}