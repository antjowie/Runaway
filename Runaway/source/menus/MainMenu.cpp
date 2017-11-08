#include "MainMenu.h"
#include "DataManager.h"
#include "BackgroundObject.h"
#include "MenuStack.h"
#include "OptionsMenu.h"
#include "GameMenu.h"

MainMenu::MainMenu(MenuStack *const menuStack) :
	Menu(menuStack)
{
	// Maybe unnecessarily 
	std::vector<Object*> tempVec;
	tempVec.reserve(2);
	m_buttons.reserve(3);

	// Background
	tempVec.push_back(new BackgroundObject("mainMenuBackground",true));

	// Menu buttons
	m_buttons.push_back(new TextButtonObject("NEW GAME", Function::Play, true));
	m_buttons.push_back(new TextButtonObject("OPTIONS", Function::Options, true));
	m_buttons.push_back(new TextButtonObject("QUIT", Function::Quit, true));

	int i{ 0 };
	for (auto iter: m_buttons)
	{
		const int offset{ i++ * 100 };
		
		iter->setOriginToLeftMiddle();
		iter->setPos(sf::Vector2f(100, 430 + static_cast<float>(offset)));
		iter->setBody(sf::IntRect(0, 400 + offset, 1280, 60));
	}

	// Title
	TextObject *title = new TextObject("RUNAWAY",true);
	title->setPos(sf::Vector2f(100, 175));
	title->setTextSize(100);
	tempVec.push_back(title);

	pushObject(tempVec);
}

MainMenu::~MainMenu()
{
	Menu::~Menu();
	for (auto iter : m_buttons) 
		delete iter;
	m_buttons.clear();
}

void MainMenu::input(sf::RenderWindow & window)
{
	// Update objects
	Menu::input(window);
	for (const auto &iter : m_buttons)
		if (iter->isValid()) iter->input(window);

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
					m_menuStack->push(new GameMenu(m_menuStack));
					break;

				case Function::Options:
					m_menuStack->push(new OptionsMenu(m_menuStack));
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
			if (event.key.code == sf::Keyboard::Key::Escape) window.close();
			break;
		}
	}
}

void MainMenu::update(const float elapsedTime)
{
	Menu::update(elapsedTime);
	for (const auto &iter : m_buttons)
		if (iter->isValid()) iter->logic(elapsedTime);
}

void MainMenu::draw(sf::RenderWindow & window)
{
	window.setView(window.getDefaultView()); // If returning from the gameMenu, restores camera positions
	Menu::draw(window);
	for (const auto &iter : m_buttons)
		if (iter->isValid())
			window.draw(*iter);
}