#include "PlayerObject.h"
#include "Config.h"
#include "DataManager.h"

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

	// If player decides to stand till on a gate
	if (m_collisionHandler.distanceTillBottomCollision(m_sprite.getHitbox()) != 0)
	{
		m_sprite.setPos(sf::Vector2f(m_sprite.getPos().x,m_sprite.getPos().y-m_collisionHandler.distanceTillBottomCollision(m_sprite.getHitbox())));
		if (m_collisionHandler.distanceTillUpperCollision(m_sprite.getHitbox()) != 0)
			m_isDead = true;
	}
	
	m_sprite.update(elapsedTime, m_collisionHandler);
	//m_sprite.debugMove(elapsedTime);
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
		m_sprite.getTextureRect().top, m_sprite.getTextureRect().width -32+14, m_sprite.getTextureRect().height));
	m_sprite.draw(window);
}

Sprite::Sprite():
	m_velocity(0,0),
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

void Sprite::update(const float elapsedTime, CollisionHandler & collisionHandler)
{
	// TODO: add jump offset

	const float
		tileSize{ 32 }, 
		speed{ 3.5  },	// Blocks to run with accel = 1
		gravity{ 24 },	// Blocks to drop in 1 second
		
		oneTileTravel{ 2.f * gravity },
		jumpStrength{ sqrt(oneTileTravel * 4) },	// Amount of blocks to jump in one second (won't go higher)
	
		maxAccel{ 2 },	// Max accel  
		accel{ 4 },		// accel / maxAccel = time to reach max accel
		decel{ 4 };		// decel / maxAccel = time to reach 0 accel

	// Jump
	if (m_moveDirection.y == -1)
	{
		if (!m_hasJumped)
		{
			m_hasJumped = true;
			m_velocity.y = -jumpStrength * tileSize;
		}
	}
	
	// This is placed in the middle because otherwise the desired jump height won't be reached
	m_sprite.move(0, m_velocity.y * elapsedTime);

	// Gravity
	if (collisionHandler.distanceTillBottomCollision(getHitbox()) == 0)
	{
		m_velocity.y += gravity * tileSize*  elapsedTime;
		m_hasJumped = true;
	}	

	if (m_velocity.y > 0 && collisionHandler.distanceTillBottomCollision(getHitbox()) != 0)
	{
		m_sprite.move(0, -collisionHandler.distanceTillBottomCollision(getHitbox()));
		m_velocity.y = 0;
		m_hasJumped = false;
	}
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
		m_velocity.x += accel * elapsedTime;
		if (m_velocity.x > maxAccel)
			m_velocity.x = maxAccel;
	}

	else if (m_moveDirection.x == -1)
	{
		if (m_velocity.x > 0)
			m_velocity.x = 0;
		m_velocity.x -= accel * elapsedTime;
		if (m_velocity.x < -maxAccel)
			m_velocity.x = -maxAccel;
	}
	m_sprite.move(m_velocity.x * speed * tileSize * elapsedTime,0);

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
		m_velocity.x = 2;
		m_sprite.move(m_velocity.x * elapsedTime * speed,0);
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
	else if (m_isCrouching == true)
	{
		m_velocity.y = 2;
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
	if (m_velocity.y < 0)
		moveDirection.y = 1;
	if (m_isCrouching)
		moveDirection.y = -1;
	return moveDirection;
}