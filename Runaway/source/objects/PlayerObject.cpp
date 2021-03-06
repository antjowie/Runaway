#include "PlayerObject.h"
#include "Config.h"
#include "DataManager.h"

PlayerObject::PlayerObject(SoundManager &soundManager, const bool isValid) :
	Object(isValid),
	m_animHandler(32 , 32)
{
	m_sprite.setTexture(DataManager::getInstance().getTexture("player"));
	
	// Animation related
	// Rest
	m_animHandler.addAnimation(Animation(0, 3, 0.75f, true, false));
	for (int i = 0; i < 4; i++)
		// Jump up right, drop down right, jump up left, drop down left
		m_animHandler.addAnimation(Animation(0, 3, 0.1f, false, true));
	for (int i = 0; i < 2; i++)
		// Walk right, walk left
		m_animHandler.addAnimation(Animation(0, 3, 0.2f, true, false));
	for (int i = 0; i < 2; i++)
		// Dash right, dash left
		m_animHandler.addAnimation(Animation(0, 2, 0.08f, false, false));

	// Otherwise player is initialized with wrong hitbox which makes him teleport when spawned
	m_animHandler.changeAnimation(0);
	m_sprite.setTextureRect(m_animHandler.getFrame());

	// Init launcher
	m_launcher.setPos(m_sprite.getPos());
	m_launcher.setFireRate(0.5f);
	m_launcher.setProjectileSpeed(32 * 12);
	m_launcher.setProjectileLife(2);
	m_launcher.setProjectileSize(sf::Vector2f(25, 25));

	// Init trail
	m_trail.loadLife(0.35f);
	m_trail.loadSprite(&m_sprite);
	
	// Init audio
	m_lightPoolSound = new SoundObject(SoundType::Effect,soundManager);
	m_lightPoolSound->setRelativeToListener(true);
	m_lightPoolSound->setPosition(0, 0, 0);
	m_lightPoolSound->setBuffer(DataManager::getInstance().getSound("lightPool"));
	m_lightPoolSound->setLoop(true);
	m_lightPoolSound->setPitch(0);
}

PlayerObject::~PlayerObject()
{
	m_lightPoolSound->m_shouldPop = true;
}

