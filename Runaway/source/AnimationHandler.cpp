#include "AnimationHandler.h"
#include <math.h>

Animation::Animation(const unsigned int startFrame, const unsigned int endFrame, const float duration, const bool isRepeated, const bool isTransition):
	m_startFrame(startFrame),
	m_endFrame(endFrame),
	m_duration(duration),
	m_isRepeated(isRepeated),
	m_isTransition(isTransition)
{
}

int Animation::getLength() const
{
	return m_endFrame - m_startFrame + 1;
}

void AnimationHandler::addAnimation(const Animation &anim)
{
	m_animations.push_back(anim);
}

void AnimationHandler::changeAnimation(unsigned int animID)
{
	if (animID == m_currentAnim || animID > m_animations.size()) return;

	m_currentAnim = animID;
	sf::IntRect rect;
	
	if (!m_animations[animID].m_isTransition)
	{
		m_elapsed = 0.0f;
		rect = m_frameSize;
	}
	else
		rect = m_currentFrame;
	
	rect.top = rect.height * animID;
	m_currentFrame = rect;
}

void AnimationHandler::update(const float dt)
{
	if (m_currentAnim >= (int)m_animations.size() || m_currentAnim < 0) return;

	const float duration = m_animations[m_currentAnim].m_duration;
	if (int((m_elapsed + dt) / duration) > int((m_elapsed / duration))) {
		int frame = int((m_elapsed + dt) / duration);
		if (!m_animations[m_currentAnim].m_isRepeated && (unsigned int)frame > m_animations.at(m_currentAnim).m_endFrame)
			frame = m_animations[m_currentAnim].m_endFrame;
		else frame %= m_animations[m_currentAnim].getLength();

		sf::IntRect rect = m_frameSize;
		rect.left = rect.width * frame;
		rect.top = rect.height * m_currentAnim;
		m_currentFrame = rect;
	}
	
	m_elapsed += dt;
	if (m_elapsed > duration * m_animations[m_currentAnim].getLength())
		fmod(m_elapsed, duration);
}

sf::IntRect AnimationHandler::getFrame() const
{
	return m_currentFrame;
}

AnimationHandler::AnimationHandler(const sf::Vector2i & frameSize):
	m_elapsed(0),
	m_currentAnim(-1)
{
	m_frameSize.left = m_frameSize.top = 1;
	m_frameSize.width = frameSize.x;
	m_frameSize.height = frameSize.y;
}

AnimationHandler::AnimationHandler(const int width, const int height):
	AnimationHandler(sf::Vector2i(width,height))
{
}
