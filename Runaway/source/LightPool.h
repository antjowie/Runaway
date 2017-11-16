//
// This class is used for a light pool
// Light pools are the lifeline of a entity
// The pool value represents the diameter in pixels

#pragma once
#include <SFML\Graphics\Sprite.hpp>

class LightPool : public sf::Drawable
{
	sf::Sprite m_sprite;
	float m_cap;	// Max light
	float m_pool;	// Amount of light
	float m_rate;	// Change rate of light

	void setSize(const float width, const float height);
	void setSize(const float diameter);

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	LightPool();

	void update(const float elapsedTime);
	void setPos(const sf::Vector2f &pos);

	// Defines at which rate the light should change
	void setRate(const float depletetion);
	const bool isCapped() const;
	const bool isEmpty() const;
};