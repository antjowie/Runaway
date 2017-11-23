//
// This menu select the level
//

#pragma once
#include "Menu.h"
#include "GameMenu.h"
#include <vector>

class GameSelectMenu :
	public Menu
{
private:
	LevelName m_currentLevel{ LevelName::Test };

	struct LevelButton
	{
		sf::RectangleShape m_rect;
		sf::RectangleShape m_unlocked;
		sf::Text m_text;
		LevelName m_level{ LevelName::Blank };
		bool m_hover{ false };
	}m_levels[static_cast<int>(LevelName::Blank)];

	std::string getLevelName(const LevelName levelName) const;
	sf::Texture &getLevelBackground(const LevelName levelName) const;
	
public:
	GameSelectMenu(MenuStack* const menuStack);
	
	virtual void input(sf::RenderWindow &window) override final;
	virtual void update(const float elapsedTime) override final;
	virtual void draw(sf::RenderWindow &window)  override final;
};
