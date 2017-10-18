#pragma once
#include "Tile.h"
#include "AnimationHandler.h"

class GateTile :
	public Tile
{
private:
	AnimationHandler m_animHandler;

public:
	bool m_isOpen{ false };

	GateTile(const float x, const float y, const std::string tilesetName);
	
	void move(const float x, const float y);
	void setSolid(const bool setSolid);

	virtual void update(const float elapsedTime) final;
	virtual void draw(sf::RenderWindow &window) final;
};

