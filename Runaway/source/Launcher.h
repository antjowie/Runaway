#pragma once
#include <SFML\Graphics\Sprite.hpp>
#include <list>
#include <vector>

#include "Tile.h"

class Launcher : public sf::Drawable
{
private:
	static const float pi;

	struct Projectile
	{
		sf::Sprite m_sprite;
		float m_life{ 0 };
		float m_angle{ 0 };

		Projectile(const float maxLife, const sf::Vector2f &size, const sf::Vector2f &pos, const float angle);
		void setSize(const float width, const float height);
		void setSize(const float diameter);
	};

	std::list<Projectile> m_projectiles;
	std::vector<std::vector<Tile*>> * m_tilemap;

	float m_cooldown;
	float m_fireRate;

	// All these values are used for projectile
	sf::Vector2f m_pos; // Used to calculate angle of projectile
	sf::Vector2f m_projectileSize;
	float m_speed;
	float m_projectileLife;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override final;

public:
	bool checkCollision(const Projectile &projectile) const;
	Launcher();

	void shoot(const sf::Vector2f &pos);
	void setPos(const sf::Vector2f &pos);
	void update(const float elapsedTime);

	// Init functions
	void setFireRate(const float fireRate);
	void setProjectileLife(const float life);
	void setProjectileSize(const sf::Vector2f &size);
	void setProjectileSpeed(const float projectileSpeed);
	void loadTilemap(std::vector<std::vector<Tile*>> * tilemap);

	const sf::Vector2f &getProjectileSize() const;

	std::list<Projectile> &getProjectiles();
};