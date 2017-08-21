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

sf::IntRect const Camera::getTileBounds(const int tileX, const int tileY) const
{
	// Get upperleft corner world coordinates of the view
	// Because float to int will always get the lowest value, we dont have to worry about a too small screen view
	float upperLeftX{ m_view.getCenter().x - m_view.getSize().x / 2 };
	float upperLeftY{ m_view.getCenter().y - m_view.getSize().y / 2 };
	
	// Change world coordinated to tile coordinates
	sf::Vector2i upperLeft(static_cast<int>(upperLeftX), static_cast<int>(upperLeftY));
	upperLeft.x /= tileX;
	upperLeft.y /= tileY;

	// Get the width and height in tile coordinates.
	// The logic behind +2 goes as follows.
	// If upperleft.x is casted from 2.5 to 2, width would be 1 to small.
	// If width is casted from 720 / 32 (22.5) to 22, width would be 1 to small.
	// We could throw these into an if statement but the cost is too small to consider (I think).
	int width{ static_cast<int>(m_view.getSize().x) / tileX + 2 };
	int height{ static_cast<int>(m_view.getSize().y) / tileY + 2 };
	
	// If the values go out of the level bounds, invalid memory will be read
	while (width + upperLeft.x > m_bounds.x /tileX)
		--width;
	while (height + upperLeft.y > m_bounds.y /tileY)
		--height;

	return sf::IntRect(upperLeft.x, upperLeft.y, width, height);
}