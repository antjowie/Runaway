#pragma once
#include "Tile.h"
class InvalidTile :
	public Tile
{
public:
	InvalidTile(const float x, const float y,const std::string tilesetName);
};

