#pragma once
#include "Object.h"
#include "AnimationHandler.h"
#include "CollisionHandler.h"
#include "Tile.h"
#include "LightPool.h"
#include "Launcher.h"
#include "PlayerSprite.h"
#include "LightTrail.h"
#include "SoundManager.h"


class PlayerObject :
	public Object
{
private:
	float m_darknessTimeline{ 0 };
	enum PlayerDirection { Rest, JumpRight, DropRight, JumpLeft, DropLeft, Right, Left ,DashRight,DashLeft};
	AnimationHandler m_animHandler;
	
	float m_soundTimeline{ 0 };
	SoundObject * m_lightPoolSound;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	bool m_isDead{ false };
	LightPool m_lightPool;					// Public for level access
	Launcher m_launcher;					// Public for level access
	CollisionHandler m_collisionHandler;	// This one is public so that level can initialize it
	Sprite m_sprite;						// This one is public so that camera can access it
	LightTrail m_trail;						// Public for light objects

	PlayerObject(SoundManager &soundManager, const bool isValid = false);
	~PlayerObject();

	virtual void logic(const float elapsedTime);
	virtual void input(sf::RenderWindow &window);
};