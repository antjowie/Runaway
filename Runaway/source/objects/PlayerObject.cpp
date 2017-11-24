#include "PlayerObject.h"
#include "Config.h"
#include "DataManager.h"

PlayerObject::PlayerObject(const bool isValid) :
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

	// Otherwise player is initialized with wrong hitbox which makes him teleport when spawned
	m_animHandler.changeAnimation(0);
	m_sprite.setTextureRect(m_animHandler.getFrame());

	// Init launcher
	m_launcher.setPos(m_sprite.getPos());
	m_launcher.setFireRate(0.5f);
	m_launcher.setProjectileSpeed(32 * 8);
	m_launcher.setProjectileLife(3);
	m_launcher.setProjectileSize(sf::Vector2f(25, 25));

	// Init trail
	m_trail.loadLife(0.35f);
	m_trail.loadSprite(&m_sprite);
}

void PlayerObject::logic(const float elapsedTime)
{
	sf::Vector2f oldPos{ m_sprite.getPos() };
	
	if (m_collisionHandler.distanceTillBottomCollision(m_sprite.getHitbox()) != 0)
	{
		m_sprite.m_sprite.move(0, -m_collisionHandler.distanceTillBottomCollision(m_sprite.m_sprite.getGlobalBounds()));
		if (m_collisionHandler.distanceTillUpperCollision(m_sprite.getHitbox()) != 0)
  			m_isDead = true;
	}
	
	m_sprite.update(elapsedTime, m_collisionHandler);
	//m_sprite.debugMove(elapsedTime);
	sf::Vector2f newPos{ m_sprite.getPos() - oldPos };
	
	// Brace for some ugly vector checking for animation
	float offset{ elapsedTime };

	// Vertical movement
	if (newPos.x >= -offset && (newPos.y < -offset || newPos.y > offset) && m_sprite.m_hasJumped)
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
	m_sprite.setTextureRect(sf::IntRect(m_sprite.getTextureRect().left + m_sprite.getTextureRect().width / 4, 
										m_sprite.getTextureRect().top + 2, m_sprite.getTextureRect().width - 32 + 14, 
										m_sprite.getTextureRect().height - 2));

	// Fix player origin (for camera centralization)
	m_sprite.fixOrigin();

	// Move the light rect
	if (m_lightPool.isCapped()) m_lightPool.setSize(75.f);
	m_lightPool.setPos(m_sprite.getPos());
	m_lightPool.update(elapsedTime);

	// Check if light pool's empty
	if (m_lightPool.isEmpty())
	{
		m_darknessTimeline += elapsedTime;
		if (m_darknessTimeline > 5)
			m_isDead = true;
	}
	else
	{
		m_darknessTimeline -= elapsedTime;
		if (m_darknessTimeline < 0)
			m_darknessTimeline = 0;
	}
	// Update launcher
	m_launcher.setPos(m_sprite.getPos());
	m_launcher.update(elapsedTime);

	// Update trail
	if (m_sprite.m_dashCooldown == 0)
		m_trail.pushTrail();
	m_trail.update(elapsedTime);
}

void PlayerObject::input(sf::RenderWindow &window)
{	
	if (!window.hasFocus()) return;
	if(!m_isDead)
	m_sprite.input();
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		unsigned int temp{ m_launcher.getProjectiles().size() };
		m_launcher.shoot(window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView()));
		if (temp < m_launcher.getProjectiles().size())
			m_lightPool.depleteLight(m_launcher.getProjectileSize().x + m_launcher.getProjectileSize().y);
	}
}

void PlayerObject::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_lightPool, states);
	target.draw(m_trail, states);
	target.draw(m_sprite, states);
	target.draw(m_launcher, states);
}