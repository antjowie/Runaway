#include "Menu.h"
#include "MenuStack.h"


Menu::Menu(MenuStack* const menuStack, const std::string &title) :
	m_menuStack(menuStack),
	m_title(title)
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
	for (const auto &iter : m_objects)
	{
		if (iter->isValid())
		{
			iter->input(window);
		}
	}
}

void Menu::update(const float elapsedTime)
{
	for (const auto &iter : m_objects)
	{
		if (iter->isValid()) 
		{
			iter->logic(elapsedTime);
		}
	}
}

void Menu::draw(sf::RenderWindow & window)
{
	for (const auto &iter : m_objects)
	{
		if (iter->isValid()) 
		{
			window.draw(*iter);
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

void Menu::changeTitle(sf::RenderWindow &window)
{
	window.setTitle(m_title);
}