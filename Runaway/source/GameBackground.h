#pragma once
#include <SFML\Graphics.hpp>

class GameBackground
{
private:
	sf::Sprite m_foreground;
	sf::RectangleShape m_background;
	sf::RectangleShape m_deadBackground;
	sf::RectangleShape m_darkBackground;
	sf::RectangleShape m_darkOverlay;
	float m_deadAlpha{ 0 };
	float m_darkAlpha{ 0 };
	bool m_inDarkZone{ false };

	sf::Vector2f m_originalPos;
	sf::Vector2f m_target;
	float m_colorChangeRate;

public:
	GameBackground(const std::string &levelPath, const float colorChangeRate = 1.f);
	GameBackground();

	void update(const float elapsedTime);
	void draw(sf::RenderWindow &window);
	void drawOverlay(sf::RenderWindow & window);

	void died();
	void setDarkZone(const bool isInDarkZone);
	void setTarget(const sf::Vector2f &target);
	void init(const sf::Vector2f &size);
};