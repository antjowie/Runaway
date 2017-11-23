#include "Finish.h"
#include "DataManager.h"

Finish::Finish(const EntityAction action, const sf::Vector2f &pos):
	Entity(EntityType::Finish,action,pos),
	m_animHandler(32,32)
{
	m_sprite.setTexture(DataManager::getInstance().getTexture("finish"));
	m_animHandler.addAnimation(Animation(0, 19, 0.10f));
	m_animHandler.changeAnimation(0);
	m_sprite.setTextureRect(m_animHandler.getFrame());
}

void Finish::logic(const float elapsedTime)
{
	m_animHandler.update(elapsedTime);
	m_sprite.setTextureRect(m_animHandler.getFrame());
}