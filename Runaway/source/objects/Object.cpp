#include "Object.h"

Object::Object(bool isValid):
	m_isValid(isValid)
{
}

void Object::logic(const float elapsedTime)
{
}

void Object::input(sf::RenderWindow & window)
{
}

bool const Object::isValid() const
{
	return m_isValid;
}

void Object::setValid(const bool isValid)
{
	m_isValid = isValid;
}