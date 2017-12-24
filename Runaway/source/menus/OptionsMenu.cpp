#include "OptionsMenu.h"
#include "DataManager.h"
#include "BackgroundObject.h"
#include "TextObject.h"
#include "Config.h"

OptionsMenu::OptionsMenu(MenuStack * const menuStack):
	Menu(menuStack),
	m_music		(400,170,Config::getInstance().getConfig("music").integer,500),
	m_effects	(400,215,Config::getInstance().getConfig("effects").integer,500)
{
	std::vector<Object*> tempVec;
	tempVec.reserve(2);
	m_buttons.reserve(1);

	// Initialize background (temp)
	tempVec.push_back(new BackgroundObject("mainMenuBackground", true));

	// Init audio sliders and text
	TextObject * volume{ new TextObject("VOLUME",true) };
	volume->setTextSize(40);
	volume->setPos(sf::Vector2f(100, 84));

	TextObject * music{ new TextObject("MUSIC",true) };
	music->setPos(sf::Vector2f(100, 155));
	TextObject * effects{ new TextObject("EFFECTS",true) };
	effects->setPos(sf::Vector2f(100, 200));


	tempVec.push_back(volume);
	tempVec.push_back(music);
	tempVec.push_back(effects);


	// Initialize buttons
	m_buttons.push_back(new TextButtonObject("BACK", Function::Back, true));
	m_buttons[0]->setOriginToLeftMiddle();
	m_buttons[0]->setPos(sf::Vector2f(100, 630));
	m_buttons[0]->setBody(sf::IntRect(0, 600, 1280, 60));

	pushObject(tempVec);
}

OptionsMenu::~OptionsMenu()
{
	Menu::~Menu();
	for(auto iter : m_buttons)
		delete iter;
	m_buttons.clear();

	Config::getInstance().setConfig("music",Item(static_cast<int>(m_music.getProgress())));
	Config::getInstance().setConfig("effects", Item(static_cast<int>(m_effects.getProgress())));
	Config::getInstance().saveConfig();
}

void OptionsMenu::input(sf::RenderWindow & window)
{
	Menu::input(window);
	for (auto iter : m_buttons)
		if (iter->isValid()) iter->input(window);

	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::MouseButtonPressed:
				for (auto iter : m_buttons)
				{
					switch (iter->getFunction())
					{
					case Function::Back:
						m_isPop = true;
						break;
					}
				}
				break;

			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Key::Escape)
					m_isPop = true;
				break;
		}
	}

	// Allows user to hold mouse instead of having to tap for every step
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2f mousePos{ window.mapPixelToCoords(sf::Mouse::getPosition(window)) };
		if (m_music.getHitbox().contains(static_cast<sf::Vector2i>(mousePos)))
			m_music.setProgress(mousePos);
		else if (m_effects.getHitbox().contains(static_cast<sf::Vector2i>(mousePos)))
			m_effects.setProgress(mousePos);
	}
}

void OptionsMenu::update(const float elapsedTime)
{
	Menu::update(elapsedTime);
	for (auto iter : m_buttons)
		if (iter->isValid()) iter->logic(elapsedTime);
}

void OptionsMenu::draw(sf::RenderWindow & window)
{
	Menu::draw(window);
	for (const auto &iter : m_buttons)
		if (iter->isValid()) 
			window.draw(*iter);
	window.draw(m_music);
	window.draw(m_effects);
}

void Slider::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_rect, states);
	target.draw(m_leftBound, states);
	target.draw(m_rightBound, states);
	target.draw(m_bump, states);
	target.draw(m_text, states);
}

Slider::Slider(const float x, const float y, const float progress, const float maxSize):
	Slider(sf::Vector2f(x, y), progress, maxSize)
{
}

Slider::Slider(const sf::Vector2f & pos, const float progress, const float maxSize) :
	m_maxSize(maxSize),
	m_currentSize(maxSize * (progress / 100.f))
{
	m_rect.setSize(sf::Vector2f(maxSize, 4));
	m_rect.setPosition(pos);
	m_rect.setOrigin(0, m_rect.getLocalBounds().height / 2);

	m_bump.setSize(sf::Vector2f(4, 18));
	m_bump.setOrigin(m_bump.getLocalBounds().width / 2, m_bump.getLocalBounds().height / 2);
	m_bump.setPosition(m_rect.getPosition() + sf::Vector2f(m_currentSize, 0));

	m_leftBound.setSize(sf::Vector2f(4, 18));
	m_leftBound.setOrigin(m_leftBound.getLocalBounds().width / 2, m_leftBound.getLocalBounds().height / 2);
	m_leftBound.setPosition(m_rect.getPosition());
	
	m_rightBound.setSize(sf::Vector2f(4, 18));
	m_rightBound.setOrigin(m_rightBound.getLocalBounds().width / 2, m_rightBound.getLocalBounds().height / 2);
	m_rightBound.setPosition(m_rect.getPosition() + sf::Vector2f(m_maxSize, 0));

	m_text.setFont(DataManager::getInstance().getFont("pixel"));
	m_text.setCharacterSize(25);
	m_text.setString(std::to_string((int)progress));
	m_text.setOrigin(0, m_text.getLocalBounds().height / 2);
	m_text.setPosition(m_rightBound.getPosition() + sf::Vector2f(10, 0));
}

void Slider::setProgress(const sf::Vector2f & pos)
{
	// Only needs x coordinate but uses vector to make it easier to implement
	m_currentSize = pos.x - m_leftBound.getGlobalBounds().left;
	m_text.setString(std::to_string(static_cast<int>(getProgress())));
	m_bump.setPosition(m_currentSize + m_leftBound.getGlobalBounds().left, m_bump.getPosition().y);
}

const sf::IntRect Slider::getHitbox() const
{
	return sf::IntRect(m_leftBound.getGlobalBounds().left,m_leftBound.getGlobalBounds().top,m_rect.getGlobalBounds().width + m_rightBound.getGlobalBounds().width,m_leftBound.getGlobalBounds().height);
}

float Slider::getProgress() const
{
	return m_currentSize / m_maxSize* 100.f;
}
