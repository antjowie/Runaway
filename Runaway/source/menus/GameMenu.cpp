#include "GameMenu.h"
#include "DataManager.h"
#include "Config.h"
#include "MenuStack.h"
#include "TileHeader.h"
#include "DiedMenu.h"
#include <iostream>
GameMenu::GameMenu(MenuStack* const menuStack, LevelName &levelName, LevelName &currentLevel):
	Menu(menuStack), m_levelName(levelName), m_levelProgress(currentLevel), m_resetSound(false)
{
	m_player = new PlayerObject(m_soundManager,true);

	std::string levelPath("Runaway/data/levels/");
	std::string lower(getLevelName(levelName));
	std::transform(lower.begin(), lower.end(), lower.begin(), tolower);
	m_level = new Level(levelPath + lower + '/', getLevelName(levelName) + " level", 1.f, "test");

	m_level->loadLevel(m_camera, m_player, m_background, m_light) && "Load level failed";
	
	changeTitle("Runaway - " + m_level->getTitle());

	m_soundManager.setTargetVolume(Config::getInstance().getConfig("effects").integer, SoundType::Effect);
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
			Config::getInstance().saveConfig();
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
	if (m_level == nullptr) return;

	// Had to be called after update because this will fix positions when player already has moved
	m_level->update(elapsedTime);

	const bool inDarkZone{ m_level->inDarkZone(m_player->m_sprite.getHitbox()) };
	if (inDarkZone)
		for (const auto &vertic : m_level->getTilemap())
		{
			bool found{ false };
			for (const auto &tile : vertic)
			{
				if (tile->getTileMeta().m_tileType != TileType::Light) continue;
				const float radius{ 32 * 3.f };
				const sf::Vector2f lightPos{ tile->getHitbox().left + tile->getHitbox().width / 2, tile->getHitbox().top + tile->getHitbox().height / 2 };
				const sf::Vector2f playerPos{ m_player->m_sprite.getHitbox().left, m_player->m_sprite.getHitbox().top };

				if (std::powf(lightPos.x - playerPos.x, 2) + std::powf(lightPos.y - playerPos.y, 2) <= radius * radius)
				{
					m_player->m_lightPool.setRate(200.f);
					found = true;
					break; // Very necessary indeed
				}
			}
			if (found) break;
			m_player->m_lightPool.setRate(-50.f);
		}
	else
		m_player->m_lightPool.setRate(150.f);

	m_player->m_inDarkZone = inDarkZone;
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
				entity->getAction();
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
	m_background.setInDarkZone(inDarkZone);
	m_background.setTarget(m_camera.getView().getCenter());
	m_background.update(elapsedTime);

	// Update foreground state
	m_light.setBrightness(m_level->inDarkZone(m_player->m_sprite.getHitbox()) ? static_cast<sf::Uint8>(0) : static_cast<sf::Uint8>(255));
	m_light.update(elapsedTime);

	// Dead conditions
	if (m_level->inLevelBounds(m_player->m_sprite.getPos()))
		if (inDarkZone)
			m_player->m_isDead = true;
		else
			m_player->m_respawn = true;

	if (m_resetSound)
	{
		m_soundManager.setTargetVolume(Config::getInstance().getConfig("effects").integer, SoundType::Effect);
		m_resetSound = false;
	}

	// Check player alive state
	if (m_player->m_isDead)
	{
		m_soundManager.setTargetVolume(0, SoundType::Effect);
		m_menuStack->push(new DiedMenu(m_menuStack));

		m_player->m_sprite.setPos(m_level->getOriginalSpawn());
		m_player->m_isDead = false;

		m_background.died();
		m_resetSound = true;
	}

	if (m_player->m_respawn)
	{
		m_player->m_sprite.setPos(m_level->getSpawn());
		m_player->m_lightPool.depleteLight(50.f,false);
		m_player->m_respawn = false;
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