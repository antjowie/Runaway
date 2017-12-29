#include "PlayerSprite.h"
#include "Config.h"

#include <iostream>

bool const Sprite::isItemPressed(const std::string itemString) const
{
	// This looks totally odd
	return
		((Config::getInstance().getConfig(itemString).itemType == Item::ItemType::Keyboard && sf::Keyboard::isKeyPressed(Config::getInstance().getConfig(itemString).keyboard)) ||
		(Config::getInstance().getConfig(itemString).itemType == Item::ItemType::Mouse && sf::Mouse::isButtonPressed(Config::getInstance().getConfig(itemString).mouse)))
		== true ? true : false;
}

Sprite::Sprite() :
	m_velocity(0, 0),
	m_moveDirection(0, 0)
{
	m_sprite.setColor(sf::Color(0, 0, 0, 255));
}

void Sprite::input()
{
	// Reset directions
	m_moveDirection.x = m_moveDirection.y = 0;

	if (isItemPressed("jump") && m_canJump)
		m_moveDirection.y = -1;

	if ((isItemPressed("dash") && (m_dashCooldown == 0)))
		m_hitDash = true;

	if (isItemPressed("moveLeft")) m_moveDirection.x -= 1;
	if (m_moveDirection.x < -1) m_moveDirection.x = -1;
	if (isItemPressed("moveRight")) m_moveDirection.x += 1;
	if (m_moveDirection.x > 1) m_moveDirection.x = 1;
}

void Sprite::update(const float elapsedTime, CollisionHandler & collisionHandler)
{
	const float
		tileSize{ 32 },
		speed{ 3.5 },	// Blocks to run with accel = 1
		gravity{ 24 },	// Blocks to drop in 1 second

		oneTileTravel{ 2.f * gravity },
		jumpStrength{ sqrt(oneTileTravel * 3.f) },	// Amount of blocks to jump in one second (won't go higher)

		maxAccel{ 2 },	// Max accel  
		accel{ 4 },		// accel / maxAccel = time to reach max accel
		decel{ 4 },		// decel / maxAccel = time to reach 0 accel

		dashCooldown{ 4 },
		dashStrength{ 3 };	// Should be blocks to dash but is arbitrary at the moment (value isn't precise)

	// Jump
	if (m_moveDirection.y == -1)
	{
		if (!m_hasJumped)
		{
			m_hasJumped = true;
			m_canJump = false;
			m_velocity.y = -jumpStrength * tileSize;
		}
	}

	if (m_hasJumped && m_velocity.y < 0 && !(isItemPressed("jump")))
		m_velocity.y += gravity * tileSize * elapsedTime;

	// This is placed in the middle because otherwise the desired jump height won't be reached
	m_sprite.move(0, m_velocity.y * elapsedTime);
	// Gravity
	if (collisionHandler.distanceTillBottomCollision(getHitbox()) == 0)
	{
		m_velocity.y += gravity * tileSize*  elapsedTime;
		sf::FloatRect tempRect{ getHitbox() };
		tempRect.top += 1;
		if (collisionHandler.distanceTillBottomCollision(tempRect) != 0)
			m_hasJumped = false;
		else
			m_hasJumped = true;
	}

	// If bottom is in block
	if (m_velocity.y > 0 && collisionHandler.distanceTillBottomCollision(getHitbox()) != 0)
	{
		m_sprite.move(0, -collisionHandler.distanceTillBottomCollision(getHitbox()));
		m_velocity.y = 0;
		if (!isItemPressed("jump"))
			m_canJump = true;
		m_hasJumped = false;
	}

	// If head hits top
	else if (m_velocity.y < 0 && collisionHandler.distanceTillUpperCollision(getHitbox()) != 0)
	{
		m_sprite.move(0, collisionHandler.distanceTillUpperCollision(getHitbox()));
		m_velocity.y = gravity;
	}

	// Horizontal movement
	if (m_moveDirection.x == 1)
	{
		if (m_velocity.x < 0)
			m_velocity.x = 0;
		if (m_velocity.x > maxAccel)
			m_velocity.x -= decel * elapsedTime;
		else
			m_velocity.x += accel * elapsedTime;
	}

	else if (m_moveDirection.x == -1)
	{
		if (m_velocity.x > 0)
			m_velocity.x = 0;
		if (m_velocity.x < -maxAccel)
			m_velocity.x += decel * elapsedTime;
		else
			m_velocity.x -= accel * elapsedTime;
	}

	if (m_hitDash && m_moveDirection.x != 0)
	{
		m_hitDash = false;
		if (m_moveDirection.x > 0)
			m_velocity.x += dashStrength;
		else if (m_moveDirection.x < 0)
			m_velocity.x -= dashStrength;
		m_dashCooldown = dashCooldown;
	}

	m_sprite.move(m_velocity.x * speed * tileSize * elapsedTime, 0);

	for (int i{ 0 }; i < 1; i++)
	{
		// Collision resolution
		if (m_velocity.x > 0 && collisionHandler.distanceTillRightCollision(getHitbox()) != 0)
		{
			m_sprite.move(-collisionHandler.distanceTillRightCollision(getHitbox()), 0);
			m_velocity.x = 0;
		}
		else if (m_velocity.x < 0 && collisionHandler.distanceTillLeftCollision(getHitbox()) != 0)
		{
			m_sprite.move(collisionHandler.distanceTillLeftCollision(getHitbox()), 0);
			m_velocity.x = 0;
		}
	}

	// Decrease accelerations
	if (m_moveDirection.x == 0)
	{
		if (m_velocity.x > 0)
		{
			m_velocity.x -= decel * elapsedTime;
			if (m_velocity.x < 0)
				m_velocity.x = 0;
		}
		else if (m_velocity.x < 0)
		{
			m_velocity.x += decel * elapsedTime;
			if (m_velocity.x > 0)
				m_velocity.x = 0;
		}
	}

	// Decrease dash cooldown
	m_dashCooldown -= elapsedTime;
	if (m_dashCooldown < 0)
		m_dashCooldown = 0;
}

