#include "PlayerObject.h"
#include "Config.h"
#include "DataManager.h"
#include <iostream>

bool const Sprite::isItemPressed(const std::string itemString) const
{
	// This looks totally odd
	return 
	((Config::getInstance().getConfig(itemString).itemType == Item::ItemType::Keyboard && sf::Keyboard::isKeyPressed(Config::getInstance().getConfig(itemString).keyboard)) ||
	(Config::getInstance().getConfig(itemString).itemType == Item::ItemType::Mouse && sf::Mouse::isButtonPressed(Config::getInstance().getConfig(itemString).mouse))) 
	== true ? true : false;
}

const bool Sprite::isFloating(CollisionHandler &collisionHandler) const
{
	return collisionHandler.distanceTillBottomCollision(getHitbox()) == 0;
}

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
	m_animHandler.changeAnimation(1);
	m_sprite.setTextureRect(m_animHandler.getFrame());
}

void PlayerObject::logic(const float elapsedTime)
{
	m_animHandler.update(elapsedTime);
	sf::Vector2f oldPos{ m_sprite.getPos() };

	m_sprite.update2(elapsedTime, m_collisionHandler);
	sf::Vector2f newPos{ m_sprite.getPos() - oldPos };

	// Brace for some ugly vector checking for animation
	float offset{ elapsedTime };

	// Horizontal movement
	if (newPos.x > offset && newPos.y > -offset && newPos.y < offset)
		m_animHandler.changeAnimation(playerDirection::Right);
	else if (newPos.x < -offset && newPos.y > -offset && newPos.y < offset)
		m_animHandler.changeAnimation(playerDirection::Left);

	// Vertical movement
	else if (newPos.x >= -offset && (newPos.y < -offset || newPos.y > offset))
	{
		if (newPos.y > offset)
			m_animHandler.changeAnimation(playerDirection::DropRight);
		else
			m_animHandler.changeAnimation(playerDirection::JumpRight);
	}
	else if (newPos.x <= -offset && (newPos.y < -offset || newPos.y > offset))
	{
		if (newPos.y > offset)
			m_animHandler.changeAnimation(playerDirection::DropLeft);
		else
			m_animHandler.changeAnimation(playerDirection::JumpLeft);
	}

	// No movement
	else
		m_animHandler.changeAnimation(playerDirection::Rest);
}

void PlayerObject::input(sf::RenderWindow &window)
{	
	if(!m_isDead)
	m_sprite.input();
}

void PlayerObject::draw(sf::RenderWindow &window)
{
	m_sprite.setTextureRect(m_animHandler.getFrame()); // Should this be handled in logic or draw?
	m_sprite.setTextureRect(sf::IntRect(m_sprite.getTextureRect().left + m_sprite.getTextureRect().width / 4,
		m_sprite.getTextureRect().top, m_sprite.getTextureRect().width / 2 - 3, m_sprite.getTextureRect().height));
	m_sprite.draw(window);
}

Sprite::Sprite():
	m_acceleration(0,0),
	m_moveDirection(0,0)
{	
}

void Sprite::input()
{
	// Reset directions
	m_moveDirection.x = m_moveDirection.y = 0;

	if ((isItemPressed("moveUp") || isItemPressed("jump")))
		m_moveDirection.y = -1;

	if (isItemPressed("moveLeft")) m_moveDirection.x -= 1;
	if (m_moveDirection.x < -1) m_moveDirection.x = -1;
	if (isItemPressed("moveRight")) m_moveDirection.x += 1;
	if (m_moveDirection.x > 1) m_moveDirection.x = 1;

	m_isCrouching = isItemPressed("moveDown");
}

