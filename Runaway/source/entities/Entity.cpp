#include "Entity.h"



void Entity::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

Entity::Entity(const EntityType type, const EntityAction action, const sf::Vector2f &pos):
	m_type(type),
	m_action(action)
{
	m_sprite.setPosition(pos);
}

void Entity::logic(const float elapsedTime)
{
}

void Entity::input(sf::RenderWindow & window)
{
}

const EntityAction & Entity::getAction()
{
	return m_action;
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