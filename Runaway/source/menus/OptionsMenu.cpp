#include "OptionsMenu.h"
#include "DataManager.h"
#include "BackgroundObject.h"
#include "TextObject.h"

OptionsMenu::OptionsMenu(MenuStack * const menuStack):
	Menu(menuStack)
{
	std::vector<Object*> tempVec;
	tempVec.reserve(2);
	m_buttons.reserve(1);

	// Initialize background (temp)
	tempVec.push_back(new BackgroundObject("mainMenuBackground", true));

	TextObject *tempText = new TextObject("THE OPTION MENU IS WORK IN PROGRESS", true);
	tempText->m_text.setText(sf::Vector2f(180, 300));
	tempVec.push_back(tempText);

	// Initialize buttons
	m_buttons.push_back(new TextButtonObject("BACK", Function::Back, true));
	m_buttons[0]->m_text.setOriginToLeftMiddle();
	m_buttons[0]->m_text.setText(sf::Vector2f(100, 630));
	m_buttons[0]->m_textButtonBody.setBody(sf::IntRect(0, 600, 1280, 60));

	pushObject(tempVec);
}

OptionsMenu::~OptionsMenu()
{
	Menu::~Menu();
	for(auto iter : m_buttons)
		delete iter;
	m_buttons.clear();
}

void OptionsMenu::input(sf::RenderWindow & window)
{
	Menu::input(window);
	for (auto iter : m_buttons)
		if (iter->isValid()) iter->input(window);

	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::MouseButtonPressed:
				for (auto iter : m_buttons)
				{
					switch (iter->m_textButtonBody.getFunction())
					{
					case Function::Back:
						m_isPop = true;
						break;
				
					}
				}
		}
	}
}

void OptionsMenu::update(const float elapsedTime)
{
	Menu::update(elapsedTime);
	for (auto iter : m_buttons)
		if (iter->isValid()) iter->logic(elapsedTime);
}

void OptionsMenu::draw(sf::RenderWindow & window)
{
	Menu::draw(window);
	for (auto iter : m_buttons)
		if (iter->isValid()) iter->draw(window);
}