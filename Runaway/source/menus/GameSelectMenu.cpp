#include "GameSelectMenu.h"
#include "DataManager.h"
#include "MenuStack.h"
#include "BackgroundObject.h"
#include <iostream>

std::string GameSelectMenu::getLevelName(const LevelName levelName) const
{
	switch (levelName)
	{
	case LevelName::Test:
		return "Test";
		break;
	case LevelName::That:
		return "That";
		break;
	default:
		return "ERROR";
		break;
	}
}

sf::Texture & GameSelectMenu::getLevelBackground(const LevelName levelName) const
{
	std::string temp(getLevelName(levelName));
	std::transform(temp.begin(), temp.end(), temp.begin(), tolower);
	return DataManager::getInstance().getTexture(temp + "Background");
}

GameSelectMenu::GameSelectMenu(MenuStack* const menuStack):
	Menu(menuStack, "Runaway - level select")
{
	pushObject(new BackgroundObject("mainMenuBackground", true));

	for (int j = 1; j < 4; ++j)
		for(int  i = 1; i < 5; ++i)
		{
			const float width{ 200 };
			const float height{ 90 };

			int index{ (j - 1) * 4 + (i - 1) };

			LevelButton &temp = m_levels[index];
			
			if (index >= static_cast<int>(LevelName::Blank))
				return;

			temp.m_level = static_cast<LevelName>(index);
			
			temp.m_rect.setTexture(&getLevelBackground(temp.m_level));
			temp.m_rect.setPosition(i * width + 25, j * height);
			temp.m_rect.setSize(sf::Vector2f(width - 50,height));
			temp.m_rect.setOutlineThickness(5);
			temp.m_rect.setOutlineColor(sf::Color::White);

			temp.m_unlocked.setSize(temp.m_rect.getSize());
			temp.m_unlocked.setPosition(temp.m_rect.getPosition());
			temp.m_unlocked.setFillColor(sf::Color(60, 60, 60));

			temp.m_text.setFont(DataManager::getInstance().getFont("pixel"));
			temp.m_text.setString(getLevelName(temp.m_level));
			temp.m_text.setPosition(temp.m_rect.getPosition().x, temp.m_rect.getPosition().y + temp.m_rect.getSize().y - temp.m_text.getCharacterSize());
		}
}

void GameSelectMenu::input(sf::RenderWindow& window)
{
	Menu::input(window);
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::MouseMoved:
			for (auto &iter : m_levels)
			{
				if ((iter.m_level <= m_currentLevel))
					iter.m_hover = iter.m_rect.getGlobalBounds().contains(window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y))) ? true : false;
			}
			break;
			
		case sf::Event::MouseButtonPressed:
			for (auto &iter : m_levels)
				if(iter.m_rect.getGlobalBounds().contains(window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x,event.mouseButton.y))) && iter.m_level <= m_currentLevel)
					m_menuStack->push(new GameMenu(m_menuStack,iter.m_level,m_currentLevel));
			break;

		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Key::Escape)
				m_isPop = true;
			break;

		default:
			break;
		}

	}
}

void GameSelectMenu::update(const float elapsedTime)
{
	const int min{ 20 };
	const int max{ 200 };
	const float fadeTime{ 1 };

	Menu::update(elapsedTime);
	for (auto &iter : m_levels)
	{
		int temp{ iter.m_rect.getOutlineColor().a };
		iter.m_hover ? temp += max * (elapsedTime / fadeTime) : temp -= max * (elapsedTime / fadeTime);

		if (temp > max)
			temp = max;
		if (temp < min)
			temp = min;

		sf::Color color{ iter.m_rect.getOutlineColor() };
		color.a = static_cast<sf::Uint8>(temp);

		iter.m_rect.setOutlineColor(color);
	}
}

void GameSelectMenu::draw(sf::RenderWindow &window)
{
	window.setView(window.getDefaultView());
	Menu::draw(window);
	std::cout << getLevelName(m_currentLevel) << '\n';
	for (const auto &iter : m_levels)
	{
		window.draw(iter.m_rect);
		window.draw(iter.m_text);
		if (!(iter.m_level <= m_currentLevel))
			window.draw(iter.m_unlocked);
	}

}