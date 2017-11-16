#pragma once
#include "Object.h"
#include "AnimationHandler.h"
#include "CollisionHandler.h"
#include "Tile.h"
#include "LightPool.h"

class Sprite: public sf::Drawable
{
private:
	sf::Vector2i m_moveDirection;	// The direction the player walks in
	sf::Vector2f m_velocity;	// The acceleration of the player


	bool m_hitDash{ false };
	float m_dashCooldown{ 0 };

	bool m_isCrouching{ false };
	bool m_canJump{ false };

	const bool isItemPressed(const std::string string) const;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	sf::Sprite m_sprite;

	bool m_hasJumped{ true };	// This value is public for the animation class
								// If the player is standing on a gate or lift tile, it will not
								// keep switcing animations

	Sprite();

	void input();
	void update(const float elapsedTime, CollisionHandler &collisionHandler);

	void debugMove(const float elapsedTime);

	void setPos(const sf::Vector2f &pos);
	void setTextureRect(const sf::IntRect &textureRect);
	void setTexture(const sf::Texture &texture);
	void fixOrigin();

	const sf::FloatRect getHitbox() const;
	const sf::IntRect getTextureRect() const;
	const sf::Vector2f getPos() const;
	const sf::Vector2i getMoveDirection() const;
};

class PlayerObject :
	public Object
{
private:
	enum PlayerDirection { Rest, JumpRight, DropRight, JumpLeft, DropLeft, Right, Left };
	AnimationHandler m_animHandler;
	
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	bool m_isDead{ false };
	LightPool m_lightPool;					// Public for level access
	CollisionHandler m_collisionHandler;	// This one is public so that level can initialize it
	Sprite	m_sprite;						// This one is public so that camera can access it

	PlayerObject(const bool isValid = false);

	virtual void logic(const float elapsedTime);
	virtual void input(sf::RenderWindow &window);
};