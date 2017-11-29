#include "Launcher.h"
#include "Level.h"
#include "DataManager.h"

void Launcher::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (const auto &iter : m_projectiles)
	{
		target.draw(iter.m_sprite, states);
	}
}

bool Launcher::checkCollision(const Launcher::Projectile &projectile) const
{
	sf::Sprite sprite = projectile.m_sprite;
	sf::Vector2i pos{ mapWorldToTilemap(sprite.getPosition(), (*m_tilemap)[0][0]->getHitbox().width, (*m_tilemap)[0][0]->getHitbox().height) };
	
	// Assumes all tilemaps are rectangles
	for (int i = pos.x - 5; i <= pos.x + 5 && i > 0 && i < m_tilemap[0][0].size(); ++i)
		for (int j = pos.y - 5; j <= pos.y + 5 && j > 0 && j < m_tilemap[0].size(); ++j)
		{
			if (sprite.getGlobalBounds().intersects((*m_tilemap)[j][i]->getHitbox()) && (*m_tilemap)[j][i]->getTileMeta().m_solid == true) return true;
		}
	return false;
}

Launcher::Launcher():
	m_tilemap(nullptr),
	m_pos(0,0),
	m_cooldown(0),
	m_fireRate(1),
	m_speed(32)
{
}

void Launcher::shoot(const sf::Vector2f & pos)
{
	if (m_cooldown != 0) return;
	m_cooldown = m_fireRate;

	// Calculate angle
	sf::Vector2f triangle{ pos.x - m_pos.x, pos.y - m_pos.y };
	
	float angle{ atan(triangle.y / triangle.x) };
	angle = angle / pi * 180.f;
	
	// Fix angle pos
	if (triangle.x < 0)
		angle += 180;
	
	m_projectiles.push_back(Projectile(m_projectileLife, m_projectileSize, m_pos, angle));
}

void Launcher::setPos(const sf::Vector2f & pos)
{
	m_pos = pos;
}

void Launcher::update(const float elapsedTime)
{
	m_cooldown -= elapsedTime;
	if (m_cooldown < 0)
		m_cooldown = 0;

	for (std::list<Projectile>::iterator iter{ m_projectiles.begin() }; iter != m_projectiles.end(); ++iter)
	{

		// Update pos
		float x{ cos(iter->m_angle*pi / 180.f) * m_speed };
		float y{ sin(iter->m_angle*pi / 180.f) * m_speed };

		x *= elapsedTime;
		y *= elapsedTime;
		iter->m_sprite.move(x, y);

		// Update life and check death conditions
		iter->m_life -= elapsedTime;
		
		// Update light size
		iter->setSize(m_projectileSize.x * (iter->m_life / m_projectileLife), m_projectileSize.y * (iter->m_life / m_projectileLife));
	}

	// Checks remove conditions
	m_projectiles.remove_if([&](const Projectile &projectile) -> bool 
	{ return (checkCollision(projectile) ||projectile.m_life < 0); });
}

void Launcher::setFireRate(const float fireRate)
{
	m_fireRate = fireRate;
}

void Launcher::setProjectileLife(const float life)
{
	m_projectileLife = life;
}

void Launcher::setProjectileSize(const sf::Vector2f & size)
{
	m_projectileSize = size;
}

void Launcher::setProjectileSpeed(const float projectileSpeed)
{
	m_speed = projectileSpeed;
}

void Launcher::loadTilemap(std::vector<std::vector<Tile*>> * tilemap)
{
	m_tilemap = tilemap;
}

const sf::Vector2f & Launcher::getProjectileSize() const
{
	return m_projectileSize;
}

std::list<Launcher::Projectile> &Launcher::getProjectiles()
{
	return m_projectiles;
}

Launcher::Projectile::Projectile(const float maxLife, const sf::Vector2f &size, const sf::Vector2f &pos, const float angle):
	m_life(maxLife),
	m_angle(angle)
{
	m_sprite.setTexture(DataManager::getInstance().getTexture("lightCircle"));
	m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2, m_sprite.getLocalBounds().height / 2);
	setSize(size.x,size.y);
	m_sprite.setPosition(pos);
}

void Launcher::Projectile::setSize(const float width, const float height)
{
	// Because sprite will never grow in size, bounds checking isn't necessary
	m_sprite.scale(width / m_sprite.getGlobalBounds().width, height / m_sprite.getGlobalBounds().height);
}

void Launcher::Projectile::setSize(const float diameter)
{
	setSize(diameter, diameter);
}

const float Launcher::pi = 3.14159265359;