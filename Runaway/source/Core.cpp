#include "Core.h"
#include "DataManager.h"

#include <iostream>

void Core::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_body, states);
	target.draw(m_eyes, states);
	target.draw(m_leftLauncher, states);
	target.draw(m_middleLauncher, states);
	target.draw(m_rightLauncher, states);
}

void Core::updateCollision(std::list<Projectile>& projectiles)
{
	m_body.updateCollision(projectiles);
	m_eyes.updateCollision(projectiles);
}

void Core::update(const float elapsedTime)
{
	if (m_state <= 0)
		m_phase = Core::Destroyed;

	// Allows core to move smoothly
	sf::Vector2f movement{ m_targetPos - m_body.getPos() };
	float magnitude{ movement.x * movement.x + movement.y * movement.y };
	if (elapsedTime < 1 && magnitude != 0) // Happens if fps is lower than 1
		magnitude = magnitude * elapsedTime / magnitude;
	else
		magnitude = 1;

	movement.x *= magnitude;
	movement.y *= magnitude;
	m_body.setPos(m_body.getPos() + movement);

	// Fixes launcher pos
	m_leftLauncher.setPos(m_eyes.getPos() + sf::Vector2f(-96, 146));
	m_middleLauncher.setPos(m_eyes.getPos() + sf::Vector2f(0, 208));
	m_rightLauncher.setPos(m_eyes.getPos() + sf::Vector2f(96, 146));

	switch (m_phase)
	{
	case Core::Sleep:
	{
		m_targetPos = m_body.getPos();
		if ((m_body.isHit() || m_eyes.isHit()) && m_timeline == -1)
			m_timeline = 0;
		else if (m_timeline > 1)
		{
			m_phase = Core::Phase::Roam;
			m_timeline = -1;
			m_eyes.setColor(sf::Color(255, 255, 255, 255));
			m_animHandler.update(elapsedTime);
			m_body.setTextureRect(m_animHandler.getFrame());
		}
		else if (m_timeline != -1)
		{
			m_timeline += elapsedTime / 5.f;
			m_animHandler.update(elapsedTime);
			m_body.setTextureRect(m_animHandler.getFrame());
			m_eyes.setColor(sf::Color(255,255,255,m_timeline * 255.f));
		}
	}
	break;
	case Core::Roam:
	{
		m_targetPos = m_playerPos + sf::Vector2f(0, -550);
		// Why can't we have variables in case loops :/
		m_eyes.setPos(m_body.getPos() + sf::Vector2f(
			// This way the value can also be negative
			(m_playerPos - m_body.getPos()).x * std::abs((m_playerPos - m_body.getPos()).x) / (std::powf((m_playerPos - m_body.getPos()).x, 2) + std::powf((m_playerPos - m_body.getPos()).y, 2)) * 300,// Arbitrary
			(m_playerPos - m_body.getPos()).y * std::abs((m_playerPos - m_body.getPos()).y) / (std::powf((m_playerPos - m_body.getPos()).x, 2) + std::powf((m_playerPos - m_body.getPos()).y, 2)) * 15	// offset value
		));

		if (m_timeline == -1)
		{
			DefaultLauncher();
			m_timeline = 0;
		}

		if (m_timeline < 1.f)
			m_timeline += elapsedTime / 1.5f;
		else
		{
			const float timeTillTarget{ std::sqrtf(std::powf(m_playerPos.x - getPos().x,2) + std::powf(m_playerPos.y - getPos().y,2)) / m_middleLauncher.getProjectileSpeed()};
			const sf::Vector2f nextPos{ (m_playerPos - m_oldPlayerPos) * (timeTillTarget / elapsedTime) };
			const sf::Vector2f predictedPos(m_playerPos + sf::Vector2f(nextPos.x, 0));
				
			switch (m_state)
			{
			case 3:
				m_middleLauncher.shoot(m_playerPos);
				break;
			case 2:
				m_leftLauncher.shoot(predictedPos);
				m_rightLauncher.shoot(predictedPos);
				break;
			case 1:
				m_leftLauncher.shoot(predictedPos);
				m_middleLauncher.shoot(m_playerPos);
				m_rightLauncher.shoot(predictedPos);
				break;
			}
		}

		if (m_eyes.isHit())
		{
			m_hp -= 1;
			m_eyes.flash(0.3f);
		}
		if (m_body.isHit())
		{
			m_hp -= 2;
			m_body.flash(0.3f);
		}

		if (m_hp <= 0)
		{
			m_phase = Core::SwitchState;
			m_hp = 10;
			m_timeline = -1;
		}
	}
		break;
	case Core::Charge:
	{
		m_targetPos = sf::Vector2f(1376, 1316);
		m_eyes.setPos(m_body.getPos());

		float timeTillTarget{ std::sqrtf(std::powf(m_playerPos.x - getPos().x,2) + std::powf(m_playerPos.y - getPos().y,2)) / m_middleLauncher.getProjectileSpeed() };
		sf::Vector2f nextPos{ (m_playerPos - m_oldPlayerPos) * (timeTillTarget / elapsedTime) };
		sf::Vector2f predictedPos(m_playerPos + sf::Vector2f(nextPos.x,0));

		if (m_timeline == -1)
		{
			ChargeLauncher();
			m_timeline = 0;
		}
		
		if (m_timeline < 1)
		{
			m_timeline += elapsedTime / 3.f;
			m_leftLauncher.setProjectileSpeed(0.f);
			m_middleLauncher.setProjectileSpeed(0.f);
			m_rightLauncher.setProjectileSpeed(0.f);
		}
		else if (m_timeline < 2)
		{
			ChargeLauncher();
			m_timeline += elapsedTime / 10.f;
		}
		
		switch (m_state)
		{
		case 3:
			m_middleLauncher.shoot(m_playerPos);
			break;
		case 2:
			m_leftLauncher.shoot(m_playerPos);
			m_rightLauncher.shoot(m_playerPos);
			break;
		case 1:
			m_leftLauncher.shoot(m_playerPos);
			m_middleLauncher.shoot(m_playerPos);
			m_rightLauncher.shoot(m_playerPos);
			break;
		}
		
		if (m_timeline > 2)
		{
			m_state -= 1;
			m_timeline = -1;
			m_phase = Core::Roam;
		}
	}
		break;
	case Core::Destroyed:
	{
		m_targetPos = sf::Vector2f(1376, 1316);
		m_eyes.setPos(m_body.getPos());

		if (m_timeline == -1)
		{
			m_hp = 3;
			m_timeline = 0;
			ChargeLauncher();
		}
		else if (!m_isDead && m_timeline > 1)
		{
			m_timeline = 1;

			if (m_eyes.isHit() || m_body.isHit())
			{
				m_eyes.flash(0.3f);
				m_body.flash(0.3f);
				m_hp -= 1;
			}
			if (m_hp <= 0) 
			{
				m_isDead = true;
				m_timeline = 0;
			}

			m_leftLauncher.shoot(m_playerPos);
			m_middleLauncher.shoot(m_playerPos);
			m_rightLauncher.shoot(m_playerPos);
		}
		else if (m_isDead)
		{
			m_timeline += elapsedTime / 5.f;
			m_eyes.flash(5.f);
			m_body.flash(5.f);
			if (m_timeline > 1)
				m_destroy = true;
			// TODO animation
		}
		else 
		m_timeline += elapsedTime / 3.f;
	}
		break;
	
	case Core::SwitchState:
	{
		m_targetPos = sf::Vector2f(1376, 1316);
		m_eyes.setPos(m_body.getPos());

		if (m_timeline == -1)
			m_timeline = 0;

		m_timeline += elapsedTime / 3.f;

		if (m_timeline > 1)
		{
			m_timeline = -1;
			m_phase = Core::Charge;
		}
			
	}
		break;
	}

	// Update colors
	m_eyes.update(elapsedTime);
	m_body.update(elapsedTime);

	// Update projectiles
	m_leftLauncher.update(elapsedTime);
	m_middleLauncher.update(elapsedTime);
	m_rightLauncher.update(elapsedTime);
}

