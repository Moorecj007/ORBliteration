//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	:	SoundSystem.h
// Description	:	This wraps all the sound objects using FMOD api.
// Author		:	Juran Griffith
// Mail			:	juran.griffith@mediadesignschool.com
//

// Inclusion Guards
#pragma once
#ifndef __SOUNDSYSTEM_H__
#define __SOUNDSYSTEM_H__

// Linking
#pragma comment(lib, "fmod_vc.lib")

// Library Includes
#include "..\DX10\DX10\DX10_Utilities.h"
#include "fmod.hpp"
#include "fmod_errors.h"

// Local Includes

// Globals
#define FILEPATH "Resources/Sound/"

enum SOUND_TYPE
{
	SOUND_TYPE_MUSIC,
	SOUND_TYPE_EFFECT
};

struct TSound
{
	// Variables
	FMOD::Sound* m_pSound;
	FMOD::Channel* m_pChannel;
	std::string m_filename;

	/***********************
	* TSound: Default destructor for the sound system class.
	* @author: Juran Griffith.
	********************/
	~TSound()
	{
		if (m_pSound)
			m_pSound->release();
	}

	/***********************
	* SetVolume: Sets the volume of the sound file.
	* @author: Juran Griffith.
	* @parameter: _volume: The volume to set the level to.
	* @return:	void.
	********************/
	void SetVolume(float _volume)
	{
		if (m_pChannel)
			m_pChannel->setVolume(max(min(_volume, 1.0f), 0.0f)); // Volumes should be between 0.0f and 1.0f. The max min ensures this.
	}

	/***********************
	* SetLooped: Sets the sound file to loop or not.
	* @author: Juran Griffith.
	* @parameter: _loop: To loop or not.
	* @return:	void.
	********************/
	void SetLooped(bool _loop)
	{
		if (!_loop)
		{
			if (m_pSound)
				m_pSound->setMode(FMOD_LOOP_OFF);
		}
		else
		{
			if (m_pSound)
			{
				m_pSound->setMode(FMOD_LOOP_NORMAL);
				m_pSound->setLoopCount(-1);
			}
		}
	}

	/***********************
	* Play: Plays the sound file.
	* @author: Juran Griffith.
	* @return:	void.
	********************/
	void Play()
	{
		if(m_pChannel)
			m_pChannel->setPaused(false);
	}

	/***********************
	* Pause: Pauses the sound file.
	* @author: Juran Griffith.
	* @return:	void.
	********************/
	void Pause()
	{
		if (m_pChannel)
			m_pChannel->setPaused(true);
	}
};

class SoundSystem
{
	// Functions
	public:
		/***********************
		* SoundSystem: Default constructor for the sound system class.
		* @author: Juran Griffith.
		********************/
		SoundSystem();

		/***********************
		* SoundSystem: Default destructor for the sound system class.
		* @author: Juran Griffith.
		********************/
		~SoundSystem();

		/***********************
		* Initialise: Intialises the sound system class.
		* @author: Juran Griffith.
		* @return:	bool: Successful or not.
		********************/
		bool Initialise();

		/***********************
		* LoadFile: Loads a sound file.
		* @author: Juran Griffith.
		* @parameter: _pTSound: The sound struct to hold the new file data.
		* @parameter: _pFile: The file name.
		* @parameter: _type: The type of file.
		* @return:	void.
		********************/
		bool LoadFile(TSound* _pTSound, std::string _pFile, SOUND_TYPE _type);

		/***********************
		* SetVolumeMusic: Sets the volume of the music.
		* @author: Juran Griffith.
		* @parameter: _volume: The volume to set the music level to.
		* @return:	void.
		********************/
		void SetVolumeMusic(float _volume);

		/***********************
		* SetVolumeEffects: Sets the volume of the effects.
		* @author: Juran Griffith.
		* @parameter: _volume: The volume to set the effects level to.
		* @return:	void.
		********************/
		void SetVolumeEffects(float _volume);

		/***********************
		* MuteMusic: Mutes and unmutes the music.
		* @author: Juran Griffith.
		* @parameter: _mute: To toggle the music off or on.
		* @return:	void.
		********************/
		void MuteMusic(bool _mute);

		/***********************
		* MuteEffects: Mutes and unmutes the effects.
		* @author: Juran Griffith.
		* @parameter: _mute: To toggle the effects off or on.
		* @return:	void.
		********************/
		void MuteEffects(bool _mute);

	protected:
	private:

	// Variables
	public:
	protected:
	private:
		FMOD::System* m_pSystem;
		FMOD::ChannelGroup* m_pChannelMusic;
		FMOD::ChannelGroup* m_pChannelEffects;
};

#endif