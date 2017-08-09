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

class TextButtonObject :
	public Object
{
private:
	sf::Text m_text;
	sf::RectangleShape m_bodyTexture;
	short m_fade = 0;
	bool hover = false;

public:
	TextButtonObject();

	virtual void draw(sf::RenderWindow &window);					
	virtual void logic(const sf::Time &elapsedTime);				
	virtual void input(sf::RenderWindow& window);

	void setPosition(const sf::Vector2i pos);
	void setBody(const sf::Rect<int> body);
	void setText(const std::string text);
};

class MainMenu :
	public Menu
{
public:
	MainMenu(MenuStack *const menuStack);

	virtual void input(sf::RenderWindow &window);
};

