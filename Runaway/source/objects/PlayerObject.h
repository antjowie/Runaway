#pragma once
#include "Object.h"
#include "AnimationHandler.h"
#include "CollisionHandler.h"
#include "Tile.h"

class Sprite
{
private:
	sf::Sprite m_sprite;
	sf::Vector2i m_moveDirection;
	sf::Vector2f m_acceleration;
	bool m_isCrouching{ false };
	bool m_canJump{ false };
	// Should this be a normal or member function
	// No added benefit making it normal function, it's only used for player
	const bool isItemPressed(const std::string string) const;
	const bool isFloating(CollisionHandler &collisionHandler) const;

public:
	Sprite();

	void input();
	void update(const float elapsedTime, CollisionHandler &collisionHandler);
	void draw(sf::RenderWindow &window);

	void setPos(const sf::Vector2f &pos);
	void setTextureRect(const sf::IntRect &textureRect);
	void setTexture(const sf::Texture &texture);

	const sf::FloatRect getHitbox() const;
	const sf::IntRect getTextureRect() const;
	const sf::Vector2f getPos() const;
};

class PlayerObject :
	public Object
{
private:
	enum playerDirection{Rest,JumpRight,DropRight,JumpLeft,DropLeft,Right,Left};
	AnimationHandler m_animHandler;

public:
	bool m_isDead{ false };
	CollisionHandler m_collisionHandler; // This one is public so that level can initialize it
	Sprite m_sprite; // This one is public so that camera can access it

	PlayerObject(const bool isValid = false);

	virtual void logic(const float elapsedTime);
	virtual void input(sf::RenderWindow &window);
	virtual void draw(sf::RenderWindow &window);
};