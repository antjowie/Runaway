#include "BossLevelMenu.h"
#include "GameSelectMenu.h"
#include "DiedMenu.h"
#include "Config.h"
#include "MenuStack.h"
#include "CreditsMenu.h"

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

	m_finish->logic(elapsedTime);

	// Had to be called after update because this will fix positions when player already has moved
	m_level->update(elapsedTime);

	const bool inDarkZone{ m_core->inDarkZone() };

	if (inDarkZone && m_coins < static_cast<int>(LevelName::TheCore))
		m_player->m_lightPool.setRate(-300.f);
	else
		m_player->m_lightPool.setRate(50.f);
	m_player->m_inDarkZone = true;
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
	if (m_resetSound)
	{
		m_soundManager.setTargetVolume(Config::getInstance().getConfig("effects").integer, SoundType::Effect);
		m_resetSound = false;
	}

	if (m_player->m_isDead)
	{
		delete m_core;
		m_core = new Core(m_soundManager);

		m_soundManager.setTargetVolume(0, SoundType::Effect);
		m_resetSound = true;
		m_menuStack->push(new DiedMenu(m_menuStack));

		m_player->m_lightPool.setRate(1000000.f);
		m_player->m_lightPool.update(1.f);
		m_player->m_sprite.setPos(m_level->getOriginalSpawn());
		m_player->m_isDead = false;

		m_background.died();
	}
	m_soundManager.update();

	if (m_core->m_completed && m_finish->getHitbox().intersects(m_player->m_sprite.getHitbox()))
	{
		m_soundManager.setTargetVolume(0, SoundType::Effect);
		m_menuStack->push(new CreditsMenu(m_menuStack));
	}

	// Update spinning coins
	m_coinCircle.rotate(360 * elapsedTime / 10.f);
	m_coinCircle.setPosition(m_player->m_sprite.getPos());
	for (size_t i = 0; i < m_coins; i++)
	{
		m_coinSprites[i].logic(elapsedTime);
		sf::Vector2f pos{ m_coinCircle.getTransform().transformPoint(m_coinCircle.getPoint(i)) };
		m_coinSprites[i].setPosition(pos - sf::Vector2f(16,14));
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

	if(m_core->m_completed)
	window.draw(*m_finish);

	if (m_core->inDarkZone())
		for (const auto&iter : m_coinSprites)
			window.draw(iter);
}

BossLevelMenu::BossLevelMenu(MenuStack *const menuStack):
	Menu(menuStack,"Runaway - The Core")
{
	for (int i = 0; i < static_cast<int>(LevelName::Blank); i++)
		if (Config::getInstance().getConfig((std::string("coin") + std::to_string(i))).logic)
			m_coins++;

	m_coinCircle.setPointCount(m_coins);
	m_coinCircle.setRadius(32 * 3);
	m_coinCircle.setFillColor(sf::Color::Transparent);
	m_coinCircle.setOrigin(m_coinCircle.getRadius(), m_coinCircle.getRadius());

	for (int i = 0; i < m_coins; i++)
	{
		sf::Sprite coin;
		EntityAction temp;
		m_coinSprites.push_back(Coin(temp, sf::Vector2f(0, 0)));
	}

	m_player = new PlayerObject(m_soundManager, true);

	m_player->m_lightPool.setRate(1000000.f);
	m_player->m_lightPool.update(1.f);

	std::string levelPath("Runaway/data/levels/");
	std::string lower(getLevelName(LevelName::TheCore));
	std::transform(lower.begin(), lower.end(), lower.begin(), tolower);
	m_level = new Level(levelPath + lower + '/', getLevelName(LevelName::TheCore), 1.f, "test");

	m_level->loadLevel(m_camera, m_player, m_background, m_light, m_soundManager) && "Load level failed";

	m_core = new Core(m_soundManager);
	m_soundManager.play();

	EntityAction temp;
	temp.value = 0;
	m_finish = new Finish(temp, sf::Vector2f(1360, 1920));
}

BossLevelMenu::~BossLevelMenu()
{
	delete m_player;
	delete m_level;
	delete m_core;
}
