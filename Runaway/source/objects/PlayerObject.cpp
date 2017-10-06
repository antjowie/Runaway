#include "PlayerObject.h"
#include "Config.h"
#include "DataManager.h"
#include "Tile.h"

#include <iostream>

bool const PlayerObject::Player::isItemPressed(const std::string itemString) const
{
	// This looks totally odd
	return 
	((Config::getInstance().getConfig(itemString).itemType == Item::ItemType::Keyboard && sf::Keyboard::isKeyPressed(Config::getInstance().getConfig(itemString).keyboard)) ||
	(Config::getInstance().getConfig(itemString).itemType == Item::ItemType::Mouse && sf::Mouse::isButtonPressed(Config::getInstance().getConfig(itemString).mouse))) 
	== true ? true : false;
}

void PlayerObject::Player::distanceTillBottomCollision()
{
	// Get lower hitbox
	sf::FloatRect bottomHit{ m_player.getGlobalBounds() };
	m_bottomDistance = bottomHit.height * 3; // To get the closest block to the player

	for (auto iter : m_surroundingTiles) 
	{
		float distance = abs(bottomHit.top + bottomHit.height - iter->getHitbox().top);
		if (iter->isSolid() && bottomHit.intersects(iter->getHitbox()) && distance < m_bottomDistance)
			m_bottomDistance = distance;
	}

	if(m_bottomDistance == bottomHit.height*3)
	m_bottomDistance = 0.0f;
}

void PlayerObject::Player::distanceTillUpperCollision()
{	
	// Get upper hitbox	
	sf::FloatRect upperHit{ m_player.getGlobalBounds() };
	m_upperDistance = upperHit.height * 3; // To get the closest block to the player

	for (auto iter : m_surroundingTiles) 
	{
		float distance = abs(upperHit.top - (iter->getHitbox().top + iter->getHitbox().height));
		if (iter->isSolid() && upperHit.intersects(iter->getHitbox()) && distance < m_upperDistance)
			m_upperDistance = distance;
	}

	if (m_upperDistance == upperHit.height * 3)
	m_upperDistance = 0.0f;
}

void PlayerObject::Player::distanceTillLeftCollision()
{
	// Get left collision
	sf::FloatRect leftHit{ m_player.getGlobalBounds() };
	m_leftDistance = leftHit.width * 3; // To get the closest block to the player

	for (auto iter : m_surroundingTiles) 
	{
		float distance = abs(leftHit.left - (iter->getHitbox().left + iter->getHitbox().width));
		if (iter->isSolid() && leftHit.intersects(iter->getHitbox()) && distance < m_leftDistance)
			m_leftDistance = distance;
	}

	if (m_leftDistance == leftHit.width * 3)
	m_leftDistance = 0.0f;
}

void PlayerObject::Player::distanceTillRightCollision()
{	
	// Get right collision
	sf::FloatRect rightHit{ m_player.getGlobalBounds() };
	m_rightDistance = rightHit.width * 3; // To get the closest block to the player

	for (auto iter : m_surroundingTiles)
	{
		float distance = abs((rightHit.left + rightHit.width) - iter->getHitbox().left);
		if (iter->isSolid() && rightHit.intersects(iter->getHitbox()) && distance < m_rightDistance)
			m_rightDistance = distance;
	}

	if (m_rightDistance == rightHit.width * 3)
		m_rightDistance = 0.0f;
}

const sf::Vector2i PlayerObject::Player::mapWorldToTilemap(const sf::Vector2f &coords) const
{
	sf::Vector2i tempVec{ coords };

	// Value mapped to int will always be correct. Since int will round down
	tempVec.x /= m_tileWidth;
	tempVec.y /= m_tileHeight;
	return tempVec;
}

PlayerObject::PlayerObject(const bool isValid) :
	Object(isValid)
{
}

void PlayerObject::logic(const float elapsedTime)
{
	m_player._logic(elapsedTime);
}