void Sprite::update(const float elapsedTime, CollisionHandler &collisionHandler)
{
	sf::Vector2f movement;

	// All these values could be defined by level, but I won't because I have no intention to
	float speed{ 32 * 3 };					// Distance with acceleration 1
	float jumpStrength{ 32 * 20 };			
	float jumpAccelerate{ 32 * 10 };
	float maxAccelerate{ 2 };
	float accelerate{ elapsedTime * 4 };    // How much time to reach that acceleration
	float decelerate{ elapsedTime * 6 };	// How much times slower in one sec
											// TODO: level should define this
	float gravity{ 32 * 10 };				// Drop this many pixels in one sec

	// Update accelerations
	if (m_moveDirection.x == 1)
	{
		if (m_acceleration.x < 0)
			m_acceleration.x = 0;
		m_acceleration.x += accelerate;
	}
	else if (m_moveDirection.x == -1)
	{
		if (m_acceleration.x > 0)
			m_acceleration.x = 0;
		m_acceleration.x -= accelerate;
	}

	if (m_moveDirection.y == -1)
	{
		if (m_canJump) 
		{
			m_acceleration.y = -jumpStrength;
			m_canJump = false;
		}
		if(m_acceleration.y < 0)
			m_acceleration.y -= jumpAccelerate * elapsedTime;
	}

	// Check movement bounds
	if (m_acceleration.x > maxAccelerate)
		m_acceleration.x = maxAccelerate;
	else if (m_acceleration.x < -maxAccelerate)
		m_acceleration.x = -maxAccelerate;
	if (m_acceleration.y > 0)
		m_acceleration.y = 0;
		
	// Update position
	// Gravity
	float bottomDistance{ collisionHandler.distanceTillBottomCollision(getHitbox()) };
	if (bottomDistance == 0)
	{
		m_acceleration.y += gravity * 2 * elapsedTime;
		m_sprite.move(0, gravity * elapsedTime);

		m_canJump = false;
		bottomDistance = collisionHandler.distanceTillBottomCollision(getHitbox());
	}
	if (bottomDistance > 0)
	{
		m_sprite.move(0, -bottomDistance);
		m_canJump = true;
	}

	if (m_acceleration.y < 0)
	{
		movement.y = m_acceleration.y;
		m_sprite.move(0, movement.y * elapsedTime);

		const float upperDistance{ collisionHandler.distanceTillUpperCollision(getHitbox()) };
		if (upperDistance > 0)
		{
			m_sprite.move(0, upperDistance);
			if (m_acceleration.y < 0)
				m_acceleration.y = -gravity;
		}
	}

	if (m_isCrouching)
	{

	}

	if (m_acceleration.x > 0)
	{
		movement.x = speed * m_acceleration.x;
		m_sprite.move(movement.x * elapsedTime, 0);

		const float rightDistance{ collisionHandler.distanceTillRightCollision(getHitbox()) };
		if (rightDistance > 0)
		{
			m_sprite.move(-rightDistance, 0);
			m_acceleration.x = 0;
		}
	}

	if (m_acceleration.x < 0)
	{
		movement.x = speed * m_acceleration.x;
		m_sprite.move(movement.x * elapsedTime, 0);

		const float leftDistance{ collisionHandler.distanceTillLeftCollision(getHitbox()) };
		if (leftDistance > 0)
		{
			m_sprite.move(leftDistance, 0);
			m_acceleration.x = 0;
		}
	}

	// Updating accelerations to decelerate
	if (m_moveDirection.x == 0)
	{
		if (m_acceleration.x > 0)
		{
			m_acceleration.x -= decelerate;
			if (m_acceleration.x < 0)
				m_acceleration.x = 0;
		}
		else if (m_acceleration.x < 0)
		{
			m_acceleration.x += decelerate;
			if (m_acceleration.x > 0)
				m_acceleration.x = 0;
		}
	}
}

