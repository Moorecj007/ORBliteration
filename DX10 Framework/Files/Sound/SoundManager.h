//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	:	SoundManager.h
// Description	:	This handles the playing of music and sound effects within the game.
// Author		:	Juran Griffith
// Mail			:	juran.griffith@mediadesignschool.com
//

// Inclusion Guards
#pragma once
#ifndef __SOUNDMANAGER_H__
#define __SOUNDMANAGER_H__

// Library Includes

// Local Includes
#include "SoundSystem.h"

class SoundManager
{
	// Functions
	public:
		/***********************
		* SoundManager: Default Constructor for this class
		* @author: Juran Griffith.
		********************/
		SoundManager();

		/***********************
		* SoundManager: Default destructor for this class.
		* @author: Juran Griffith.
		********************/
		~SoundManager();

		/***********************
		* Initialise: Initialise this object
		* @author: Juran Griffith.
		* @return: bool: Successful or not
		********************/
		bool Intialise();

		/***********************
		* Update: Updates the sound system. Must be called every frame.
		* @author: Juran Griffith.
		* @return: void
		********************/
		void Update();

		/***********************
		* PlaySong: Plays a music track based on the index given. All other songs will pause and restart from the beginning of the track.
		* @author: Juran Griffith.
		* @parameter: _songChoice - Which song to play.
		* @return: void
		********************/
		void PlaySong(UINT _songChoice);

		/***********************
		* PlayMenuAccept: Plays the sound effect.
		* @author: Juran Griffith.
		* @return: void
		********************/
		void PlayMenuAccept();

		/***********************
		* PlayMenuBack: Plays the sound effect.
		* @author: Juran Griffith.
		* @return: void
		********************/
		void PlayMenuBack();

		/***********************
		* PlayMenuNavigate: Plays the sound effect.
		* @author: Juran Griffith.
		* @return: void
		********************/
		void PlayMenuNavigate();

		/***********************
		* PlayMenuToggles: Plays the sound effect.
		* @author: Juran Griffith.
		* @return: void
		********************/
		void PlayMenuToggles();

		/***********************
		* PlayPlayerDeath: Plays the sound effect.
		* @author: Juran Griffith.
		* @return: void
		********************/
		void PlayPlayerDeath();

		/***********************
		* PlayPlayerHit: Plays the sound effect.
		* @author: Juran Griffith.
		* @return: void
		********************/
		void PlayPlayerHit();

		/***********************
		* PlayPlayerPhase: Plays the sound effect.
		* @author: Juran Griffith.
		* @return: void
		********************/
		void PlayPlayerPhase();

		/***********************
		* PlayPlayerBoost: Plays the sound effect.
		* @author: Juran Griffith.
		* @return: void
		********************/
		void PlayPlayerBoost();

		/***********************
		* PlayPhenomenaSplash: Plays the sound effect.
		* @author: Juran Griffith.
		* @return: void
		********************/
		void PlayPhenomenaSplash();

		/***********************
		* Mute: Toggles the sound effects and music off or on based on the flag passed in.
		* @author: Juran Griffith.
		* @parameter: _mute - True to mute otherwise false to unmute.
		* @return: void
		********************/
		void Mute(bool _mute);

	protected:
	private:

	// Variables
	public:
	protected:
	private:
		SoundSystem* m_pSoundSystem;

		std::vector<TSound*> m_pMusic;
		TSound* m_pEffect_splash_ps;
		TSound* m_pEffect_menu_nav;
		TSound* m_pEffect_menu_back;
		TSound* m_pEffect_menu_accept;
		TSound* m_pEffect_menu_tog;
		TSound* m_pEffect_player_hit;
		TSound* m_pEffect_player_death;
		TSound* m_pEffect_player_phase;
		TSound* m_pEffect_player_boost;
};

#endif