#include "OptionsMenu.h"
#include "DataManager.h"
#include "BackgroundObject.h"
#include "TextObject.h"
#include "Config.h"
#include "InputNames.h"

void OptionsMenu::initKeys()
{
	m_keys.clear();
	m_keys.push_back(ConfigKey(100, 370, "LEFT", Config::getInstance().getConfig("moveLeft").keyboard, KeyType::Keyboard));
	m_keys.push_back(ConfigKey(100, 420, "RIGHT", Config::getInstance().getConfig("moveRight").keyboard, KeyType::Keyboard));
	m_keys.push_back(ConfigKey(100,470,"DASH",Config::getInstance().getConfig("dash").keyboard,KeyType::Keyboard));
	m_keys.push_back(ConfigKey(100,520,"JUMP",Config::getInstance().getConfig("jump").keyboard,KeyType::Keyboard));
	m_keys.push_back(ConfigKey(550,370,"SHOOT",Config::getInstance().getConfig("shoot").mouse,KeyType::Mouse));
	m_keys.push_back(ConfigKey(550,420,"RESPAWN",Config::getInstance().getConfig("respawn").keyboard,KeyType::Keyboard));
}

OptionsMenu::OptionsMenu(MenuStack * const menuStack) :
	Menu(menuStack),
	m_music(400, 170, Config::getInstance().getConfig("music").integer, 500),
	m_effects(400, 215, Config::getInstance().getConfig("effects").integer, 500)
{
	initKeys();
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
	m_buttons[0]->setBody(sf::IntRect(0, 600, 1280/2, 60));

	// Reset button
	TextButtonObject* reset = new TextButtonObject("RESET GAME", Function::Options, true);
	reset->setOriginToRightMiddle();
	reset->setPos(sf::Vector2f(1280- 100 , 630));
	reset->setBody(sf::IntRect(1280/2, 600, 1280 / 2, 60));
	m_buttons.push_back(reset);

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
	
	// Embrace for ugly code
	Config::getInstance().setConfig("moveLeft",(m_keys[0].getKey().key));
	Config::getInstance().setConfig("moveRight", (m_keys[1].getKey().key));
	Config::getInstance().setConfig("dash", (m_keys[2].getKey().key));
	Config::getInstance().setConfig("jump", (m_keys[3].getKey().key));
	Config::getInstance().setConfig("shoot", (m_keys[4].getKey().button));
	Config::getInstance().setConfig("respawn", (m_keys[5].getKey().key));
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
					case Function::Options:
						Config::getInstance().loadDefaultConfig();
						initKeys();
						break;
					}
				}				
				for (auto &iter : m_keys)
				{
					if (iter.selected && iter.getKeyType() == KeyType::Mouse)
						iter.setKey(event.key.code, KeyType::Mouse);
					iter.selected = false;
					if (iter.getHitbox().contains(sf::Vector2i(window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)))))
						iter.selected = true;
				}
				break;

			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Key::Escape)
					m_isPop = true;
				for (auto &iter : m_keys)
					if (iter.selected && iter.getKeyType() == KeyType::Keyboard)
					{
						iter.setKey(event.key.code, KeyType::Keyboard);
						iter.selected = false;
					}
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
	for (auto &iter : m_keys)
		iter.update(elapsedTime);
}

void OptionsMenu::draw(sf::RenderWindow & window)
{
	Menu::draw(window);
	for (const auto &iter : m_buttons)
		if (iter->isValid()) 
			window.draw(*iter);
	window.draw(m_music);
	window.draw(m_effects);

	for(const auto &iter:m_keys)
		window.draw(iter);
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

void ConfigKey::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_name, states);
	target.draw(m_text, states);
}

void ConfigKey::setKey(const Key & key, const KeyType keyType)
{
	m_keyType = keyType;
	std::string trans;
	if (m_keyType == KeyType::Keyboard)
	{
		m_key.key = key.key;
		trans = toString(m_key.key);
	}
	if (m_keyType == KeyType::Mouse)
	{
		m_key.button = key.button;
		trans = toString(m_key.button);
	}
	std::transform(trans.begin(), trans.end(), trans.begin(), toupper);
	m_text.setString(trans);
}

void ConfigKey::update(const float elapsedTime)
{
	sf::Color newColor(selected ? sf::Color::Red : sf::Color::White);
	m_name.setFillColor(newColor);
	m_text.setFillColor(newColor);
}

const Key ConfigKey::getKey() const
{
	return m_key;
}

const KeyType ConfigKey::getKeyType() const
{
	return m_keyType;
}

const sf::IntRect ConfigKey::getHitbox() const
{
	return sf::IntRect(m_name.getGlobalBounds().left,m_name.getGlobalBounds().top,m_name.getGlobalBounds().width + m_text.getGlobalBounds().width,m_name.getGlobalBounds().height);
}

ConfigKey::ConfigKey(const sf::Vector2f pos, const std::string name, const Key & key, const KeyType keyType):
	ConfigKey(pos.x,pos.y,name,key,keyType)
{
}

ConfigKey::ConfigKey(const float x, const float y, const std::string name, const Key & key, const KeyType keyType):
	m_keyType(keyType),
	m_key(key),
	selected(false)
{
	m_name.setFont(DataManager::getInstance().getFont("pixel"));
	m_text.setFont(DataManager::getInstance().getFont("pixel"));

	m_name.setString(std::string(name + ":  "));
	m_name.setPosition(x, y);
	
	if (m_keyType == KeyType::Keyboard)
		m_text.setString(toString(m_key.key));
	if (m_keyType == KeyType::Mouse)
		m_text.setString(toString(m_key.button));
	std::string trans{ m_text.getString() };
	std::transform(trans.begin(), trans.end(), trans.begin(), toupper);
	m_text.setString(trans);
	m_text.setPosition(x + m_name.getGlobalBounds().width, y); 
}

Key::Key(sf::Keyboard::Key key):
	key(key)
{
}

Key::Key(sf::Mouse::Button button):
	button(button)
{
}
