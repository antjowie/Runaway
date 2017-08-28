#include "PlayerObject.h"
#include "Config.h"
#include "DataManager.h"

bool const PlayerObject::Player::isItemPressed(const std::string itemString) const
{
	// This looks totally odd
	return 
	((Config::getInstance().getConfig(itemString).itemType == Item::ItemType::Keyboard && sf::Keyboard::isKeyPressed(Config::getInstance().getConfig(itemString).keyboard)) ||
	(Config::getInstance().getConfig(itemString).itemType == Item::ItemType::Mouse && sf::Mouse::isButtonPressed(Config::getInstance().getConfig(itemString).mouse))) 
	== true ? true : false;
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
	const float gravity{ 0.5 };
	const float maxHorizonSpeed{ 32 * 8};
	const float maxVerticSpeed{ 32* 10 };

	m_movement.y *= gravity;
	if (m_movement.x > 1.0f)
		m_movement.x = 1.0f;
	else if (m_movement.x < -1.0f)
		m_movement.x = -1.0f;

	m_player.move(maxHorizonSpeed*m_movement.x*elapsedTime, maxVerticSpeed*m_movement.y*elapsedTime);
	m_movement.x *= 0.5f;
	m_movement.y *= 0.5f;
	if (m_movement.x > -0.1f && m_movement.x < 0.1f)
		m_movement.x = 0;
	
}

void PlayerObject::Player::_input(sf::RenderWindow & window)
{
	// Arbitrary values. The value represents acceleration. 0.25 means that it will take 4 seconds to get max speed
	const float jumpSpeed{ -1 };
	const float walkSpeed{ 0.25 };

	if (isItemPressed("moveUp") || isItemPressed("jump"))
	{
		m_jumped = true;
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

sf::FloatRect const PlayerObject::Player::getHitBox() const
{
	return m_player.getGlobalBounds();
}

sf::Vector2f PlayerObject::Player::getPos()
{
	return sf::Vector2f(m_player.getGlobalBounds().left + m_player.getGlobalBounds().width / 2,
		m_player.getGlobalBounds().top + m_player.getGlobalBounds().height / 2);
}
