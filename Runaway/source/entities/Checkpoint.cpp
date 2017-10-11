#include "Checkpoint.h"
#include "DataManager.h"

Checkpoint::Checkpoint(const EntityType type, const EntityAction action, const sf::Vector2f &pos):
	Entity(type,action,pos),
	m_animHandler(32,32)
{
	m_sprite.setTexture(DataManager::getInstance().getTexture("checkpoint"));

	m_animHandler.addAnimation(Animation(0, 4, 1.f / 8.f));
	m_animHandler.changeAnimation(0);
	
	m_sprite.setTextureRect(m_animHandler.getFrame());
}

const EntityAction & Checkpoint::getAction()
{
	m_isActive = true;
	return Entity::getAction();
}

void Checkpoint::logic(const float elapsedTime)
{
	if(m_isActive)
	m_animHandler.update(elapsedTime);
}

void Checkpoint::input(sf::RenderWindow & window)
{
}

void Checkpoint::draw(sf::RenderWindow & window)
{
	sf::IntRect correctedHitbox{ m_animHandler.getFrame()};
	
	// These are directly taken from the file

	correctedHitbox.left += 9;
	correctedHitbox.top = 3;
	correctedHitbox.width = 13;
	correctedHitbox.height = 26;

	m_sprite.setTextureRect(correctedHitbox);
	
	window.draw(m_sprite);
}
