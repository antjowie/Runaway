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

const float PlayerObject::Player::distanceTillBottomCollision(const std::vector<Tile*>& surroundingTiles) const
{
	// Get lower hitbox
	sf::FloatRect bottomHit{ m_player.getGlobalBounds() };
	bottomHit.top += bottomHit.height / 2;

	for (auto iter : surroundingTiles)
		if (iter->isSolid() && bottomHit.intersects(iter->getHitbox()))
			return bottomHit.top - iter->getHitbox().top;

	return 0.0f;
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
	m_movement(0,0)
{
	m_player.setTexture(DataManager::getInstance().getTexture("player"));
	m_player.setOrigin(m_player.getLocalBounds().width / 2, m_player.getLocalBounds().height / 2);
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
	if (m_movement.x > 1.0f)
		m_movement.x = 1.0f;
	else if (m_movement.x < -1.0f)
		m_movement.x = -1.0f;
	
	// Drop check
	if (m_isFloating)
		m_movement.y += gravity;
	else
	{
		m_movement.y = 0;
		m_jumped = false;
	}

	m_player.move(maxHorizonSpeed*m_movement.x*elapsedTime, maxVerticSpeed*m_movement.y*elapsedTime);
	m_movement.x *= slowDownSpeed;
	//m_movement.y *= slowDownSpeed;
	if (m_movement.x > -0.1f && m_movement.x < 0.1f)
		m_movement.x = 0;
	*/

	float speed{ 32*4 };

	sf::Vector2f movement;
	if (m_movement.x == 1)
		movement.x = speed;
	if (m_movement.x == -1)
		movement.x = -speed;

	/*
	if (verticalDistance > 0)
		movement.y += 5;
	if (m_movement.y == 1)
		movement.y = -32 * 3;
	*/

	if (m_movement.y == 1)
		movement.y = -speed;
	if (m_isCrouching)
		movement.y = speed;

	std::cout << bottomDistance << '\n';
	m_player.move(movement * elapsedTime);
	m_movement.x = m_movement.y = 0;
}

void PlayerObject::Player::_input(sf::RenderWindow & window)
{
	/*
	// Arbitrary values. The value represents acceleration. 0.25 means that it will take 4 seconds to get max speed
	const float jumpSpeed{ -1 };
	const float walkSpeed{ 0.25 };
	*/
	// Update jump value depending on being in air


	if ((isItemPressed("moveUp") || isItemPressed("jump")))// && verticalDistance == 0)
	{
		m_movement.y = 1; //jumpSpeed;
	}
	if (isItemPressed("moveLeft")) m_movement.x = -1;	//walkSpeed;
	if (isItemPressed("moveRight")) m_movement.x = 1;	//walkSpeed;
	m_isCrouching = isItemPressed("moveDown");
}

void PlayerObject::Player::_draw(sf::RenderWindow & window)
{
	window.draw(m_player);
}

void PlayerObject::Player::setPos(const sf::Vector2f & pos)
{
	m_player.setPosition(pos);
}

void PlayerObject::Player::updateCollisionDistance(const std::vector<std::vector<Tile*>>& tileMap)
{
	sf::Vector2i tileMapPlayerCoords{ mapWorldToTilemap(m_player.getPosition()) };

	std::vector<Tile*> surroundingTiles;
	for (int horizontal = tileMapPlayerCoords.x - 1; horizontal < tileMapPlayerCoords.x + 1; horizontal++)
	{
		for (int vertical = tileMapPlayerCoords.y - 1; vertical < tileMapPlayerCoords.y + 1; vertical++)
		{
			if (horizontal < tileMap.size() && vertical < tileMap[horizontal].size()) 
			{
				// Order shouldn't matter. If we do map this vector to tileMap vector. Our function calls would do 6 comparisons less each
				std::cout << tileMap[horizontal][vertical]->getHitbox().left << ' ' << tileMap[horizontal][vertical]->getHitbox().left  / 32 << ' ' << tileMap[horizontal][vertical]->getHitbox().top << ' ' << tileMap[horizontal][vertical]->getHitbox().top / 32 << "      ";
				surroundingTiles.push_back(tileMap[horizontal][vertical]);
			}
		}
		std::cout << '\n';
	}
	
	bottomDistance = distanceTillBottomCollision(surroundingTiles);
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