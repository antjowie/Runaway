//
// The MainMenu class handles the main menu
//

#pragma once
#include "Menu.h"
#include "TextButtonObject.h"
#include "GameSelectMenu.h"

class MainMenu :
	public Menu
{
private:
	std::vector<TextButtonObject*> m_buttons;
	LevelName m_level;

	void init(); // If player returns from GameSelect, continue button will not be loaded. This funtion fixed that
public:
	MainMenu(MenuStack *const menuStack);
	virtual ~MainMenu() override final;

	virtual void input(sf::RenderWindow &window);
	virtual void update(const float elapsedTime);
	virtual void draw(sf::RenderWindow &window);
};