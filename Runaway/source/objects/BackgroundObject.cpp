#include "BackgroundObject.h"
#include "DataManager.h"

void BackgroundObject::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_background, states);
}

BackgroundObject::BackgroundObject(const std::string & texture, bool isValid):
	Object(isValid)
{
	m_background.setTexture(DataManager::getInstance().getTexture(texture));
}