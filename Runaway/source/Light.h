//
// This class renders an off screen texture with all the light objects.
// It should be drawn over the whole scene. It also manages the darkness of it.
//

#pragma once
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RenderTexture.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include "Tile.h"

class Light : public sf::Drawable
{
private: 
	std::vector<const Tile*> m_tiles; // Tiles which emit light
	
	sf::RenderTexture m_lightTex;
	sf::Sprite m_light;

	// These are some variables for a lerp function
	sf::RectangleShape m_overlay;
	sf::Uint8 m_brightnessTarget{ 255 };
	sf::Uint8 m_oldBrightness{ 255 };
	float m_fadeTime{ 1 };
	float m_elapsedTime{ 0 };

	// This can draw onto window
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	Light();

	void addTile(const std::vector<const Tile*> &tiles);
	void addTile(const Tile* const tile);

	void update(const float elapsedTime);

	// 0 dark - 255 white
	void setBrightness(const sf::Uint8 brightness);

	// Init
	void setFadeTime(const float time);
	void setSize(const sf::Vector2f size);
	void setSize(const float width, const float height);
};

