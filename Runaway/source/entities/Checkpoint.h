#pragma once
#include "Entity.h"
#include "AnimationHandler.h"

class Checkpoint : public Entity
{
private:
	AnimationHandler m_animHandler;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:

	Checkpoint(const EntityAction action, const sf::Vector2f &pos);
	
	virtual const EntityAction &getAction() final;

	virtual void logic(const float elapsedTime) final;
};