void Sprite::update2(const float elapsedTime, CollisionHandler & collisionHandler)
{
	const float
		tileSize{ 32 }, 
		gravity{ 8* tileSize },					// Blocks to drop in 1 second
		// To explain the math down here refer to my portfolio website
		jumpStrength							
	{ sqrt(2.f * gravity * tileSize * 3.f)},	// Amount of blocks to jump in one second
		jumpOffset{ 1 * tileSize },				// jumpStrength / jumpOffset = extra jump heigth blocks
		speed{ 3 * tileSize },					// Blocks to run with accel = 1

		maxAccel{ 2 },	// Max accel  
		accel{ 4 },	// accel / maxAccel = time to reach max accel
		decel{ 4 };	// decel / maxAccel = time to reach 0 accel

	// Jump
	if (m_moveDirection.y == -1)
	{
		if (!m_hasJumped)
		{
			m_hasJumped = true;
			m_acceleration.y = -jumpStrength;
		}
		/*
		else if (m_acceleration.y < 0)
			m_acceleration.y -= (jumpStrength / jumpOffset) * elapsedTime;
		*/
	}
	

	// Gravity
	if (collisionHandler.distanceTillBottomCollision(getHitbox()) == 0)
	{
		m_acceleration.y += gravity * elapsedTime;
		m_hasJumped = true;
	}

	m_sprite.move(0, m_acceleration.y * elapsedTime);

	if (m_acceleration.y > 0 && collisionHandler.distanceTillBottomCollision(getHitbox()) != 0)
	{
		m_sprite.move(0, -collisionHandler.distanceTillBottomCollision(getHitbox()));
		m_acceleration.y = 0;
		m_hasJumped = false;
	}
	else if (m_acceleration.y < 0 && collisionHandler.distanceTillUpperCollision(getHitbox()) != 0)
	{
		m_sprite.move(0, collisionHandler.distanceTillUpperCollision(getHitbox()));
		m_acceleration.y = gravity;
	}

	// Horizontal movement
	if (m_moveDirection.x == 1)
	{
		if (m_acceleration.x < 0)
			m_acceleration.x = 0;
		m_acceleration.x += accel * elapsedTime;
		if (m_acceleration.x > maxAccel)
			m_acceleration.x = maxAccel;
	}

	else if (m_moveDirection.x == -1)
	{
		if (m_acceleration.x > 0)
			m_acceleration.x = 0;
		m_acceleration.x -= accel * elapsedTime;
		if (m_acceleration.x < -maxAccel)
			m_acceleration.x = -maxAccel;
	}
	m_sprite.move(m_acceleration.x * speed * elapsedTime,0);

	// Collision resolution
	if (m_acceleration.x > 0 && collisionHandler.distanceTillRightCollision(getHitbox()) != 0)
	{
		m_sprite.move(-collisionHandler.distanceTillRightCollision(getHitbox()), 0);
		m_acceleration.x = 0;
	}
	else if (m_acceleration.x < 0 && collisionHandler.distanceTillLeftCollision(getHitbox()) != 0)
	{
		m_sprite.move(collisionHandler.distanceTillLeftCollision(getHitbox()), 0);
		m_acceleration.x = 0;
	}

	// Decrease accelerations
	if (m_moveDirection.x == 0)
	{
		if (m_acceleration.x > 0)
		{
			m_acceleration.x -= decel * elapsedTime;
			if (m_acceleration.x < 0)
				m_acceleration.x = 0;
		}
		else if (m_acceleration.x < 0)
		{
			m_acceleration.x += decel * elapsedTime;
			if (m_acceleration.x > 0)
				m_acceleration.x = 0;
		}
	}
}

