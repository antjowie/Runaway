//
// Class for the options menu
//

#pragma once
#include "Menu.h"
#include "TextButtonObject.h"

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

class OptionsMenu :
	public Menu
{
private:
	std::vector<TextButtonObject*> m_buttons;

	Slider m_music;
	Slider m_effects;

public:
	OptionsMenu(MenuStack* const menuStack);
	virtual ~OptionsMenu() override final;

	virtual void input(sf::RenderWindow &window);
	virtual void update(const float elapsedTime);
	virtual void draw(sf::RenderWindow &window);
};

