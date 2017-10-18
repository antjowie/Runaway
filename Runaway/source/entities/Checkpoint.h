#pragma once
#include "Entity.h"
#include "AnimationHandler.h"

class Checkpoint : public Entity
{
private:
	AnimationHandler m_animHandler;
public:

	Checkpoint(const EntityAction action, const sf::Vector2f &pos);
	
	virtual const EntityAction &getAction() final;

	virtual void logic(const float elapsedTime) final;
	virtual void input(sf::RenderWindow& window) final;
	virtual void draw(sf::RenderWindow &window) final;
};