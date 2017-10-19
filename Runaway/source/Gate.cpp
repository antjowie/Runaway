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

void Gate::updateTextureRect()
{
	unsigned int middle{ m_tiles.size() / 2 -1};

	m_tiles[middle++]->setTextureRect(2);
	m_tiles[middle]->setTextureRect(1);

}

void Gate::update(const float elapsedTime)
{
	if (m_tiles.empty()) return;

	// Update texture
	if (m_isOpen)
	{
		for (auto &iter : m_tiles)
		{
			iter->m_isOpen = true;
		}
	}
	else
	{
		for (auto &iter : m_tiles)
		{
			iter->m_isOpen = false;
		}
	}

	for (const auto &iter : m_tiles)
		iter->update(elapsedTime);

	// Update pos

	const unsigned int middle{ m_tiles.size() / 2 };

	if (m_isOpen)
	{
		// Lower bottom row
		for (unsigned int i = 0; i < middle; i++)
		{
			m_tiles[i]->move(0, 32 * elapsedTime);
			if (m_tiles[i]->getHitbox().top > m_originalTiles[i].getHitbox().top + m_originalTiles[i].getHitbox().height * middle)
				m_tiles[i]->move(0, -(m_tiles[i]->getHitbox().top - (m_originalTiles[i].getHitbox().top + m_originalTiles[i].getHitbox().height * middle)));
		}

		// Rise upper row
		
		for (unsigned int i = middle; i < m_tiles.size(); i++)
		{
			m_tiles[i]->move(0, -32 * elapsedTime);
			if (m_tiles[i]->getHitbox().top < m_originalTiles[i].getHitbox().top - m_originalTiles[i].getHitbox().height * (m_tiles.size() - middle))
				m_tiles[i]->move(0, abs(m_tiles[i]->getHitbox().top - (m_originalTiles[i].getHitbox().top - m_originalTiles[i].getHitbox().height * (m_tiles.size() - middle))));
		}
		
		
	}
	else
	{
		// Rise lower row
		for (unsigned int i = 0; i < middle; i++)
		{
			m_tiles[i]->move(0, -32 * elapsedTime);
			if (m_tiles[i]->getHitbox().top < m_originalTiles[i].getHitbox().top)
				m_tiles[i]->move(0, abs(m_tiles[i]->getHitbox().top - m_originalTiles[i].getHitbox().top));
		}

		// Lower upper row
		for (unsigned int i = middle; i < m_tiles.size(); i++)
		{
			m_tiles[i]->move(0, 32 * elapsedTime);
			if (m_tiles[i]->getHitbox().top > m_originalTiles[i].getHitbox().top)
				m_tiles[i]->move(0, -(m_tiles[i]->getHitbox().top - m_originalTiles[i].getHitbox().top));
		}
	}
}