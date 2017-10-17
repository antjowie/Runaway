#pragma once
#include "Tile.h"

class LightTile:
	public Tile
{
public:
	LightTile(const float x, const float y, const std::string tilesetName);
	~LightTile();
};