void Sprite::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

void Sprite::debugMove(const float elapsedTime)
{
	const float speed{ 32 * 4 };
	if (m_moveDirection.x == 1)
	{
		m_velocity.x = 2;
		m_sprite.move(m_velocity.x * elapsedTime * speed, 0);
	}
	else if (m_moveDirection.x == -1)
	{
		m_velocity.x = -2;
		m_sprite.move(m_velocity.x * elapsedTime* speed, 0);

	}
	else m_velocity.x = 0;
	if (m_moveDirection.y == -1)
	{
		m_velocity.y = -2;
		m_sprite.move(0, m_velocity.y * elapsedTime* speed);

	}
	else m_velocity.y = 0;
	m_moveDirection.x = m_moveDirection.y = 0;
}

void Sprite::setPos(const sf::Vector2f & pos)
{
	m_sprite.setPosition(pos);
}

void Sprite::setTextureRect(const sf::IntRect & textureRect)
{
	m_sprite.setTextureRect(textureRect);
}

void Sprite::setTexture(const sf::Texture & texture)
{
	m_sprite.setTexture(texture);
}

void Sprite::fixOrigin()
{
	m_sprite.setOrigin(m_sprite.getGlobalBounds().width / 2, m_sprite.getGlobalBounds().height / 2);
}

float Sprite::getMaxSpeed() const
{
	// TODO
	// Give class variables to calculate this instead of handwriting it
	return 3.5f * 2.f * 32.f;
}

const sf::FloatRect Sprite::getHitbox() const
{
	return m_sprite.getGlobalBounds();
}

const sf::IntRect Sprite::getTextureRect() const
{
	return m_sprite.getTextureRect();
}

const sf::Vector2f Sprite::getPos() const
{
	return m_sprite.getPosition();
}