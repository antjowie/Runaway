#include "DoorTile.h"
#include "DataManager.h"


DoorTile::DoorTile(const float x, const float y, const std::string tilesetName):
	Tile(x,y,tilesetName,false)
{
	m_sprite.setTextureRect(sf::IntRect(32 * 3, 0, 32,32));
}