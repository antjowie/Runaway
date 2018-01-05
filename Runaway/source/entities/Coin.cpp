#include "Coin.h"
#include "Config.h"
#include "DataManager.h"

void Coin::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

Coin::Coin(const EntityAction action, const sf::Vector2f &pos) :
	Entity(EntityType::Coin, action, pos),
	m_animHandler(32, 32)
{
	m_sprite.setTexture(DataManager::getInstance().getTexture("coin"));

	if (Config::getInstance().getConfig(std::string("coin" + std::to_string(action.value))).logic == true)
		m_isActive = false;
	else
		m_isActive = true;

	m_animHandler.addAnimation(Animation(0, 25, 1.f / 12.f, true,false));
	m_animHandler.changeAnimation(0);
	m_sprite.setTextureRect(m_animHandler.getFrame());
}

void Coin::logic(const float elapsedTime)
{
	if (!m_isActive) return;
	m_animHandler.update(elapsedTime);
	
	m_sprite.setTextureRect(m_animHandler.getFrame());
}

void Coin::setPosition(const sf::Vector2f & pos)
{
	m_sprite.setPosition(pos);
}

const EntityAction & Coin::getAction()
{
	// Coin updates config self
	Config::getInstance().setConfig(std::string("coin" + std::to_string(m_action.value)), Item(true));
	
	m_sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
	m_isActive = false;

	return Entity::getAction();
}
