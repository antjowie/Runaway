#pragma once
#include "Tile.h"
#include "AnimationHandler.h"

class GateTile :
	public Tile
{
private:
	AnimationHandler m_animHandler;
	int m_textureType{ 0 };

public:
	bool m_isOpen{ false };

	GateTile(const float x, const float y);
	
	void move(const float x, const float y);
	void setTextureType(const unsigned int textureType);
	void setSolid(const bool isSolid);

	virtual void update(const float elapsedTime) final;
};