#include "Camera.h"
#include <iostream>

Camera::Camera()
{
	m_speed = 0;
	m_view = sf::View();
	m_target = m_view.getCenter();
}

Camera::Camera(const sf::FloatRect &view, const float speed) :
	m_view(view)
{
	m_speed = speed;
	if (m_speed > 1.0f) m_speed = 1.0f;
	if (m_speed < 0.0f) m_speed = 0.0f;
	m_target = m_view.getCenter(); // To prevent undefined error
}

void Camera::setView(const sf::Vector2f &target)
{
	m_target = target;
	m_view.setCenter(target);
}

void Camera::moveView(const sf::Vector2f &target)
{
	m_target = target;
}

void Camera::setViewSize(const sf::Vector2f & size)
{
	m_view.setSize(size);
}

void Camera::update(const float elapsedTime)
{
	float x{ m_target.x - m_view.getCenter().x };
	float y{ m_target.y - m_view.getCenter().y };

	if (x * x + y * y <= 1)
	{
		setView(m_target);
	}
	else
	{
		float d{ sqrt(x * x + y * y) };
		float v = d * m_speed * elapsedTime;
		if (v < 1.0f) v = 1.0f;

		x *= (v / d); // If our speed decreases by 4 times
		y *= (v / d); // these will also decrease by 4 times

		m_view.move(x, y);
	}
}

sf::View const Camera::getView() const
{
	return m_view;
}

sf::IntRect const Camera::getTileBounds() const
{
	// Not finished
	return sf::IntRect();
}
