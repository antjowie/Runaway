#include "BossLauncher.h"

void BossLauncher::update(const float elapsedTime)
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
	// This is the only line that needs change
	m_projectiles.remove_if([&](const Projectile &projectile) -> bool
	{ return (projectile.m_life < 0); });
}

float BossLauncher::getProjectileSpeed() const
{
	return m_speed;
}

BossLauncher::BossLauncher() :
	Launcher(true)
{
	
}