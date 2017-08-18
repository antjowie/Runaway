#include "GameMenu.h"
#include "DataManager.h"
#include "BackgroundObject.h"

void GameMenu::changeLevel()
{
}

GameMenu::GameMenu(MenuStack* const menuStack):
	Menu(menuStack)
{
	m_objects.push_back(new BackgroundObject("mainMenuBackground",true));
}


GameMenu::~GameMenu()
{
	Menu::~Menu();
}

void GameMenu::input(sf::RenderWindow & window)
{
	sf::Event event;
	
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed: 
			window.close();
			break;

		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Key::Escape) m_isPop = true;
			break;
		}
	}
}

void GameMenu::update(const float elapsedTime)
{
	Menu::update(elapsedTime);
}

void GameMenu::draw(sf::RenderWindow & window)
{
	Menu::draw(window);
	Level level("Runaway/data/levels/level1/level1.tmx", 100, 100);
	level.loadLevel(m_tileMap, m_camera);
}
