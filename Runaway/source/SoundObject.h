//
// For some reason sound was already used
// Wrapper for sound class, used to delete from array
//

#pragma once
#include <SFML\Audio\Sound.hpp>
class SoundManager;

enum class SoundType { Music, Effect };
class SoundObject : public sf::Sound
{
public:
	const SoundType m_type;
	bool m_shouldPop;

	SoundObject(const SoundType soundType, SoundManager &soundManager);
};

