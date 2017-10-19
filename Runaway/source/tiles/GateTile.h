#pragma once
#include "Tile.h"
#include "AnimationHandler.h"

class GateTile :
	public Tile
{
private:
	AnimationHandler m_animHandler;
	int m_textureRect{ 0 };

public:
	bool m_isOpen{ false };

	GateTile(const float x, const float y, const std::string tilesetName);
	
	void move(const float x, const float y);
	void setTextureRect(const int textureRect);

	virtual void update(const float elapsedTime) final;
	virtual void draw(sf::RenderWindow &window) final;
};
