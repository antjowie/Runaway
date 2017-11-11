#pragma once
#include <SFML\Graphics.hpp>

class GameBackground : public sf::Drawable
{
private:
	sf::Sprite m_foreground;				// The structures which move
	sf::RectangleShape m_background;		// The background which can move
	sf::RectangleShape m_deadBackground;	// The red flash
	sf::RectangleShape m_darkBackground;	// The darkness effect
	sf::RectangleShape m_darkOverlay;		// Dark overlay over all tiles
	float m_deadAlpha{ 0 };
	float m_darkAlpha{ 0 };
	float m_overlayAlpha{ 0 };

	sf::Vector2f m_originalPos;
	sf::Vector2f m_target;
	float m_colorChangeRate;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	GameBackground(const std::string &levelPath, const float colorChangeRate = 1.f);
	GameBackground();

	void update(const float elapsedTime);

	void died();
	void setTarget(const sf::Vector2f &target);
	void init(const sf::Vector2f &size);

	const sf::RectangleShape &getDarkOverlay() const;
};