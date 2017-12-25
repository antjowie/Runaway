#pragma once
#include "ElevatorTile.h"
#include "SoundObject.h"

class Elevator
{
private:
	ElevatorTile* m_topElevatorTile;
	ElevatorTile* m_bottomElevatorTile;

	const int m_id;
	const float m_height;
	float m_originalHeight;

	bool m_hasHit{ true };
	bool m_moving{ false };
	float m_soundTimeline{ 0 };
	SoundObject * m_movingSound;
	SoundObject * m_hitSound;

	float m_speed;
	float m_timeline{ 0 };

	const bool m_inverted{ false };

public:
	bool m_isOpen;

	Elevator(const int id, const float m_height, const float speed, const bool inverted = false);

	void initSound(SoundManager & soundManager);

	void loadTopTile(ElevatorTile* const topElevatorTile);
	void loadBottomTile(ElevatorTile* const bottomElevatorTile);
	void loadElevator();

	void update(const float elapsedTime);

	const int getId() const; 

};