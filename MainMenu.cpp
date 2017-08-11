#include "MainMenu.h"
#include "DataManager.h"
#include "BackgroundObject.h"

MainMenu::MainMenu(MenuStack *const menuStack) :
	Menu(menuStack)
{
	// Background
	m_objects.push_back(new BackgroundObject("mainMenuBackground",false));

	// Menu buttons
	TextButtonObject *menuButtons[3] { new TextButtonObject(true),new TextButtonObject(true), new TextButtonObject(true) };
	
	menuButtons[0]->setString("PLAY");
	menuButtons[0]->setFunction(Function::Play);
	menuButtons[1]->setString("OPTIONS");
	menuButtons[1]->setFunction(Function::Options);
	menuButtons[2]->setString("QUIT");
	menuButtons[2]->setFunction(Function::Quit);

	for (int i = 0; i < 3; ++i)
	{
		//menuButtons[i]->setOriginToMiddle();
		menuButtons[i]->setText(sf::Vector2f(100, 400 + static_cast<float>(100 * i)));
		menuButtons[i]->setBody(sf::IntRect(0, 400 + 100 * i, 1280, 60));
	}


	for (int i = 0; i < 3; ++i)
		m_buttons.push_back(menuButtons[i]);

	// Title
	TextObject *title = new TextObject("RUNAWAY",true);
	title->setText(sf::Vector2f(100, 175));
	title->setTextSize(100);

	m_objects.push_back(title);
}

MainMenu::~MainMenu()
{
	Menu::~Menu();
	for (auto iter : m_buttons)
	{
		delete iter;
	}
	m_buttons.clear();
}

void MainMenu::input(sf::RenderWindow & window)
{
	// Update objects
	for (auto iter : m_objects)
	{
		if (iter->isValid())
		{ 
			iter->input(window);
		}
	}

	for (auto iter : m_buttons)
	{
		if (iter->isValid())
		{
			iter->input(window);
		}
	}

	// Run standard event loop
	sf::Event event;
	while (window.pollEvent(event)) 
	{
		switch (event.type)
		{	
			// For button functions
		case sf::Event::MouseButtonPressed:
			for (auto iter : m_buttons)
			{
				switch (iter->getFunction())
				{
				case Function::Play:
					break;

				case Function::Options:
					break;

				case Function::Quit:
					window.close();
					break;

				case Function::Nothing:
					break;
				}
			}
			break;

		case sf::Event::Closed:
			window.close();
			break; 

		case sf::Event::KeyPressed:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			{
				window.close();
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
		{
			iter->logic(elapsedTime);
		}
	}
}

void MainMenu::draw(sf::RenderWindow & window)
{
	Menu::draw(window);
	for (auto iter : m_buttons)
	{
		if (iter->isValid())
		{
		iter->draw(window);
		}
	}
}