#pragma once
#include <SFML\Graphics.hpp>
#include <vector>

struct Animation
{
	// Each Animation object represents a row in the spritesheet
	unsigned int m_startFrame;
	unsigned int m_endFrame;

	float m_duration;
	bool m_isRepeated;

	Animation(const unsigned int startFrame, const unsigned int endFrame, const float duration, const bool isRepeated = true);
	int getLength() const;
};

class AnimationHandler
{
private:
	sf::IntRect m_frameSize;
	sf::IntRect m_currentFrame;

	std::vector<Animation> m_animations;

	int m_currentAnim;
	float m_elapsed;

public:
	void addAnimation(const Animation &anim);
	void changeAnimation(unsigned int animID);
	
	void update(const float dt);
	
	sf::IntRect getFrame() const;

	// Class assumes every frame has same size. Because it iterates the spritesheet like blocks
	AnimationHandler(const sf::Vector2i &frameSize);
	AnimationHandler(const int width, const int height);
};