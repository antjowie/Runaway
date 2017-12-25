#pragma once
#include <SFML\Graphics.hpp>
#include "CollisionHandler.h"

class Sprite : public sf::Drawable
{
private:
	sf::Vector2i m_moveDirection;	// The direction the player walks in
	sf::Vector2f m_velocity;	// The acceleration of the player

	bool m_hitDash{ false };

	bool m_canJump{ false };

	const bool isItemPressed(const std::string string) const;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	float m_dashCooldown{ 0 };
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

	// Used for dash animation, returns value in pixels
	float getMaxSpeed() const;
	const sf::FloatRect getHitbox() const;
	const sf::IntRect getTextureRect() const;
	const sf::Vector2f getPos() const;
};
