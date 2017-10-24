#pragma once
#include <SFML\Graphics.hpp>

class GameBackground
{
private:
	sf::Sprite m_foreground;
	sf::RectangleShape m_background;
	sf::RectangleShape m_deadBackground;
	float m_alpha{ 0 };

	sf::Vector2f m_target;
	float m_colorChangeRate;

public:
	GameBackground(const std::string &levelPath, const float colorChangeRate = 1.f);
	GameBackground();

	void update(const float elapsedTime);
	void draw(sf::RenderWindow &window);

	// 0-255
	void setAlpha(const sf::Uint8 &alpha = 255);
	void setTarget(const sf::Vector2f &target);
};