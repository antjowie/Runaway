#include "MainMenu.h"
#include "DataManager.h"
#include <iostream>

MainMenu::MainMenu(MenuStack *const menuStack) :
	Menu(menuStack)
{
	m_objects.push_back(new BackgroundObject());

}

void MainMenu::input(sf::RenderWindow & window)
{
	sf::Event event;
	while (window.pollEvent(event)) 
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		default:
			break;
		}
	}
}

BackgroundObject::BackgroundObject()
{
	m_background.setTexture(DataManager::getInstance().getTexture("mainMenuBackground"));
	m_isValid = true;
}

void BackgroundObject::draw(sf::RenderWindow & window)
{
	window.draw(m_background);
}

void BackgroundObject::logic(const sf::Time & elapsedTime)
{
}

void BackgroundObject::input(sf::RenderWindow & window)
{
}

TextButtonObject::TextButtonObject()
{
	m_text.setFont(DataManager::getInstance().getFont("pixel"));
	m_isValid = true;
}

void TextButtonObject::draw(sf::RenderWindow & window)
{
	window.draw(m_bodyTexture);
	window.draw(m_text);
}

void TextButtonObject::logic(const sf::Time & elapsedTime)
{
	const int fadeIn = 10;
	const int fadeOut = 5;
	const int maxFade = 100;

	m_fade -= fadeOut;

	if (hover)
		m_fade += fadeIn;
	
	// Bounds check
	if (m_fade > maxFade)
		m_fade = maxFade;
	else if (m_fade <= 0)
		m_fade = 0;

	m_bodyTexture.setFillColor(sf::Color(255, 0, 0, static_cast<sf::Uint8>(m_fade)));
}

void TextButtonObject::input(sf::RenderWindow & window)
{
	sf::Mouse mouse;
	if (m_body.contains(mouse.getPosition(window)))
		hover = true;
	else
		hover = false;
}

void TextButtonObject::setPosition(const sf::Vector2i pos)
{
	m_body.left = pos.x;
	m_body.top = pos.y;
}

void TextButtonObject::setBody(const sf::Rect<int> body)
{
	m_body = body;
	m_bodyTexture.setPosition(sf::Vector2f(static_cast<float>(m_body.left), static_cast<float>(m_body.top)));
	m_bodyTexture.setSize(sf::Vector2f(static_cast<float>(m_body.width), static_cast<float>(m_body.height)));
}

void TextButtonObject::setText(const std::string text)
{
	m_text.setString(text);
}