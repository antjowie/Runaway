//
// Template class for all tiles
//

#pragma once
#include <SFML\Graphics\Sprite.hpp>

enum class TileType{ Air, Block, Top, Light, Gate, Invalid}; // Has to be same order as in Tiled

struct TileMeta
{
	TileType m_tileType{ TileType::Invalid };
	bool m_solid{ true };
	float m_light{ 0.0f };
};

class Tile : public sf::Drawable
{
private:
	TileMeta m_meta;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

protected:
	sf::Sprite m_sprite;

public:
	Tile(const TileType type, const float x, const float y, const float lightRadius = 0.0f, const bool isSolid = true);
	
	virtual void update(const float elapsedTime);

	void setSolid(const bool isSolid = true);
	// 255 = initial brightness, 0 = black
	void setBrightness(const sf::Uint8 &brightness = 255); // Temp for background objects

	const sf::FloatRect getHitbox() const;
	const TileMeta &getTileMeta() const;
};

Tile * const getTile(const int id, const float x, const float y);