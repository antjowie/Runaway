#include "Entity.h"



Entity::Entity(const EntityType type, const EntityAction action, const sf::Vector2f &pos):
	Object(true),
	m_type(type),
	m_action(action)
{
	m_sprite.setPosition(pos);
}

const EntityAction & Entity::getAction()
{
	return m_action;
}

Entity * Entity::getEntity()
{
	return this;
}

const sf::FloatRect Entity::getHitbox()
{
	return m_sprite.getGlobalBounds();
}

const EntityType & Entity::getType()
{
	return m_type;
}

EntityAction::EntityAction()
{
}
