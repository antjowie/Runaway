//
// Optional collectible, unlocks special last level
//

#pragma once
#include "Entity.h"
#include "AnimationHandler.h"

class Coin :
	public Entity
{
private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;
	AnimationHandler m_animHandler;

public:
	Coin(const EntityAction action, const sf::Vector2f &pos);
	
	virtual void logic(const float elapsedTime) override final;
	
	// Used for boss fight
	void setPosition(const sf::Vector2f & pos);

	// This value doesn't matter
	virtual const EntityAction &getAction() override final;
};

