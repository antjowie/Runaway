//
// The MainMenu class handles the main menu
//

#pragma once
#include "Menu.h"
#include "TextButtonObject.h"

class BackgroundObject :
	public Object
{
private:
	sf::Sprite m_background;

public:
	BackgroundObject();

	virtual void draw(sf::RenderWindow &window);					
	virtual void logic(const float elapsedTime);				
	virtual void input(sf::RenderWindow& window);												
};

class MainMenu :
	public Menu
{
private:
	std::vector<TextButtonObject*> m_buttons;

public:
	MainMenu(MenuStack *const menuStack);
	~MainMenu();

	virtual void input(sf::RenderWindow &window);
	virtual void update(const float elapsedTime);
	virtual void draw(sf::RenderWindow &window);
};