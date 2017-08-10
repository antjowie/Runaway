#include "MainMenu.h"
#include "DataManager.h"
#include "TextButtonObject.h"

MainMenu::MainMenu(MenuStack *const menuStack) :
	Menu(menuStack)
{
	m_objects.push_back(new BackgroundObject());

	TextButtonObject *play = new TextButtonObject;
	play->setString("PLAY");
	play->setBody(sf::IntRect(0, 400, 1280, 60));
	play->setText(sf::Vector2f(100, 420));

	TextButtonObject *options = new TextButtonObject;
	options->setString("OPTIONS");
	options->setBody(sf::IntRect(0, 500, 1280, 60));
	options->setText(sf::Vector2f(100, 520));

	TextButtonObject *quit = new TextButtonObject;
	quit->setString("QUIT");
	quit->setBody(sf::IntRect(0, 600, 1280, 60));
	quit->setText(sf::Vector2f(100, 620));

	m_objects.push_back(play);
	m_objects.push_back(options);
	m_objects.push_back(quit);
}

void MainMenu::input(sf::RenderWindow & window)
{
	sf::Event event;
	while (window.pollEvent(event)) 
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break; 
		case sf::Event::Resized:
		default:
			break;
		}
	}
	
	for (auto iter : m_objects)
		if (iter->isValid())
			iter->input(window);
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

void BackgroundObject::logic(const sf::Time & elapsedTime)
{
}

void BackgroundObject::input(sf::RenderWindow & window)
{
}

