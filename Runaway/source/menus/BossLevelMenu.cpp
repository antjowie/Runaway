#include "BossLevelMenu.h"
#include "GameSelectMenu.h"
#include "DiedMenu.h"
#include "Config.h"
#include "MenuStack.h"

void BossLevelMenu::input(sf::RenderWindow & window)
{
	Menu::input(window);
	m_player->input(window);
	sf::Event event;

	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			// Redundant?
			Config::getInstance().saveConfig();
			window.close();
			break;

		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Key::Escape) m_isPop = true;
			break;
		}
	}
}

void BossLevelMenu::update(const float elapsedTime)
{
	Menu::update(elapsedTime);
	if (m_level == nullptr) return;

	// Had to be called after update because this will fix positions when player already has moved
	m_level->update(elapsedTime);

	const bool inDarkZone{ m_level->inDarkZone(m_player->m_sprite.getHitbox()) };

	m_player->m_lightPool.setRate(50.f);
	m_player->m_inDarkZone = inDarkZone;
	m_player->logic(elapsedTime);

	m_camera.setView(m_player->m_sprite.getPos());
	m_camera.update(elapsedTime);

	// Update boss
	m_core->setPlayerPos(m_player->m_sprite.getPos());
	m_core->updateCollision(m_player->m_launcher.getProjectiles());
	m_core->update(elapsedTime);

	// Check boss bullet collision
	if (m_core->hit(m_player->m_sprite.getHitbox()))
		m_player->m_lightPool.depleteLight(50.f, false);

	// Update background state
	m_background.setInDarkZone(inDarkZone);
	m_background.setTarget(m_camera.getView().getCenter());
	m_background.update(elapsedTime);

	// Update foreground state
	m_light.setBrightness(inDarkZone ? static_cast<sf::Uint8>(0) : static_cast<sf::Uint8>(255));
	m_light.update(elapsedTime);

	// Dead conditions
	if (m_level->inLevelBounds(m_player->m_sprite.getPos()) || m_player->m_lightPool.isEmpty())
		m_player->m_isDead = true;

	// Check player alive state
	if (m_player->m_isDead)
	{
		delete m_core;
		m_core = new Core();

		m_soundManager.setTargetVolume(0, SoundType::Effect);
		m_menuStack->push(new DiedMenu(m_menuStack));

		m_player->m_sprite.setPos(m_level->getOriginalSpawn());
		m_player->m_isDead = false;

		m_background.died();
	}

	m_soundManager.update();
}

void BossLevelMenu::draw(sf::RenderWindow & window)
{
	Menu::draw(window);
	window.setView(m_camera.getView());

	window.draw(m_background);

	m_level->draw(window, m_camera);
	window.draw(m_light, sf::BlendMultiply);

	window.draw(*m_player);
	window.draw(*m_core);
}

BossLevelMenu::BossLevelMenu(MenuStack *const menuStack):
	Menu(menuStack,"Runaway - The Core")
{
	m_player = new PlayerObject(m_soundManager, true);

	std::string levelPath("Runaway/data/levels/");
	std::string lower(getLevelName(LevelName::TheCore));
	std::transform(lower.begin(), lower.end(), lower.begin(), tolower);
	m_level = new Level(levelPath + lower + '/', getLevelName(LevelName::TheCore), 1.f, "test");

	m_level->loadLevel(m_camera, m_player, m_background, m_light, m_soundManager) && "Load level failed";

	m_core = new Core();

	m_soundManager.play();
}

BossLevelMenu::~BossLevelMenu()
{
	delete m_player;
	delete m_level;
	delete m_core;
}
