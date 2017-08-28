#pragma once
#include "Object.h"

class PlayerObject :
	public Object
{
private:
	class Player
	{
	private:
		sf::Sprite m_player;
		sf::Vector2f m_movement;
		bool m_jumped = false;
		bool m_isCrouching = false;

		bool const isItemPressed(const std::string string) const; // Should this be a normal or member function

	public:
		Player();

		virtual void _logic(const float elapsedTime);
		virtual void _input(sf::RenderWindow &window);
		virtual void _draw(sf::RenderWindow &window);

		void setPos(const sf::Vector2f &pos);

		sf::FloatRect const getHitBox() const;
		sf::Vector2f getPos();
	};

public:
	Player m_player;

	PlayerObject(const bool isValid = false);

	virtual void logic(const float elapsedTime);
	virtual void input(sf::RenderWindow &window);
	virtual void draw(sf::RenderWindow &window);
};

