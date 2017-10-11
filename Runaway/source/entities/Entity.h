//
// Entities are all in game related items, like coins, checkpoints
//

#pragma once
#include "Object.h"

enum class EntityType
{
	Coin,
	Checkpoint
};

union EntityAction{
	int				value;
	sf::Vector2f	pos;

	EntityAction();
};

class Entity :
	public Object
{
protected:
	EntityType m_type;
	EntityAction m_action;
	sf::Sprite m_sprite;

public:
	bool m_isActive{ false };

	Entity(const EntityType type, const EntityAction action, const sf::Vector2f &pos);

	virtual void logic(const float elapsedTime) override =0;
	virtual void input(sf::RenderWindow& window) override =0;
	virtual void draw(sf::RenderWindow &window)	override =0;

	virtual const EntityAction &getAction();
	Entity * getEntity();
	const sf::FloatRect getHitbox();
	const EntityType &getType();
};