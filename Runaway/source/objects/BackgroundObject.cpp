#include "BackgroundObject.h"
#include "DataManager.h"

BackgroundObject::BackgroundObject(const std::string & texture, bool isValid):
	Object(isValid),m_background(texture)
{
}

void BackgroundObject::draw(sf::RenderWindow & window)
{
	m_background._draw(window);
}

void BackgroundObject::logic(const float elapsedTime)
{
}

void BackgroundObject::input(sf::RenderWindow & window)
{
}

BackgroundObject::Background::Background(const std::string & texture)
{
	m_background.setTexture(DataManager::getInstance().getTexture(texture));
}

void BackgroundObject::Background::_draw(sf::RenderWindow &window)
{
	window.draw(m_background);
}
