#include "MainMenu.h"
#include "DataManager.h"

#include <iostream>
MainMenu::MainMenu(MenuStack *const menuStack) :
	Menu(menuStack)
{
	// Background
	m_objects.push_back(new BackgroundObject());

	// Menu buttons
	TextButtonObject *menuButtons[3];
	for (int i = 0; i < 3; ++i)
	{
		menuButtons[i] = new TextButtonObject();
		menuButtons[i]->setText(sf::Vector2f(100, 420 + static_cast<float>(100 * i)));
		menuButtons[i]->setBody(sf::IntRect(0, 400 + 100 * i, 1280, 60));
	}

	menuButtons[0]->setString("PLAY");
	menuButtons[0]->setFunction(Function::Play);
	menuButtons[1]->setString("OPTIONS");
	menuButtons[1]->setFunction(Function::Options);
	menuButtons[2]->setString("QUIT");
	menuButtons[2]->setFunction(Function::Quit);

	for (int i = 0; i < 3; ++i)
		m_buttons.push_back(menuButtons[i]);

	// Title
	TextObject *title = new TextObject();
	title->setString("RUNAWAY");
	title->setText(sf::Vector2f(100, 175));
	title->setTextSize(100);

	m_objects.push_back(title);
}

MainMenu::~MainMenu()
{
	Menu::~Menu();

}

void MainMenu::input(sf::RenderWindow & window)
{
	// Update objects
	for (auto iter : m_objects)
		if (iter->isValid())
			iter->input(window);

	for (auto iter : m_buttons)
		if (iter->isValid())
			iter->input(window);

	// Run standard event loop
	sf::Event event;
	while (window.pollEvent(event)) 
	{
		switch (event.type)
		{	
		case sf::Event::Closed:
			window.close();
			break; 

			// For button functions
		case sf::Event::MouseButtonPressed:
			for(auto iter: m_buttons)
				switch (iter->getFunction()) 
				{
					case Function::Play:
						std::cout << "PLAY\n";
						break;
					
					case Function::Options:
						std::cout << "OPTIONS\n";
						break;
					
					case Function::Quit:
						std::cout << "QUIT\n";
						break;
				
					case Function::Nothing:
						break;
					
					default:
						break;
				}
			break;

		default:
			break;
		}
	}
}

void MainMenu::update(const float elapsedTime)
{
	Menu::update(elapsedTime);
	for (auto iter : m_buttons)
	{
		if (iter->isValid())
			iter->logic(elapsedTime);
	}
}

void MainMenu::draw(sf::RenderWindow & window)
{
	Menu::draw(window);
	for (auto iter : m_buttons)
	{
		if (iter->isValid())
			iter->draw(window);
	}
}

BackgroundObject::BackgroundObject()
{
	m_background.setTexture(DataManager::getInstance().getTexture("mainMenuBackground"));
	m_isValid = true;
}

void BackgroundObject::draw(sf::RenderWindow & window)
{
	window.draw(m_background);
}

void BackgroundObject::logic(const float elapsedTime)
{
}

void BackgroundObject::input(sf::RenderWindow & window)
{
}

