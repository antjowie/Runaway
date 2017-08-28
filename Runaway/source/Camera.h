//
// The camera class will manage the view ingame
//

#pragma once
#include <SFML\Graphics.hpp>

class Camera
{
private:
	sf::Vector2f m_target;
	sf::Vector2f m_bounds;
	sf::View m_view;

	float m_speed; // The value is how many seconds the camera will take to move to m_target
				   // if value is 0.5 it means 1/0.5 (2) second to move towards m_target
	void checkBounds();

public:
	Camera();
	Camera(const sf::FloatRect &view, const sf::Vector2f &bounds, const float speed);

	void moveTarget(sf::Vector2f &target);
	void moveView(sf::Vector2f &target);
	void setView(sf::Vector2f &target);

	void setSpeed(const float speed);
	void setBounds(const sf::Vector2f &bounds);
	void setViewSize(const sf::Vector2f &size);

	void update(const float elapsedTime);

	void draw(sf::RenderWindow &window) const;
	sf::IntRect const getTileBounds(const int tileX, const int tileY) const;
};

