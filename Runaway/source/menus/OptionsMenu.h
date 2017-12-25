//
// Class for the options menu
//

#pragma once
#include "Menu.h"
#include "TextButtonObject.h"

#include "SoundManager.h"
#include "SoundObject.h"

// Volume sliders
class Slider : public sf::Drawable
{
private:
	sf::RectangleShape m_leftBound;
	sf::RectangleShape m_rightBound;
	sf::RectangleShape m_bump;

	sf::RectangleShape m_rect;
	sf::Text m_text;

	float m_maxSize;
	float m_currentSize;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	// Assumes middle position, not left-top
	Slider(const float x, const float y, const float progress, const float maxSize);
	Slider(const sf::Vector2f &pos, const float progress, const float maxSize);

	// When player clicks on the slider, only call when hitbox collision, else undefined behavior
	void setProgress(const sf::Vector2f &pos);

	const sf::IntRect getHitbox() const;
	// Returns a percentage
	float getProgress() const;
};

union Key
{
	sf::Keyboard::Key key;
	sf::Mouse::Button button;
	
	Key(sf::Keyboard::Key key);
	Key(sf::Mouse::Button button);
};

enum class KeyType
{
	Keyboard,
	Mouse
};

class ConfigKey : public sf::Drawable
{
private:
	Key m_key;
	KeyType m_keyType;

	sf::Text m_name;
	sf::Text m_text;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	bool selected;
	void setKey(const Key &key, const KeyType keyType);
	
	void update(const float elapsedTime);

	const Key getKey() const;
	const KeyType getKeyType() const;
	const sf::IntRect getHitbox() const;

	ConfigKey(const sf::Vector2f pos, const std::string name, const Key &key, const KeyType keyType);
	ConfigKey(const float x, const float y, const std::string name, const Key &key, const KeyType keyType);
};

class OptionsMenu :
	public Menu
{
private:
	std::vector<TextButtonObject*> m_buttons;

	Slider m_music;
	Slider m_effects;

	std::vector<ConfigKey> m_keys;
	
	void initKeys();

	SoundManager m_soundManager;
	SoundObject * m_sound;
public:
	OptionsMenu(MenuStack* const menuStack);
	virtual ~OptionsMenu() override final;

	virtual void input(sf::RenderWindow &window);
	virtual void update(const float elapsedTime);
	virtual void draw(sf::RenderWindow &window);
};