void PlayerObject::input(sf::RenderWindow &window)
{	
	m_player._input(window);
}

void PlayerObject::draw(sf::RenderWindow &window)
{
	m_player._draw(window);
}

PlayerObject::Player::Player():
	m_acceleration(0,0),
	m_animHandler(32,32)
{
	m_player.setTexture(DataManager::getInstance().getTexture("player"));

	// Animation related
	// Rest
	m_animHandler.addAnimation(Animation(0, 3, 0.75f, true,false));
	for (int i = 0; i < 4; i++)
		// Jump up right, drop down right, jump up left, drop down left
		m_animHandler.addAnimation(Animation(0, 3, 0.1f, false,true));
	for (int i = 0; i < 2; i++)
		// Walk right, walk left
		m_animHandler.addAnimation(Animation(0, 3, 0.2f, true,false));
	
	// Otherwise player is initialized with wrong hitbox which makes him teleport when spawned
	m_animHandler.changeAnimation(1);
	m_player.setTextureRect(m_animHandler.getFrame());
}

void PlayerObject::Player::_logic(const float elapsedTime)
{
	/*
	// Gravity will be determined by the level. This is a temporary value
	const float gravity{ 0.005f };
	const float maxHorizonSpeed{ 32 * 15};
	const float maxVerticSpeed{ 32* 32};
	const float slowDownSpeed{ 0.5f };		// This value represent an acceleraion, 0.5 means it wil take 2 frames to slow down (wait isn't this useless)

	// Horizontal movement bound check
	if (m_acceleration.x > 1.0f)
		m_acceleration.x = 1.0f;
	else if (m_acceleration.x < -1.0f)
		m_acceleration.x = -1.0f;
	
	// Drop check
	if (m_isFloating)
		m_acceleration.y += gravity;
	else
	{
		m_acceleration.y = 0;
		m_jumped = false;
	}

	m_player.move(maxHorizonSpeed*m_acceleration.x*elapsedTime, maxVerticSpeed*m_acceleration.y*elapsedTime);
	m_acceleration.x *= slowDownSpeed;
	//m_acceleration.y *= slowDownSpeed;
	if (m_acceleration.x > -0.1f && m_acceleration.x < 0.1f)
		m_acceleration.x = 0;
	*/	


	// All these values could be defined by level, but I won't because I have no intention to
	float speed{ 32*3 };					// Distance with acceleration 1
	float jumpStrength{ 32 * 20 };
	float maxAccelerate{ 2 };				
	float accelerate{ elapsedTime * 4 };    // How much time to reach that acceleration
	float decelerate{ elapsedTime * 6 };	// How much times slower in one sec
	// TODO: level should define this
	float gravity{ 32 * 8 };				// Drop this many pixels in one sec

	m_animHandler.update(elapsedTime);

	sf::Vector2f movement;
	sf::Vector2f oldPos{ getPos() };

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
		m_acceleration.x -= accelerate;;
	}
	if (m_moveDirection.y == -1)
	{
		m_moveDirection.y = 0;
		m_acceleration.y = -jumpStrength;
	}

	// Check movement bounds
	if (m_acceleration.x > maxAccelerate)
		m_acceleration.x = maxAccelerate;
	else if (m_acceleration.x < -maxAccelerate)
		m_acceleration.x = -maxAccelerate;

	// Update position
	if (m_isCrouching)
	{
		m_acceleration.x = m_moveDirection.x = 0;
	}

	if (m_acceleration.x > 0)
	{
		movement.x = speed * m_acceleration.x;
		m_player.move(movement.x * elapsedTime, 0);

		loadSurroundingTiles();
		distanceTillRightCollision();
		if (m_rightDistance> 0)
			m_player.move(-m_rightDistance, 0);
	}
	if (m_acceleration.x < 0)
	{
		movement.x = speed * m_acceleration.x;
		m_player.move(movement.x * elapsedTime,0);

		loadSurroundingTiles();
		distanceTillLeftCollision();
		if (m_leftDistance > 0)
			m_player.move(m_leftDistance, 0);
	}

	if (m_acceleration.y < 0)
	{
		movement.y = m_acceleration.y; // jumpStrength
		m_player.move(0, movement.y * elapsedTime);

		loadSurroundingTiles();
		distanceTillUpperCollision();
		if (m_upperDistance > 0)
			m_player.move(0, m_upperDistance);	
	}

	// Gravity
	loadSurroundingTiles();
	distanceTillBottomCollision();
	if (m_bottomDistance == 0)
	{
		m_acceleration.y += gravity * 2 * elapsedTime;
		m_player.move(0, gravity * elapsedTime);
		distanceTillBottomCollision();
	}
	if (m_bottomDistance > 0)
	{
		m_player.move(0, -m_bottomDistance);
		m_hasJumped = false;
	}

	// Updating accelerations to decelerate
	if (m_moveDirection.x == 0 )
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
	m_moveDirection.x = m_moveDirection.y = 0;

	sf::Vector2f newPos{ getPos() - oldPos };

	// Brace for some ugly vector checking
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

