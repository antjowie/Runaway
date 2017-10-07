#pragma once
#include "Object.h"
#include "AnimationHandler.h"
#include "CollisionHandler.h"
#include "Tile.h"

const sf::Vector2i mapWorldToTilemap(const sf::Vector2f &coords, sf::Vector2i &tileSize);
const sf::Vector2i mapWorldToTilemap(const sf::Vector2f &coords, const int tileWidth, const int tileHeight);

class PlayerObject :
	public Object
{
private:
	class Player
	{
	private:
		// Sprite related
		sf::Sprite m_player;
		sf::Vector2i m_moveDirection;
		sf::Vector2f m_acceleration;
		bool m_isCrouching{ false };
		bool m_hasJumped{ false };

		// Collision related
		CollisionHandler m_collisionHandler;

		// Animation related
		AnimationHandler m_animHandler;
		enum playerDirection{Rest,JumpRight,DropRight,JumpLeft,DropLeft,Right,Left};

		// Should this be a normal or member function
		// No added benefit making it normal function, it's only used for player
		const bool isItemPressed(const std::string string) const; 

	public:
		Player();

		virtual void _logic(const float elapsedTime);
		virtual void _input(sf::RenderWindow &window);
		virtual void _draw(sf::RenderWindow &window);

		void setPos(const sf::Vector2f &pos);

		sf::FloatRect const getHitbox() const;
		sf::Vector2f const getPos() const;
		CollisionHandler &getCollisionHandler(); // Only used for initLevel
	};

public:
	Player m_player;

	PlayerObject(const bool isValid = false);

	virtual void logic(const float elapsedTime);
	virtual void input(sf::RenderWindow &window);
	virtual void draw(sf::RenderWindow &window);
};

