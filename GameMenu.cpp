#include "GameMenu.h"
#include "DataManager.h"
#include "BackgroundObject.h"

GameMenu::GameMenu(MenuStack* const menuStack):
	Menu(menuStack), m_camera(sf::FloatRect(1280 / 2,720 / 2,1280,720),0.1f)
{
	m_objects.push_back(new BackgroundObject("mainMenuBackground",true));
}


GameMenu::~GameMenu()
{
}

void GameMenu::input(sf::RenderWindow & window)
{
	sf::Event event;
	sf::Vector2f center{ 1280 / 2,720 / 2 };
	
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;

		case sf::Event::KeyPressed:
			m_isPop = true; // Temp
			break;

		case sf::Event::MouseMoved:
			m_camera.moveView(sf::Vector2f(event.mouseMove.x - 1280 / 2 + center.x , event.mouseMove.y - 720/2 + center.y));
			break;

		default:
			break;
		}
	}
}

void GameMenu::update(const float elapsedTime)
{
	m_camera.update(elapsedTime);
}

void GameMenu::draw(sf::RenderWindow & window)
{
	sf::RectangleShape origin(sf::Vector2f(128, 72));
	origin.setPosition(1280 / 2 - 128 / 2, 720 / 2 - 72 / 2);
	origin.setFillColor(sf::Color::Black);


	Menu::draw(window);
	window.draw(origin);
	window.setView(m_camera.getView());
}