void PlayerObject::Player::_input(sf::RenderWindow & window)
{
	if ((isItemPressed("moveUp") || isItemPressed("jump")) && !m_hasJumped)
	{
		m_moveDirection.y = -1;	
		m_hasJumped = true;
	}
	
	if (isItemPressed("moveLeft")) m_moveDirection.x -= 1;
	if (m_moveDirection.x < -1) m_moveDirection.x = -1;
	if (isItemPressed("moveRight")) m_moveDirection.x += 1;
	if (m_moveDirection.x > 1) m_moveDirection.x = 1;

	m_isCrouching = isItemPressed("moveDown");
}

void PlayerObject::Player::_draw(sf::RenderWindow & window)
{
	m_player.setTextureRect(m_animHandler.getFrame()); // Should this be handled in logic or draw?
	m_player.setTextureRect(sf::IntRect(m_player.getTextureRect().left + m_player.getTextureRect().width / 4,
		m_player.getTextureRect().top, m_player.getTextureRect().width / 2, m_player.getTextureRect().height));
	window.draw(m_player);
}

void PlayerObject::Player::loadTilemap(std::vector<std::vector<Tile*>>* const tilemap)
{
	m_tilemap = tilemap;
}

void PlayerObject::Player::setPos(const sf::Vector2f & pos)
{
	m_player.setPosition(pos);
}

void PlayerObject::Player::loadSurroundingTiles()
{
	sf::Vector2i tilemapPlayerCoords{ mapWorldToTilemap(m_player.getPosition()) };
	const int offset{ 2 };

	m_surroundingTiles.clear();
	for (int horizontal = tilemapPlayerCoords.x - offset; horizontal <= tilemapPlayerCoords.x + offset; horizontal++)
	{
		for (int vertical = tilemapPlayerCoords.y - offset; vertical <= tilemapPlayerCoords.y + offset; vertical++)
		{
			if ((unsigned int)horizontal < m_tilemap->size() && (unsigned int)vertical < (*m_tilemap)[horizontal].size()) 
			{
				// Order shouldn't matter. If we do map this vector to tilemap vector. Our function calls would do 6 comparisons less each
				// Our first vector is y, the subsequent vectors are x.
				m_surroundingTiles.push_back((*m_tilemap)[vertical][horizontal]);
			}	
		}
	}
}

void PlayerObject::Player::setTileSize(const sf::Vector2i & tileSize)
{
	m_tileWidth = tileSize.x;
	m_tileHeight = tileSize.y;
}

sf::FloatRect const PlayerObject::Player::getHitbox() const
{
	return m_player.getGlobalBounds();
}

sf::Vector2f const PlayerObject::Player::getPos() const
{
	return m_player.getPosition();
}