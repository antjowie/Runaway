#include "SoundObject.h"
#include "SoundManager.h"

SoundObject::SoundObject(const SoundType soundType, SoundManager &soundManager):
	m_type(soundType),
	m_shouldPop(false)
{
	soundManager.addSound(this);
}
