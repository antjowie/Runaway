//
// This class is used for a light pool
// Light pools are the lifeline of a entity
// The pool value represents the diameter in pixels

#pragma once
#include <SFML\Graphics\Sprite.hpp>

class LightPool : public sf::Drawable
{
	sf::Sprite m_sprite;
	bool m_resizeEvent; // Used to overwrite size being equal to pool

	// 0 means not shown - 1 means shown
	float m_timeline;
	float m_fadeTime;

	sf::Vector2f m_sizeTarget;   // Used to make fluid transition

	float m_cap;	// Max light
	float m_pool;	// Amount of light
	float m_rate;	// Change rate of light

	void changeSize(const float elapsedTime);

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;
	
public:
	bool m_shouldAppear;

	LightPool();

	void update(const float elapsedTime);
	void setPos(const sf::Vector2f &pos);

	void setSize(const float width, const float height);
	void setSize(const float diameter);
	void depleteLight(const float radius);

	// Defines at which rate the light should change
	void setRate(const float depletetion);
	const bool isCapped() const;
	const bool isEmpty() const;
};