void Core::setPos(const sf::Vector2f & pos)
{
	m_targetPos = pos;
}

void Core::setPlayerPos(const sf::Vector2f & pos)
{
	m_oldPlayerPos = m_playerPos;
	m_playerPos = pos;
}

const sf::Vector2f & Core::getPos() const
{
	return m_body.getPos();
}

bool Core::hit(const sf::FloatRect & hitbox)
{
	bool isHit{ false };
	const sf::Vector2f middle{ hitbox.left + hitbox.width / 2, hitbox.top + hitbox.height / 2 };

	for (auto &proj : m_leftLauncher.getProjectiles())
	{
		if (proj.m_sprite.getGlobalBounds().contains(middle))
		{
			isHit = true;
			proj.m_life = 0;
		}
	}

	for (auto &proj : m_middleLauncher.getProjectiles())
	{
		if (proj.m_sprite.getGlobalBounds().contains(middle))
		{
			isHit = true;
			proj.m_life = 0;
		}
	}

	for (auto &proj : m_rightLauncher.getProjectiles())
	{
		if (proj.m_sprite.getGlobalBounds().contains(middle))
		{
			isHit = true;
			proj.m_life = 0;
		}
	}
	return isHit;
}

bool Core::inDarkZone() const
{
	return m_phase == Core::Charge || m_phase == Core::SwitchState;
}

