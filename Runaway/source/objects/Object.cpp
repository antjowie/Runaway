#include "Object.h"

Object::Object(bool isValid):
	m_isValid(isValid)
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