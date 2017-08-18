#include "BackgroundObject.h"
#include "DataManager.h"

BackgroundObject::BackgroundObject(const bool isValid) :
	Object(isValid)
{
}

BackgroundObject::BackgroundObject(const std::string & texture, bool isValid):
	Object(isValid)
{
	setTexture(texture);
}

void BackgroundObject::draw(sf::RenderWindow & window)
{
	window.draw(m_background);
}

void BackgroundObject::logic(const float elapsedTime)
{
}

void BackgroundObject::input(sf::RenderWindow & window)
{
}

void BackgroundObject::setTexture(const std::string & texture)
{
	m_background.setTexture(DataManager::getInstance().getTexture(texture));
}
