#include "OptionsMenu.h"

OptionsMenu::OptionsMenu(MenuStack * const menuStack):
	Menu(menuStack)
{
	TextButtonObject *back = new TextButtonObject("BACK",true);
	back->setBody(sf::IntRect(0, 660, 1280, 60));
	back->setText(sf::Vector2f(0, 660));

	m_buttons.push_back(back);
}

OptionsMenu::~OptionsMenu()
{
}

void OptionsMenu::input(sf::RenderWindow & window)
{
}

void OptionsMenu::update(const float elapsedTime)
{
	Menu::update(elapsedTime);
	for (auto iter : m_buttons)
	{
		if (iter->isValid())
			iter->logic(elapsedTime);
	}
}

void OptionsMenu::draw(sf::RenderWindow & window)
{
	Menu::draw(window);
	for (auto iter : m_buttons)
	{
		if (iter->isValid())
			iter->draw(window);
	}
}
