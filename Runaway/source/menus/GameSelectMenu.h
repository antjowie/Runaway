//
// This menu select the level
// All new levels go into this class
// To add a level, just place it into the LevelName enum and getLevelName function
// All levels are made with tiled, do be carefull about the structure
//

#pragma once
#include "Menu.h"
#include <vector>

enum class LevelName {
	WakeUp,
	TheFactory,
	TheHallway,
	TheGate,
	TheMine,
	TheLaboratory,
	TheElevator,
	TheCore,
	Blank
};

std::string getLevelName(const LevelName levelName);

class GameSelectMenu :
	public Menu
{
private:
	LevelName m_currentLevel;

	float m_timeline{ 0 };

	struct LevelButton
	{
		sf::RectangleShape m_rect;
		sf::RectangleShape m_unlocked;
		sf::Text m_text;
		LevelName m_level{ LevelName::Blank };
		bool m_hover{ false };
	}m_levels[static_cast<int>(LevelName::Blank)];

	sf::Texture &getLevelBackground(const LevelName levelName) const;
	
public:
	GameSelectMenu(MenuStack* const menuStack);
	~GameSelectMenu();

	virtual void input(sf::RenderWindow &window) override final;
	virtual void update(const float elapsedTime) override final;
	virtual void draw(sf::RenderWindow &window)  override final;
};
