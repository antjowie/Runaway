#pragma once
#include "Object.h"
#include "AnimationHandler.h"
#include "CollisionHandler.h"
#include "Tile.h"

class Sprite
{
private:
	sf::Sprite m_sprite;
	sf::Vector2i m_moveDirection;	// The direction the player walks in
	sf::Vector2f m_velocity;	// The acceleration of the player

	bool m_isCrouching{ false };
	bool m_hasJumped{ true };

	const bool isItemPressed(const std::string string) const;
	const bool isFloating(CollisionHandler &collisionHandler) const;

public:

	Sprite();

	void input();
	void update(const float elapsedTime, CollisionHandler &collisionHandler);
	void draw(sf::RenderWindow &window);

	void debugMove(const float elapsedTime);

	void setPos(const sf::Vector2f &pos);
	void setTextureRect(const sf::IntRect &textureRect);
	void setTexture(const sf::Texture &texture);

	const sf::FloatRect getHitbox() const;
	const sf::IntRect getTextureRect() const;
	const sf::Vector2f getPos() const;
	const sf::Vector2i getMoveDirection() const;
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