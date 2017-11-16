#include "GameMenu.h"
#include "DataManager.h"

#include <cassert>

void GameMenu::changeLevel(const GameMenu::LevelName level)
{
	if (m_levelId == level) return;
	m_levelId = level;

	clearObject();
	delete m_player;
	delete m_level;

	m_player = new PlayerObject(true);

	std::string levelPath("Runaway/data/levels/");
	switch (m_levelId)
	{
	case LevelName::Test:
		m_level = new Level(levelPath + "test/", "Test level", 1.f, "test");
		break;

	case LevelName::That:
		m_level = new Level(levelPath + "that/", "That level", 1.f, "test");
		break;

	default:
		assert(false && "Level doesn't exist");
		break;
	}
	
	assert(m_level->loadLevel(m_camera,m_player,m_background,m_light) && "Load level failed");
	changeTitle("Runaway - " + m_level->getTitle());
}

GameMenu::GameMenu(MenuStack* const menuStack):
	Menu(menuStack)
{
	changeLevel(LevelName::That);
}

GameMenu::~GameMenu()
{
	Menu::~Menu();
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
			// Temporary placed right here
			for (const auto &iter : m_level->getEntityMap())
			{
				if (iter->getType() == EntityType::Switch && iter->getHitbox().contains(window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y),m_camera.getView())))
				{
					iter->m_isActive = iter->m_isActive ? false : true;
					m_level->toggleGate(iter->getAction().value);
				}
			}

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
	
	sf::Vector2f  offset{ m_player->m_sprite.getMoveDirection() };
	if (offset.x > 0)
		offset.x = m_camera.getView().getSize().x / 2;
	else if (offset.x <0)
		offset.x = -m_camera.getView().getSize().x / 2;
	if (offset.y < 0)
		offset.y = -m_camera.getView().getSize().y / 2;
	else if (offset.y > 0)
	offset.y = m_camera.getView().getSize().y / 2;

	//m_camera.setTarget(m_player->m_sprite.getPos() + offset);
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