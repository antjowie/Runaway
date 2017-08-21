#include "Camera.h"

void Camera::checkBounds()
{
	if (m_view.getCenter().x - m_view.getSize().x / 2 < 0)
	{
		m_view.setCenter(sf::Vector2f(m_view.getSize().x / 2, m_view.getCenter().y));
		m_target.x = m_view.getCenter().x;
	}
	else if (m_view.getCenter().x + m_view.getSize().x / 2 > m_bounds.x)
	{
		m_view.setCenter(sf::Vector2f(m_bounds.x - m_view.getSize().x / 2, m_view.getCenter().y));
		m_target.x = m_view.getCenter().x;
	}

	if (m_view.getCenter().y - m_view.getSize().y / 2 < 0)
	{
		m_view.setCenter(sf::Vector2f(m_view.getCenter().x, m_view.getSize().y / 2));
		m_target.y = m_view.getCenter().y;
	}
	else if (m_view.getCenter().y + m_view.getSize().y / 2 > m_bounds.y)
	{
		m_view.setCenter(sf::Vector2f(m_view.getCenter().x, m_bounds.y - m_view.getSize().y / 2));
		m_target.y = m_view.getCenter().y;
	}
}

Camera::Camera()
{
	m_speed = 0;
	m_view = sf::View();
	m_target = m_view.getCenter();
}

Camera::Camera(const sf::FloatRect &view, const sf::Vector2f &bounds, const float speed) :
	m_view(view), m_bounds(bounds),m_speed(speed)
{
	if (m_speed > 1.0f) m_speed = 1.0f;
	if (m_speed < 0.0f) m_speed = 0.0f;
	m_target = m_view.getCenter(); // To prevent undefined error
}

void Camera::setView(sf::Vector2f &target)
{
	
	m_target = target;
	m_view.setCenter(target);
}

void Camera::moveView(sf::Vector2f &target)
{
	m_target = target;
}

void Camera::moveTarget(sf::Vector2f & target)
{
	m_target += target;
}

void Camera::setSpeed(const float speed)
{
	m_speed = speed;
}

void Camera::setBounds(const sf::Vector2f & bounds)
{
	m_bounds = bounds;
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
	checkBounds();
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