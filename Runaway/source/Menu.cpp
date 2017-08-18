#include "Menu.h"

Menu::Menu(MenuStack* const menuStack):
	m_menuStack(menuStack)
{
}

Menu::~Menu()
{
	for (auto iter : m_objects)
	{
		delete iter;
	}
	m_objects.clear();
}

void Menu::update(const float elapsedTime)
{
	for (auto iter : m_objects)
	{
		if (iter->isValid()) 
		{
			iter->logic(elapsedTime);
		}
	}
}

void Menu::draw(sf::RenderWindow & window)
{
	for (auto iter : m_objects)
	{
		if (iter->isValid()) 
		{
			iter->draw(window);
		}
	}
}

bool const Menu::isPop() const
{
	return m_isPop;
}