void Sprite::update3(const float elapsedTime, CollisionHandler & collisionHandler)
{
	const float
		tileSize	{ 32 },
		gravity		{ 1 * tileSize },	// Blocks to drop in 1 second
		jumpStrength{ 2 * tileSize },	// Amount of blocks to jump in one second
		jumpOffset	{ 1 * tileSize },	// jumpStrength / jumpOffset = extra jump heigth blocks
		speed		{ 3 * tileSize },	// Blocks to run with accel = 1
		
		maxAccel	{ 2 },	// Max accel  
		accel		{ 4 },	// accel / maxAccel = time to reach max accel
		decel		{ 4 };	// decel / maxAccel = time to reach 0 accel

	// Update accelerations
	// Horizontal
	if (m_moveDirection.x == 1)
	{
		if (m_acceleration.x < 0)
			m_acceleration.x = 0;
		m_acceleration.x += accel * elapsedTime;
		if (m_acceleration.x > maxAccel)
			m_acceleration.x = maxAccel;
	}
	else if (m_moveDirection.x == -1)
	{
		if (m_acceleration.x > 0)
			m_acceleration.x = 0;
		m_acceleration.x -= accel * elapsedTime;
		if (m_acceleration.x < -maxAccel)
			m_acceleration.x = -maxAccel;
	}

	// Jump
	if (m_moveDirection.y == -1)
	{
		if (m_canJump)
		{
			m_acceleration.y = -jumpStrength;
			m_canJump = false;
		}
		/*
		else if (m_acceleration.y < 0)
			m_acceleration.y -= (jumpStrength / jumpOffset) * elapsedTime;
		*/
	}

	// Update position
	// The reason why we call this one first, is because this way gravity will only
	// affect the value when accel reaches under 0. This should fix the bug
	// regarding the player having an 'boost' effect when touching the edge of a tile.
	m_sprite.move(0, m_acceleration.y * elapsedTime);

	// Check jump collision
	if (m_acceleration.y < 0) 
	{
		if (collisionHandler.distanceTillUpperCollision(getHitbox()) > 0)
		{
			m_sprite.move(0, collisionHandler.distanceTillUpperCollision(getHitbox()));
			if (m_acceleration.y < 0)
				m_acceleration.y = gravity;
		}
	}
	
	// Update gravity value
	if (collisionHandler.distanceTillBottomCollision(getHitbox()) == 0) 
	{
		m_acceleration.y += gravity * elapsedTime;
		m_canJump = false;
	}
	
	// Check lower collision
	if (collisionHandler.distanceTillBottomCollision(getHitbox()) > 0) 
	{
		m_sprite.move(0, -collisionHandler.distanceTillBottomCollision(getHitbox()));
		m_acceleration.y = 0;
		m_canJump = true;
	}

	if (m_isCrouching)
	{
		// TODO
	}

	if (m_acceleration.x > 0)
	{
		m_sprite.move(m_acceleration.x * speed * elapsedTime, 0);

		const float rightDistance{ collisionHandler.distanceTillRightCollision(getHitbox()) };
		if (rightDistance > 0)
		{
			m_sprite.move(-rightDistance, 0);
			m_acceleration.x = 0;
		}
	}

	if (m_acceleration.x < 0)
	{
		m_sprite.move(m_acceleration.x * speed * elapsedTime, 0);

		const float leftDistance{ collisionHandler.distanceTillLeftCollision(getHitbox()) };
		if (leftDistance > 0)
		{
			m_sprite.move(leftDistance, 0);
			m_acceleration.x = 0;
		}
	}

	// Decelerate
	if (m_moveDirection.x == 0)
	{
		if (m_acceleration.x > 0)
		{
			m_acceleration.x -= decel * elapsedTime;
			if (m_acceleration.x < 0)
				m_acceleration.x = 0;
		}
		else if (m_acceleration.x < 0)
		{
			m_acceleration.x += decel * elapsedTime;
			if (m_acceleration.x > 0)
				m_acceleration.x = 0;
		}
	}
}

void Sprite::draw(sf::RenderWindow & window)
{
	window.draw(m_sprite);
}

void Sprite::debugMove(const float elapsedTime)
{
	const float speed{ 32 * 4 };
	m_canJump = true;
	if (m_moveDirection.x == 1)
	{
		m_acceleration.x = 2;
		m_sprite.move(m_acceleration.x * elapsedTime * speed,0);
	}
	else if (m_moveDirection.x == -1)
	{
		m_acceleration.x = -2;
		m_sprite.move(m_acceleration.x * elapsedTime* speed, 0);

	}
	else m_acceleration.x = 0;
	if (m_moveDirection.y == -1)
	{
		m_acceleration.y = -2;
		m_sprite.move(0, m_acceleration.y * elapsedTime* speed);

	}
	else if (m_isCrouching == true)
	{
		m_acceleration.y = 2;
		m_sprite.move(0, m_acceleration.y * elapsedTime* speed);
	}
	else m_acceleration.y = 0;
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

const sf::Vector2i Sprite::getMoveDirection() const
{
	sf::Vector2i moveDirection{ m_moveDirection };
	if (m_acceleration.y < 0)
		moveDirection.y = 1;
	if (m_isCrouching)
		moveDirection.y = -1;
	return moveDirection;
}
