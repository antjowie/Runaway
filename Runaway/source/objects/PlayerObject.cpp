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

const bool PlayerObject::Player::checkCollision(const std::vector<std::vector<Tile*>> tiles) const
{
	sf::FloatRect globalPlayerPos{ m_player.getGlobalBounds() };

	// The magic values are the offset, this way the area around the player will be checked
	for (int column = m_tileCoords.top - 1; column < m_tileCoords.top + m_tileCoords.height + 1; ++column)
	{
		for (int row = m_tileCoords.left - 1; row < m_tileCoords.left + m_tileCoords.width + 1; ++row)
		{
			if (tiles[column][row]->getType() != tileType::Air && tiles[column][row]->getHitbox().intersects(globalPlayerPos))
			{
				return true;
			}
		}
	}
	return false;
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
	// Gravity will be determined by the level. This is a temporary value
	const float gravity{ 0.05f };
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
	{
		m_movement.y += gravity;
		std::cout << "tru\n";
	}
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

	// Map world to tile coords
	m_tileCoords = (sf::IntRect)m_player.getGlobalBounds();

	m_tileCoords.left /= m_tileWidth;
	m_tileCoords.width /= m_tileWidth;
	m_tileCoords.top /= m_tileHeight;
	m_tileCoords.height /= m_tileHeight;
	if (m_tileCoords.width == 0) m_tileCoords.width = 1;
	if (m_tileCoords.height == 0) m_tileCoords.height = 1;
}

void PlayerObject::Player::_input(sf::RenderWindow & window)
{
	// Arbitrary values. The value represents acceleration. 0.25 means that it will take 4 seconds to get max speed
	const float jumpSpeed{ -1 };
	const float walkSpeed{ 0.25 };

	if ((isItemPressed("moveUp") || isItemPressed("jump")) && !m_jumped)
	{
		m_jumped = true;
		m_isFloating = true;
   		m_movement.y = jumpSpeed;
	}
	if (isItemPressed("moveLeft")) m_movement.x -= walkSpeed;
	if (isItemPressed("moveRight")) m_movement.x += walkSpeed;
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

void PlayerObject::Player::setTileSize(const sf::Vector2i & tileSize)
{
	m_tileWidth = tileSize.x;
	m_tileHeight = tileSize.y;
}

void PlayerObject::Player::isDropping(const std::vector<std::vector<Tile*>> tiles)
{
	
	m_isFloating = !checkCollision(tiles);
	// This function is safer but does unnecessary checking
	/*
	sf::FloatRect playerHitbox{ m_player.getGlobalBounds() };
	playerHitbox.height += 1; // So that player won't fall through the block
	m_isFloating = true;

	for (auto const &column: tiles)
	{
		for (auto const &row : column)
		{
			if (row->getType() != tileType::Air && row->getHitbox().intersects(playerHitbox))
			{
				m_isFloating = false;
				break;
			}
		}
	}
	*/
}

sf::FloatRect const PlayerObject::Player::getHitBox() const
{
	return m_player.getGlobalBounds();
}

sf::Vector2f PlayerObject::Player::getPos()
{
	return sf::Vector2f(m_player.getGlobalBounds().left + m_player.getGlobalBounds().width / 2,
		m_player.getGlobalBounds().top + m_player.getGlobalBounds().height / 2);
}

sf::Vector2f PlayerObject::Player::getMovement()
{
	return m_movement;
}