Core::Core():
	m_animHandler(576,480)
{
	// Center eye
	m_body.pushHitbox(Part::Hitbox(-96, 16, 192, 64));
	m_body.pushHitbox(Part::Hitbox(-32, -16, 64, 128));

	// Surrounding eyes
	m_body.pushHitbox(Part::Hitbox(-224, 16, 64, 64));
	m_body.pushHitbox(Part::Hitbox(-128, -80, 64, 64));
	m_body.pushHitbox(Part::Hitbox(-32,144, 64, 64));
	m_body.pushHitbox(Part::Hitbox(-64, -80, 64, 64));
	m_body.pushHitbox(Part::Hitbox(160, 16, 64, 64));
	
	// Center eye
	m_eyes.pushHitbox(Part::Hitbox(-64, 16, 128, 64));
	
	// Surrounding eyes
	m_eyes.pushHitbox(Part::Hitbox(-128, 112, 64, 64));
	m_eyes.pushHitbox(Part::Hitbox(-32, 176, 64, 64));
	m_eyes.pushHitbox(Part::Hitbox(64, 112, 64, 64));

	m_body.setTexture(DataManager::getInstance().getTexture("core"));
	m_eyes.setTexture(DataManager::getInstance().getTexture("coreEye"));

	m_eyes.setColor(sf::Color(255, 255, 255, 0));
	m_body.setColor(sf::Color(255, 255, 255, 255));

	// Same order as States
	m_animHandler.addAnimation(Animation(0, 8, 5.f/9.f, false, false));
	m_animHandler.addAnimation(Animation(0, 1, 1.f, true, false));
	m_animHandler.addAnimation(Animation(0, 16, 0.083f, false, false));

	m_animHandler.changeAnimation(m_phase);
	m_body.setTextureRect(m_animHandler.getFrame());
	
	m_body.setOrigin(288, 240);
	m_eyes.setOrigin(287, 144);

	m_body.setPos(1376, 1616);
	m_eyes.setPos(1376, 1616);
	m_targetPos = m_body.getPos();
}

void Core::Part::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

void Core::DefaultLauncher()
{
	m_leftLauncher.setFireRate(1.f);
	m_middleLauncher.setFireRate(1.f);
	m_rightLauncher.setFireRate(1.f);

	m_middleLauncher.setProjectileLife(13);
	m_leftLauncher.setProjectileLife(13);
	m_rightLauncher.setProjectileLife(13);

	m_leftLauncher.setProjectileSpeed(32 * 10);
	m_middleLauncher.setProjectileSpeed(32 * 10);
	m_rightLauncher.setProjectileSpeed(32 * 10);

	m_leftLauncher.setProjectileSize(sf::Vector2f(64 * 2, 64 * 2));
	m_middleLauncher.setProjectileSize(sf::Vector2f(64 * 2, 64 * 2));
	m_rightLauncher.setProjectileSize(sf::Vector2f(64 * 2, 64 * 2));
}

void Core::ChargeLauncher()
{
	DefaultLauncher();

	m_leftLauncher.setFireRate(0.f);
	m_middleLauncher.setFireRate(0.f);
	m_rightLauncher.setFireRate(0.f);
}

void Core::Part::setTexture(const sf::Texture &texture)
{
	m_sprite.setTexture(texture);
}

void Core::Part::setTextureRect(const sf::IntRect & rect)
{
	m_sprite.setTextureRect(rect);
}

void Core::Part::setOrigin(const float x, const float y)
{
	m_sprite.setOrigin(x,y);
}

void Core::Part::setPos(const sf::Vector2f & pos)
{
	m_sprite.setPosition(pos);
	for (auto &hitbox : m_hitbox)
	{
		hitbox.hitbox.left = pos.x + hitbox.offset.x;
		hitbox.hitbox.top = pos.y + hitbox.offset.y;
	}
}

void Core::Part::setPos(const float x, const float y)
{
	setPos(sf::Vector2f(x, y));
}

void Core::Part::setColor(const sf::Color & color)
{
	m_sprite.setColor(color);
}

const sf::Color &Core::Part::getColor() const
{
	return m_sprite.getColor();
}

const sf::Vector2f & Core::Part::getPos() const
{
	return m_sprite.getPosition();
}

void Core::Part::pushHitbox(const Hitbox &&hitbox)
{
	m_hitbox.push_back(std::move(hitbox));
}

void Core::Part::update(const float elapsedTime)
{
	m_timeline -= elapsedTime;
	sf::Color newColor(255,255,255,m_sprite.getColor().a);
	if (m_timeline < 0)
		m_timeline = 0;
	else
	{
		newColor.b = 0;
		newColor.g = 0;
	}
	m_sprite.setColor(newColor);
}

void Core::Part::updateCollision(std::list<Projectile>& projectiles)
{
	for (auto &proj : projectiles)
	{
		for (const auto &hitbox : m_hitbox)
			if (hitbox.hitbox.intersects(static_cast<sf::IntRect>(proj.m_sprite.getGlobalBounds())))
			{
				m_hit = true;
				proj.m_life = 0;
			}
	}

	projectiles.remove_if([this](Projectile &projectile) -> bool
	{
		return projectile.m_life == 0;
	});
}

bool Core::Part::isHit()
{
	bool temp{ m_hit };
	m_hit = false;
	return temp;
}

void Core::Part::flash(const float elapsedTime)
{
	m_timeline = elapsedTime;
}

Core::Part::Hitbox::Hitbox(const float offsetX, const float offsetY, const float width, const float height):
	offset(sf::Vector2f(offsetX,offsetY)),
	hitbox(sf::IntRect(0,0,width,height))
{	
}
