#include "Switch.h"
#include "DataManager.h"

Switch::Switch(const EntityAction action, const sf::Vector2f &pos):
	Entity(EntityType::Switch,action,pos), m_animHandler(32,32)
{
	m_sprite.setTexture(DataManager::getInstance().getTexture("switch"));

	m_animHandler.addAnimation(Animation(0, 3, 0.1f));
	m_animHandler.addAnimation(Animation(0, 0, 1));

	m_animHandler.changeAnimation(1);
	m_sprite.setTextureRect(m_animHandler.getFrame());
}

void Switch::logic(const float elapsedTime)
{
	m_animHandler.update(elapsedTime);
	// Update texture
	if (m_isActive)
		m_animHandler.changeAnimation(0);
	else
		m_animHandler.changeAnimation(1);

	// Fix texture positioning (not tuned yet)
	sf::IntRect corrected{ m_animHandler.getFrame() };

	corrected.left += 2;
	corrected.width -= 4;
	m_sprite.setTextureRect(corrected);
}

void Switch::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_sprite,states);
}
