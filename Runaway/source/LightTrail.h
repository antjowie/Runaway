#pragma once
#include <SFML\Graphics\Drawable.hpp>
#include <list>
#include "PlayerSprite.h"

class Sprite;

class LightTrail:
	public sf::Drawable
{
private:
	float m_life;

	struct Trail
	{
		float m_life;
		sf::Sprite m_sprite;
	};
	std::list<Trail> m_trail;
	Sprite *m_sprite;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	LightTrail();

	void update(const float elapsedTime);
	void pushTrail();

	void loadSprite(Sprite *sprite);
	void loadLife(const float life);
	const LightTrail &getLightTrail() const;
};
