#include "GameMenu.h"
#include "DataManager.h"
#include "Config.h"

#include <iostream>
#include <cassert>

GameMenu::GameMenu(MenuStack* const menuStack, LevelName &levelName, LevelName &currentLevel):
	Menu(menuStack), m_levelName(levelName), m_levelProgress(currentLevel)
{
	m_player = new PlayerObject(m_soundManager,true);

	std::string levelPath("Runaway/data/levels/");
	std::string lower(getLevelName(levelName));
	std::transform(lower.begin(), lower.end(), lower.begin(), tolower);
	m_level = new Level(levelPath + lower + '/', getLevelName(levelName) + " level", 1.f, "test");

	assert(m_level->loadLevel(m_camera, m_player, m_background, m_light) && "Load level failed");
	changeTitle("Runaway - " + m_level->getTitle());

	m_soundManager.play();
}

GameMenu::~GameMenu()
{
	Menu::~Menu();
	delete m_player;
	delete m_level;
}

void GameMenu::input(sf::RenderWindow & window)
{
	Menu::input(window);
	m_player->input(window);
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

		case sf::Event::MouseButtonPressed:
			//event.mouseButton.button == sf::Mouse::Left ? m_camera.zoom(1.05f) : m_camera.zoom(0.95f);
			break;
		}
	}
}

void GameMenu::update(const float elapsedTime)
{
	Menu::update(elapsedTime);
	if (m_level == nullptr) return;

	// Had to be called after update because this will fix positions when player already has moved
	m_level->update(elapsedTime);

	m_player->m_lightPool.setRate(m_level->inDarkZone(m_player->m_sprite.getHitbox()) ? -50.f : 200.f);
	m_player->logic(elapsedTime);
	
	m_camera.setView(m_player->m_sprite.getPos());
	m_camera.update(elapsedTime);

	// Update entity when entity collision
	for (const auto &entity : m_level->getEntityMap())
	{
		if (entity->getHitbox().intersects(m_player->m_sprite.getHitbox()))
			switch (entity->getType())
			{
			case EntityType::Checkpoint:
				for (const auto &toggler : m_level->getEntityMap())
					if (toggler->getType() == EntityType::Checkpoint)
						toggler->m_isActive = false;
				entity->m_isActive = true;
				m_level->setSpawn(entity->getAction().pos);
				break;

			case EntityType::Coin:
				
				break;				

			case EntityType::Finish:
				// If player completes level
				if (m_levelName >= m_levelProgress)
					m_levelProgress = static_cast<LevelName>(static_cast<int>(m_levelProgress) + 1);
				m_isPop = true;
				break;

			}
		for(auto &iter: m_player->m_launcher.getProjectiles())
			if(entity->getType() == EntityType::Switch && entity->getHitbox().intersects(iter.m_sprite.getGlobalBounds()))
			{
				for (const auto &toggler : m_level->getEntityMap())
				{
					if (toggler->getType() == EntityType::Switch && toggler->getAction().value == entity->getAction().value)	
						toggler->m_isActive = toggler->m_isActive ? false : true;
				}
					
				m_level->toggleGate(entity->getAction().value);
				iter.m_life = 0;
			}
	}
	
	// Update background state
	m_background.setTarget(m_camera.getView().getCenter());
	m_background.update(elapsedTime);

	// Update foreground state
	m_light.setBrightness(m_level->inDarkZone(m_player->m_sprite.getHitbox()) ? static_cast<sf::Uint8>(0) : static_cast<sf::Uint8>(255));
	m_light.update(elapsedTime);

	// Dead conditions
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) m_player->m_isDead = true;
	if (m_level->inLevelBounds(m_player->m_sprite.getPos()))
		m_player->m_isDead = true;

	// Check player alive state
	if (m_player->m_isDead)
	{
		m_player->m_sprite.setPos(m_level->getSpawn());
		m_player->m_isDead = false;

		m_background.died();
	}

	m_soundManager.update();
}

void GameMenu::draw(sf::RenderWindow & window)
{
	Menu::draw(window);
	window.setView(m_camera.getView());
	
	window.draw(m_background);

	m_level->draw(window, m_camera);
	window.draw(m_light,sf::BlendMultiply);
	 
	window.draw(*m_player);
}