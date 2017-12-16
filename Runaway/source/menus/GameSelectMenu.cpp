#include "GameSelectMenu.h"
#include "GameMenu.h"
#include "DataManager.h"
#include "MenuStack.h"
#include "BackgroundObject.h"
#include "Config.h"
#include "DiedMenu.h"

std::string getLevelName(const LevelName levelName)
{
	switch (levelName)
	{
	case LevelName::WakeUp:
		return "Wake Up";
		break;
	case LevelName::TheGate:
		return "The Gate";
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
	m_currentLevel = static_cast<LevelName>(Config::getInstance().getConfig("level").integer);
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
			float textpos{ temp.m_text.getGlobalBounds().width - temp.m_rect.getGlobalBounds().width };
			if (textpos < 0)
				textpos = 0;
			temp.m_text.setPosition(temp.m_rect.getPosition().x - textpos / 2.f, temp.m_rect.getPosition().y + temp.m_rect.getSize().y + temp.m_text.getCharacterSize() / 2);
		}
}

GameSelectMenu::~GameSelectMenu()
{
	// For some reason this doesn't get called
	Config::getInstance().setConfig("level", Item(static_cast<int>(m_currentLevel)));
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
				if (iter.m_rect.getGlobalBounds().contains(window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) && iter.m_level <= m_currentLevel)
					m_menuStack->push(new GameMenu(m_menuStack,iter.m_level,m_currentLevel));
			while (window.pollEvent(event));	// This line makes sure that player can't spam a level
			break;

		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Key::Escape)
			{
				Config::getInstance().setConfig("level", Item(static_cast<int>(m_currentLevel)));
				m_isPop = true;
			}
			break;
		}
	}
}

void GameSelectMenu::update(const float elapsedTime)
{
	const int min{ 20 };
	const int max{ 200 };
	const float fadeTime{ 0.5f };

	// Sometimes the user his fps is too high which results into the int not changing (the change is too small f.e. 20.0 goes to 20.7 and gets casted to int (20))
	m_timeline += elapsedTime;
	if (m_timeline < 1.f / 60.f)
		return;
	
	Menu::update(m_timeline);
	for (auto &iter : m_levels)
	{
		int temp{ iter.m_rect.getOutlineColor().a };
		int before{ temp };
		
		temp = iter.m_hover ? temp + max * (m_timeline /fadeTime): temp - max * (m_timeline /fadeTime);
		if (temp > max) 
			temp = max;
		if (temp < min)
			temp = min;

		sf::Color color{ iter.m_rect.getOutlineColor() };
		color.a = static_cast<sf::Uint8>(temp);
		iter.m_rect.setOutlineColor(color);
	}

	m_timeline = 0;
}

void GameSelectMenu::draw(sf::RenderWindow &window)
{
	window.setView(window.getDefaultView());
	Menu::draw(window);
	for (const auto &iter : m_levels)
	{
		window.draw(iter.m_rect);
		window.draw(iter.m_text);
		if (!(iter.m_level <= m_currentLevel))
			window.draw(iter.m_unlocked);
	}
}