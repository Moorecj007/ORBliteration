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
		// TO DO - Juran (comments)
		SoundManager();
		~SoundManager();

		bool Intialise();

		void Update();

		void PlaySong(UINT _songChoice);

		void PlayMenuAccept();

		void PlayMenuBack();

		void PlayMenuNavigate();

		void PlayMenuToggles();

		void PlayPlayerDeath();

		void PlayPlayerHit();

		void PlayPhenomenaSplash();

		void Mute(bool _mute);

	protected:
	private:

	// Variables
	public:
	protected:
	private:
		SoundSystem* m_pSoundSystem;

		TSound* m_pMusic_DNB;
		TSound* m_pEffect_splash_ps;
		TSound* m_pEffect_menu_nav;
		TSound* m_pEffect_menu_back;
		TSound* m_pEffect_menu_accept;
		TSound* m_pEffect_menu_tog;
		TSound* m_pEffect_player_hit;
		TSound* m_pEffect_player_death;
};

#endif