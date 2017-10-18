#include "Gate.h"



bool Gate::isClosed()
{
	if(!m_tiles.empty())
		return m_tiles[0]->getHitbox()== m_originalTiles[0].getHitbox();
	return true;
}

Gate::Gate()
{
}

Gate::Gate(const int id, const float speed):
	m_id(id),m_speed(speed)
{
}

void Gate::addTile(GateTile * const tile)
{
	m_tiles.push_back(tile);
	m_originalTiles.push_back(GateTile(*tile));
}

void Gate::addTile(std::vector<GateTile*> &tiles)
{
	for (auto &iter : tiles)
		addTile(iter);
}

void Gate::setSpeed(const float speed)
{
	m_speed = speed;
}

void Gate::update(const float elapsedTime)
{
	// Calculate at what speed the gate should open
	if (m_tiles.empty()) return;

	if (m_isOpen)
	{
		for (auto &iter : m_tiles)
		{
			iter->m_isOpen = true;
			iter->setSolid(false);
		}
	}
	else
	{
		for (auto &iter : m_tiles)
		{
			iter->m_isOpen = false;
			iter->setSolid(true);
		}
	}

	for (const auto &iter : m_tiles)
		iter->update(elapsedTime);

	/*
	unsigned int middle = m_tiles.size() / 2; // Amount of tiles to move
	if (middle < static_cast<float>(m_tiles.size()) / 2.f)
		++middle;
	const float movement = m_originalTiles[0].getHitbox().height * middle / m_speed;

	for (int i = 0; i < m_tiles.size(); i++)
	{
		// Update state and location
		if (!m_isOpen)
		{
			m_tiles[i]->m_isOpen = true;
			
			if (abs(m_tiles[i]->getHitbox().top - m_originalTiles[i].getHitbox().top) < middle*m_tiles[i]->getHitbox().height)
			{
				if (i < middle)
				{
					m_tiles[i]->move(0, -movement * elapsedTime);
					if (abs(m_tiles[i]->getHitbox().top - m_originalTiles[i].getHitbox().top) > middle*m_tiles[i]->getHitbox().height)
						m_tiles[i]->move(0, (abs(m_tiles[i]->getHitbox().top - m_originalTiles[i].getHitbox().top)) - middle*m_tiles[i]->getHitbox().height);
				}
				else
				{
					m_tiles[i]->move(0, movement * elapsedTime);
					if (abs(m_tiles[i]->getHitbox().top - m_originalTiles[i].getHitbox().top) > middle*m_tiles[i]->getHitbox().height)
						m_tiles[i]->move(0, -(abs(m_tiles[i]->getHitbox().top - m_originalTiles[i].getHitbox().top)) - middle*m_tiles[i]->getHitbox().height);
				}		
			}
		}
		else
		{
			m_tiles[i]->m_isOpen = false;
		}
	}
	*/
}