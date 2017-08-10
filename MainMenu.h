//
// The MainMenu class handles the main menu
//

#pragma once
#include "Menu.h"

class BackgroundObject :
	public Object
{
private:
	sf::Sprite m_background;

public:
	BackgroundObject();

	virtual void draw(sf::RenderWindow &window);					
	virtual void logic(const sf::Time &elapsedTime);				
	virtual void input(sf::RenderWindow& window);												
};

class MainMenu :
	public Menu
{
public:
	MainMenu(MenuStack *const menuStack);

	virtual void input(sf::RenderWindow &window);
};

