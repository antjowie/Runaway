#pragma once
#include "ElevatorTile.h"

class Elevator
{
private:
	ElevatorTile* m_topElevatorTile;
	ElevatorTile* m_bottomElevatorTile;

	const int m_id;
	const float m_height;
	float m_originalHeight;

	float m_speed;
	float m_timeline{ 0 };

public:
	bool m_isOpen;

	Elevator(const int id, const float m_height, const float speed);

	void loadTopTile(ElevatorTile* const topElevatorTile);
	void loadBottomTile(ElevatorTile* const bottomElevatorTile);
	void loadElevator();

	void update(const float elapsedTime);

	const int getId() const; 

};