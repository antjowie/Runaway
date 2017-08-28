#include "GameMenu.h"
#include "DataManager.h"
#include "BackgroundObject.h"

#include <cassert>
#include <iostream>

void GameMenu::changeLevel(const int level)
{
	if (m_levelId == level) return;
	m_levelId = level;

	switch (m_levelId)
	{
	case 0:

		break;

	case 1:
		clearObject();

		m_level = new Level("Runaway/data/levels/level1/level1.tmx", "Test level", 1280 / 2, 720 /2, 1.0f, 1550,1728);
		m_player->m_player.setPos(sf::Vector2f(1550, 1728));
		m_camera.setView(m_player->m_player.getPos());
		assert(m_level->loadLevel(m_camera) && "Load level failed");
		Config::getInstance().loadConfig();

		break;
		
	default:
		assert(false && "Level doesn't exist");
		break;
	}
}

GameMenu::GameMenu(MenuStack* const menuStack):
	Menu(menuStack)
{
	// I should start learning smart pointers, shouldn't I? (Wasn't it not called unique_ptr?)
	m_player = new PlayerObject(true);
	changeLevel(1);
}

GameMenu::~GameMenu()
{
	Menu::~Menu();
	delete m_level;
}

void GameMenu::input(sf::RenderWindow & window)
{
	Menu::input(window);
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
	if (m_level == nullptr) return;
	m_player->input(window);
	sf::Mouse::setPosition(sf::Vector2i(1280 / 2, 720 / 2),window);
}

void GameMenu::update(const float elapsedTime)
{
	Menu::update(elapsedTime);
	if (m_level == nullptr) return;
	m_player->logic(elapsedTime);

	m_camera.moveView(m_player->m_player.getPos());
	m_camera.update(elapsedTime);
}

void GameMenu::draw(sf::RenderWindow & window)
{
	Menu::draw(window);
	if (m_level == nullptr) 
	{
		window.setView(window.getDefaultView());
		return;
	}
	m_camera.draw(window);
	m_level->draw(window,m_camera);
	m_player->m_player._draw(window);
}