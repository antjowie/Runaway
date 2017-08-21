#include "GameMenu.h"
#include "DataManager.h"
#include "BackgroundObject.h"

#include <cassert>

void GameMenu::changeLevel(const int level)
{
	switch (level)
	{
	case 1:
		m_level = new Level("Runaway/data/levels/level1/level1.tmx", "Test level", 3200, 3200, 0.5f, 1550,1728);
		assert(m_level->loadLevel(m_camera) && "Load level failed");
		m_config.loadDefaultConfig();
		m_config.saveConfig();

		break;
		
	default:
		assert(false && "Level doesn't exist");
		break;
	}
}

GameMenu::GameMenu(MenuStack* const menuStack):
	Menu(menuStack)
{
	changeLevel(1);
}

GameMenu::~GameMenu()
{
	Menu::~Menu();
	delete m_level;
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) m_camera.moveTarget(sf::Vector2f(-10, 0));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) m_camera.moveTarget(sf::Vector2f(0, 10));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) m_camera.moveTarget(sf::Vector2f(10, 0));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) m_camera.moveTarget(sf::Vector2f(0, -10));
}

void GameMenu::update(const float elapsedTime)
{
	Menu::update(elapsedTime);
	m_camera.update(elapsedTime);
}

void GameMenu::draw(sf::RenderWindow & window)
{
	Menu::draw(window);
	window.setView(m_camera.getView());
	if (m_level == nullptr) return;
	m_level->draw(window);
}
