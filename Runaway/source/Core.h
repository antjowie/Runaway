#pragma once
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Rect.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <vector>
#include <list>

#include "AnimationHandler.h"
#include "PlayerObject.h"
#include "BossLauncher.h"

class Core:
	public sf::Drawable
{
private:
	enum Phase
	{
		Sleep,
		Roam,
		Charge,
		Destroyed,
		SwitchState
	}m_phase{ Phase::Sleep };

	AnimationHandler m_animHandler;
	

	// This class is a class for sprite, but it uses corrected hitboxes
	class Part : public sf::Drawable
	{
	public:
		struct Hitbox
		{
			const sf::Vector2f offset;
			sf::IntRect hitbox;
			
			// Offset from origin
			Hitbox(const float offsetX, const float offsetY, const float width, const float height);
		};

	private:
		std::vector<Hitbox> m_hitbox;
		sf::Sprite m_sprite;
		bool m_hit{ false };

		float m_timeline{ 0 };

		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

	public:
		// Wrapper functions
		void setTexture(const sf::Texture &texture);
		void setTextureRect(const sf::IntRect &rect);
		void setOrigin(const float x, const float y);
		void setPos(const sf::Vector2f &pos);
		void setPos(const float x, const float y);
		void setColor(const sf::Color &color);
		const sf::Color &getColor() const;
		const sf::Vector2f &getPos() const;

		// Removes collided projectiles
		void updateCollision(std::list<Projectile> &projectiles);
		void pushHitbox(const Hitbox &&hitbox);

		// Used for red flash effect
		void update(const float elapsedTime);

		// This function resets hit value to false when called
		bool isHit();
		// Used to make sprite flash red
		void flash(const float elapsedTime = 0.5f);
	}m_body,m_eyes;
	
	BossLauncher m_leftLauncher,
			 m_rightLauncher,
			 m_middleLauncher;

	// -1 init phase
	// 0-1 duration
	float m_timeline{ -1 };
	
	// States
	// 3 1 launchers
	// 2 2 launchers
	// 1 3 launchers
	// 0 destoryed
	int m_state{ 3 };

	// Every time player hits the core, it looses hp
	int m_hp{ 10 };
	
	sf::Vector2f m_targetPos;
	// Used to predict next shot
	sf::Vector2f m_oldPlayerPos;
	sf::Vector2f m_playerPos;
	
	void DefaultLauncher();

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;
public:

	// This also removes collided objects, that's why we need projectile instead of just the hitbox
	void updateCollision(std::list<Projectile> &projectiles);
	void update(const float elapsedTime);

	void setPos(const sf::Vector2f &pos);
	void setPlayerPos(const sf::Vector2f &pos);

	const sf::Vector2f &getPos() const;

	// Checks if hitbox is hit by projectiles and removes them
	bool hit(const sf::FloatRect &hitbox);
	bool inDarkZone() const;
	Core();
};

