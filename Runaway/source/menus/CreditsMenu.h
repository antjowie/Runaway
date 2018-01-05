#pragma once
#include "Menu.h"
#include "TextObject.h"

#include <vector>

class CreditsMenu:
	public Menu
{
private:
	std::vector<TextObject> m_text;
	
public:
	virtual void input(sf::RenderWindow &window) override final;
	virtual void update(const float elapsedTime) override final;
	virtual void draw(sf::RenderWindow &window) override final;

	CreditsMenu(MenuStack * const menuStack);
};