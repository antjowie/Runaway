//
// Template class for all tiles
//

#pragma once
#include <SFML\Graphics.hpp>

enum class TileType{ Air, Block, Top, Light, Gate, Invalid}; // Has to be same order as in Tiled

class Tile : public sf::Drawable
{
private:
	TileType m_TileType;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

protected:
	bool m_solid = true;
	sf::Sprite m_sprite;

public:
	Tile(const TileType type, const float x, const float y, const bool isSolid = true);
	
	virtual void update();

	void setSolid(const bool isSolid = true);
	// 255 = initial brightness, 0 = black
	void setBrightness(const sf::Uint8 &brigthness = 255);

	const sf::FloatRect getHitbox() const;
	const TileType getType() const;
	const bool isSolid() const;
};

Tile * const getTile(const int id, const float x, const float y);