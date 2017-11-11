#pragma once
#include "Tile.h"
#include "AnimationHandler.h"

class GateTile :
	public Tile
{
private:
	AnimationHandler m_animHandler;
	int m_textureType{ 0 };

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;
public:
	bool m_isOpen{ false };

	GateTile(const float x, const float y);
	
	void move(const float x, const float y);
	void setTextureType(const unsigned int textureType);

	virtual void update(const float elapsedTime) final;
};