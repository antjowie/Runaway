//
// Entities are all in game related items, like coins, checkpoints
//

#pragma once
#include "Object.h"

enum class EntityType
{
	Coin,
	Checkpoint,
	Switch
};

union EntityAction{
	int				value;
	sf::Vector2f	pos;

	EntityAction();
};

class Entity : public sf::Drawable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
	
protected:
	EntityType m_type;
	EntityAction m_action;
	sf::Sprite m_sprite;

public:
	bool m_isActive{ false };

	Entity(const EntityType type, const EntityAction action, const sf::Vector2f &pos);

	virtual void logic(const float elapsedTime);
	virtual void input(sf::RenderWindow& window);

	virtual const EntityAction &getAction();
	const sf::FloatRect getHitbox();
	const EntityType &getType();
};