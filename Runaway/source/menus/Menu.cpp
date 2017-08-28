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

void Menu::input(sf::RenderWindow & window)
{
	for (auto iter : m_objects)
	{
		if (iter->isValid())
		{
			iter->input(window);
		}
	}
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

void Menu::pushObject(Object* const object)
{
	m_objects.push_back(object);
}

void Menu::pushObject(const std::vector<Object*> objects)
{
	for (auto iter : objects)
		pushObject(iter);
}

void Menu::clearObject()
{
	m_objects.clear();
}
