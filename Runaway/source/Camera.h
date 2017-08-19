//
// The camera class will manage the view ingame
//

#pragma once
#include <SFML\Graphics.hpp>

class Camera
{
private:
	sf::Vector2f m_target;
	sf::View m_view;

	float m_speed;	// The value is how many seconds the camera will take to move to m_target
					// if value is 0.5 it means 1/0.5 (2) second to move towards m_target

public:
	Camera();
	Camera(const sf::FloatRect &view, const float speed);

	void setView(const sf::Vector2f &target);
	void moveView(const sf::Vector2f &target);
	void moveTarget(const sf::Vector2f &target);
	void setSpeed(const float speed);

	void setViewSize(const sf::Vector2f &size);

	void update(const float elapsedTime);

	sf::View const getView() const;
	sf::IntRect const getTileBounds() const;
};

