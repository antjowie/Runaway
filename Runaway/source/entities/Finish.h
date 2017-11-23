#pragma once
#include "Entity.h"
#include "AnimationHandler.h"

class Finish :
	public Entity
{
private:
	AnimationHandler m_animHandler;

public:
	Finish(const EntityAction action, const sf::Vector2f &pos);
	
	virtual void logic(const float elapsedTime) override final;
};