//
// SoundManager keeps track of all the volumes of sounds so that the
// used can easily change it
//
// Not sure if music should also be handled here.
//

#pragma once
#include "SoundObject.h"
#include <list>

class SoundManager
{
private:
	std::list<SoundObject* > m_effects;
	std::list<SoundObject* > m_music;

public:
	void play(); // Used to play sound which are loaded before hand
	void update();

	// Should be between 0 - 100
	void setTargetVolume(const float volume, const SoundType type);
												   
	void addSound(SoundObject * const sound);

	~SoundManager();
};