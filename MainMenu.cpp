#include "MainMenu.h"
#include "DataManager.h"
#include "BackgroundObject.h"

MainMenu::MainMenu(MenuStack *const menuStack) :
	Menu(menuStack)
{
	// Maybe unnecessarily 
	m_objects.reserve(2);
	m_buttons.reserve(3);

	// Background
	m_objects.push_back(new BackgroundObject("mainMenuBackground",false));

	// Menu buttons
	m_buttons.push_back(new TextButtonObject("PLAY", Function::Play, true));
	m_buttons.push_back(new TextButtonObject("OPTIONS", Function::Options, true));
	m_buttons.push_back(new TextButtonObject("QUIT", Function::Quit, true));

	int i{ 0 };
	for (auto iter: m_buttons)
	{
		const int offset{ i++ * 100 };
		iter->setOriginToLeftMiddle();
		iter->setText(sf::Vector2f(100, 430 + offset));
		iter->setBody(sf::IntRect(0, 400 + offset, 1280, 60));
	}

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