void PlayerObject::logic(const float elapsedTime)
{
	sf::Vector2f oldPos{ m_sprite.getPos() };
	
	if (m_collisionHandler.distanceTillBottomCollision(m_sprite.getHitbox()) != 0)
	{
		m_sprite.m_sprite.move(0, -m_collisionHandler.distanceTillBottomCollision(m_sprite.m_sprite.getGlobalBounds()));
		if (m_collisionHandler.distanceTillUpperCollision(m_sprite.getHitbox()) != 0)
			if (m_inDarkZone)
				m_isDead = true;
			else
				m_respawn = true;
	}
	
	m_sprite.update(elapsedTime, m_collisionHandler);
	//m_sprite.debugMove(elapsedTime);
	sf::Vector2f newPos{ m_sprite.getPos() - oldPos };
	
	// Brace for some ugly vector checking for animation
	float offset{ elapsedTime };

	// Used for fixing player sprite sheet collision

	bool dashed{ false };
	// Dash movement
	if (newPos.x / elapsedTime > m_sprite.getMaxSpeed() + 10.f)
	{
		m_animHandler.changeAnimation(PlayerDirection::DashRight);
		dashed = true;
	}
	else if (newPos.x / elapsedTime < -m_sprite.getMaxSpeed() - 10.f)
	{
		m_animHandler.changeAnimation(PlayerDirection::DashLeft);
		dashed = true;
	}
	
		// Vertical movement
	else if (newPos.x >= -offset && (newPos.y < -offset || newPos.y > offset) && m_sprite.m_hasJumped)
	{
		if (newPos.y > offset)
			m_animHandler.changeAnimation(PlayerDirection::DropRight);
		else
			m_animHandler.changeAnimation(PlayerDirection::JumpRight);
	}
	else if (newPos.x <= -offset && (newPos.y < -offset || newPos.y > offset) && m_sprite.m_hasJumped)
	{
		if (newPos.y > offset)
			m_animHandler.changeAnimation(PlayerDirection::DropLeft);
		else
			m_animHandler.changeAnimation(PlayerDirection::JumpLeft);
	}

	// Horizontal movement
	else if (newPos.x > offset)
		m_animHandler.changeAnimation(PlayerDirection::Right);
	else if (newPos.x < -offset )
		m_animHandler.changeAnimation(PlayerDirection::Left);

	// No movement
	else
		m_animHandler.changeAnimation(PlayerDirection::Rest);
	
	// Update animation
	m_animHandler.update(elapsedTime);
	m_sprite.setTextureRect(m_animHandler.getFrame());
	
	// Correcting hitbox
	if(!dashed)
	m_sprite.setTextureRect(sf::IntRect(m_sprite.getTextureRect().left + m_sprite.getTextureRect().width / 4, 
										m_sprite.getTextureRect().top + 2, m_sprite.getTextureRect().width - 32 + 14, 
										m_sprite.getTextureRect().height - 2));
	else
		m_sprite.setTextureRect(sf::IntRect(m_sprite.getTextureRect().left,
			m_sprite.getTextureRect().top + 2, m_sprite.getTextureRect().width,
			m_sprite.getTextureRect().height - 2));

	// Fix player origin (for camera centralization)
	m_sprite.fixOrigin();

	// Move the light rect
	if (!m_inDarkZone && m_lightPool.isCapped()) m_lightPool.setSize(75.f);
	m_lightPool.setPos(m_sprite.getPos());
	m_lightPool.update(elapsedTime);

	// Check if light pool's empty
	if (m_lightPool.isEmpty())
	{
		m_darknessTimeline += elapsedTime;
		if (m_darknessTimeline > 3)
			m_isDead = true;
	}
	else
	{
		m_darknessTimeline -= elapsedTime;
		if (m_darknessTimeline < 0)
			m_darknessTimeline = 0;
	}

	// Update sound
	sf::Listener::setPosition(m_sprite.getPos().x, 0.f, m_sprite.getPos().y);
	m_lightPoolSound->setPitch(m_lightPool.getPool()/ m_lightPool.getCap() + (0.2f / (m_lightPool.getPool() + 1.f)));

	m_lightPool.isCapped() ? m_soundTimeline -= 50 * elapsedTime : m_soundTimeline += 50 * elapsedTime;
	if (m_soundTimeline < std::min(Config::getInstance().getConfig("effects").integer, 10))
		m_soundTimeline = std::min(Config::getInstance().getConfig("effects").integer, 10);
	else if (m_soundTimeline > Config::getInstance().getConfig("effects").integer * .5f)
		m_soundTimeline = Config::getInstance().getConfig("effects").integer * .5f;
	m_lightPoolSound->setVolume(m_soundTimeline);

	// Update launcher
	m_launcher.setPos(m_sprite.getPos());
	m_launcher.update(elapsedTime);

	// Update trail
	if (m_sprite.m_dashCooldown == 0 || dashed)
		m_trail.pushTrail();
	m_trail.update(elapsedTime);

	// Update respawn timer
	m_respawnTimeline -= elapsedTime;
	if (m_respawnTimeline < 0)
		m_respawnTimeline = 0;
}

void PlayerObject::input(sf::RenderWindow &window)
{	
	if (!window.hasFocus()) return;
	if(!m_isDead)
	m_sprite.input();
	if (sf::Mouse::isButtonPressed(Config::getInstance().getConfig("shoot").mouse))
	{
		size_t temp{ m_launcher.getProjectiles().size() };
		m_launcher.shoot(window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView()));
		if (temp < m_launcher.getProjectiles().size())
			m_lightPool.depleteLight(m_launcher.getProjectileSize().x + m_launcher.getProjectileSize().y);
	}
	if (m_respawnTimeline == 0  && sf::Keyboard::isKeyPressed(Config::getInstance().getConfig("respawn").keyboard))
	{
		m_respawn = true;
		m_respawnTimeline = 0.5f;
	}
}


void PlayerObject::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_lightPool, states);
	target.draw(m_trail, states);
	target.draw(m_sprite, states);
	target.draw(m_launcher, states);
}