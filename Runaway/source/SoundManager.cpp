#include "SoundManager.h"

void SoundManager::play()
{
	for (const auto& iter : m_effects)
		if (iter->getStatus() != sf::Sound::Status::Playing)
			iter->play();
	for (const auto& iter : m_music)
		if (iter->getStatus() != sf::Sound::Status::Playing)
			iter->play();
}

void SoundManager::update()
{
	// Clear removed sounds
	m_effects.remove_if([this](SoundObject* const soundObject)
	{	if (soundObject->m_shouldPop)
		{
			delete soundObject;
			return true;
		}
		return false; });
	m_music.remove_if([this](SoundObject* const soundObject)
	{	if (soundObject->m_shouldPop)
		{
			delete soundObject;
			return true;
		}
		return false; });
}

void SoundManager::setTargetVolume(const float volume, const SoundType type)
{
	// TODO push new volume values to config

	switch (type)
	{
	case SoundType::Effect:
		for (const auto& iter : m_effects)
			iter->setVolume(iter->getVolume() * (volume / 100.f ));
		break;
		
	case SoundType::Music:
		for (const auto& iter : m_effects)
			iter->setVolume(iter->getVolume() * (volume / 100.f));
		
	default:
		// Maybe return error code
		break;
	}
}

void SoundManager::addSound(SoundObject * const sound)
{
	switch (sound->m_type)
	{
	case SoundType::Effect:
		m_effects.push_back(sound);
		break;

	case SoundType::Music:
		m_music.push_back(sound);
		break;

	default:
		// Maybe push some kind of error
		break;
	}
}

SoundManager::~SoundManager()
{
	for (const auto &deleter : m_effects)
		delete deleter;
	for (const auto &deleter : m_music)
		delete deleter;
	
	// Probably uneccessary but I like to explicitly state it
	m_effects.clear();
	m_music.clear();
}
