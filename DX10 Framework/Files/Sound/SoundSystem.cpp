// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	:	SoundSystem.cpp
// Description	:	The implementation file.
// Author(s)	:	Juran Griffith.
// Mail			:	juran.griffith@mediadesignschool.com
//

// Library Includes

// Local Includes
#include "SoundSystem.h"

SoundSystem::SoundSystem()
{
	
}

SoundSystem::~SoundSystem()
{
	m_pChannelMusic->release();
	m_pChannelEffects->release();

	m_pSystem->release();
}

bool SoundSystem::Initialise()
{
	if (FMOD::System_Create(&m_pSystem) != FMOD_OK)
	{
		// Report Error
		return false;
	}

	int driverCount = 0;
	m_pSystem->getNumDrivers(&driverCount);

	if (driverCount == 0)
	{
		// Report Error
		return false;
	}

	// Initialize our Instance with 36 Channels
	if (m_pSystem->init(36, FMOD_INIT_NORMAL, NULL) != FMOD_OK)
	{
		// Report Error
		return false;
	}

	if (m_pSystem->createChannelGroup(NULL, &m_pChannelMusic) != FMOD_OK)
	{
		// Report Error
		return false;
	}

	if (m_pSystem->createChannelGroup(NULL, &m_pChannelEffects) != FMOD_OK)
	{
		// Report Error
		return false;
	}

	return true;
}

bool SoundSystem::LoadFile(TSound* _pTSound, std::string _pFile, SOUND_TYPE _type)
{
	std::string soundFilePath = FILEPATH;

	_pTSound->m_filename = _pFile;

	// Note:
	// Use FMOD_CREATESAMPLE to load the entire sound and decompress it in memory, to speed up playback.

	if (_type == SOUND_TYPE_MUSIC)
	{
		soundFilePath += "Music/" + _pFile;
		if (FMOD_OK == m_pSystem->createStream(soundFilePath.c_str(), FMOD_DEFAULT, 0, &_pTSound->m_pSound)) // Stream for music
		{
			if (FMOD_OK == m_pSystem->playSound(_pTSound->m_pSound, m_pChannelMusic, true, &_pTSound->m_pChannel)) // Play the file paused to get the channel. Unpause it to play it later.
			{
				return true;
			}
		}
	}
	else
	{
		soundFilePath += "Effect/" + _pFile;
		if (FMOD_OK == m_pSystem->createSound(soundFilePath.c_str(), FMOD_DEFAULT, 0, &_pTSound->m_pSound)) // Sound for effects
		{
			if (FMOD_OK == m_pSystem->playSound(_pTSound->m_pSound, m_pChannelEffects, true, &_pTSound->m_pChannel))
			{
				return true;
			}
		}
	}
	return false;
}

void SoundSystem::SetVolumeMusic(float _volume)
{
	m_pChannelMusic->setVolume(max(min(_volume, 1.0f), 0.0f)); // Volumes should be between 0.0f and 1.0f. The max min ensures this.
}

void SoundSystem::SetVolumeEffects(float _volume)
{
	m_pChannelEffects->setVolume(max(min(_volume, 1.0f), 0.0f)); // Volumes should be between 0.0f and 1.0f. The max min ensures this.
}

void SoundSystem::MuteMusic(bool _mute)
{
	m_pChannelMusic->setMute(_mute);
}

void SoundSystem::MuteEffects(bool _mute)
{
	m_pChannelEffects->setMute(_mute);
}