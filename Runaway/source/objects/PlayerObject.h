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
		sf::IntRect m_tileCoords; // Player hitbox in tile coords
		sf::Vector2f m_movement;
		int m_tileWidth;	// We need these values to map our global coords to tilemap coords
		int m_tileHeight;
		bool m_jumped = false;
		bool m_isFloating = true;
		bool m_isCrouching = false;

		const bool isItemPressed(const std::string string) const; // Should this be a normal or member function
		const bool checkCollision(const std::vector<std::vector<Tile*>> tiles) const; 

	public:
		Player();

		virtual void _logic(const float elapsedTime);
		virtual void _input(sf::RenderWindow &window);
		virtual void _draw(sf::RenderWindow &window);

		void isDropping(const std::vector<std::vector<Tile*>> tiles);
		
		void setPos(const sf::Vector2f &pos);
		void setTileSize(const sf::Vector2i &tileSize);

		sf::FloatRect const getHitBox() const;
		sf::Vector2f getPos();
		[[deprecated]]sf::Vector2f getMovement();	// For camera
	};

public:
	Player m_player;

	PlayerObject(const bool isValid = false);

	virtual void logic(const float elapsedTime);
	virtual void input(sf::RenderWindow &window);
	virtual void draw(sf::RenderWindow &window);
};

