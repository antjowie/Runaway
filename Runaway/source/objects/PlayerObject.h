#pragma once
#include "Object.h"

class Tile; // Should I already include Tile header

class PlayerObject :
	public Object
{
private:
	class Player
	{
	private:
		sf::Sprite m_player;
		sf::Vector2f m_movement;
		int m_tileWidth{ 0 };	// We need these values to map our global coords to tilemap coords
		int m_tileHeight{ 0 };
		float m_bottomDistance{ 0 };
		bool m_isCrouching = false;

		const sf::Vector2i mapWorldToTilemap(const sf::Vector2f &coords) const;
		const bool isItemPressed(const std::string string) const; // Should this be a normal or member function
																  // No added benefit making it normal function, it's only used for player

		void updateCollisionDistance(const std::vector<std::vector<Tile*>> &tileMap); 
		const float distanceTillBottomCollision(const std::vector<Tile*> &surroundingTiles) const;
	public:
		Player();

		virtual void _logic(const float elapsedTime);
		virtual void _input(sf::RenderWindow &window);
		virtual void _draw(sf::RenderWindow &window);
		
		void snapOutOfBlocks(const std::vector<std::vector<Tile*>> &tileMap);
		void setTileSize(const sf::Vector2i &tileSize);
		void setPos(const sf::Vector2f &pos);

		sf::FloatRect const getHitbox() const;
		sf::Vector2f const getPos() const;
	};

public:
	Player m_player;

	PlayerObject(const bool isValid = false);

	virtual void logic(const float elapsedTime);
	virtual void input(sf::RenderWindow &window);
	virtual void draw(sf::RenderWindow &window);
};

