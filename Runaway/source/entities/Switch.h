#pragma once
#include "Entity.h"
#include "AnimationHandler.h"

class Switch :
	public Entity
{
private:
	AnimationHandler m_animHandler;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	Switch(const EntityAction action, const sf::Vector2f &pos);

	virtual void logic(const float elapsedTime)	final;
};

