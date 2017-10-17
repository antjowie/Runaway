#pragma once
#include "Tile.h"

class GroundTile:
	public Tile
{
public:
	GroundTile(const float x, const float y, const std::string tilesetName);
};

