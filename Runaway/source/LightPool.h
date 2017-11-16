#pragma once
#include <SFML\Graphics\Sprite.hpp>

class LightPool : public sf::Drawable
{
	sf::Sprite m_sprite;
	
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:

	void update(const float elapsedTime);
	void setPos(const sf::Vector2f &pos);

	LightPool();
	~LightPool();
};