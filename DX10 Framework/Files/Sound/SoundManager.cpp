// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	:	SoundManager.cpp
// Description	:	The implementation file.
// Author(s)	:	Juran Griffith.
// Mail			:	juran.griffith@mediadesignschool.com
//

// Library Includes

// Local Includes
#include "SoundManager.h"

SoundManager::SoundManager()
{
	
}

SoundManager::~SoundManager()
{
	// Release the sound system and files
	ReleasePtr(m_pMusic_DNB);

	ReleasePtr(m_pEffect_splash_ps);
	ReleasePtr(m_pEffect_menu_nav);
	ReleasePtr(m_pEffect_menu_back);
	ReleasePtr(m_pEffect_menu_accept);
	ReleasePtr(m_pEffect_menu_tog);
	ReleasePtr(m_pEffect_player_hit);
	ReleasePtr(m_pEffect_player_death);

	ReleasePtr(m_pSoundSystem);
}

bool SoundManager::Intialise()
{
	// Initialize our sound system
	m_pSoundSystem = new SoundSystem();
	VALIDATE(m_pSoundSystem->Initialise());
	m_pSoundSystem->SetVolumeMusic(0.5f);
	m_pSoundSystem->SetVolumeEffects(1.0f);

	// Create the music
	m_pMusic_DNB = new TSound(); // TO DO - Juran (put in a vector)
	VALIDATE(m_pSoundSystem->LoadFile(m_pMusic_DNB, "tron_thebestdnbsongever.mp3", SOUND_TYPE_MUSIC));
	m_pMusic_DNB->SetLooped(true);

	// Create the effects
	m_pEffect_splash_ps = new TSound();
	VALIDATE(m_pSoundSystem->LoadFile(m_pEffect_splash_ps, "splash_phenomena.wav", SOUND_TYPE_EFFECT));

	m_pEffect_menu_nav = new TSound();
	m_pEffect_menu_back = new TSound();
	m_pEffect_menu_accept = new TSound();
	m_pEffect_menu_tog = new TSound();
	m_pEffect_player_hit = new TSound();
	m_pEffect_player_death = new TSound();

	m_pSoundSystem->LoadFile(m_pEffect_menu_accept, "tron_menuaccept.wav", SOUND_TYPE_EFFECT);
	m_pSoundSystem->LoadFile(m_pEffect_menu_back, "tron_menuback.wav", SOUND_TYPE_EFFECT);
	m_pSoundSystem->LoadFile(m_pEffect_menu_nav, "tron_menunavigation.wav", SOUND_TYPE_EFFECT);
	m_pSoundSystem->LoadFile(m_pEffect_menu_tog, "tron_menutoggles.wav", SOUND_TYPE_EFFECT);
	m_pSoundSystem->LoadFile(m_pEffect_player_death, "tron_playerdeath.wav", SOUND_TYPE_EFFECT);
	m_pSoundSystem->LoadFile(m_pEffect_player_hit, "tron_playerhit_lightsaber.wav", SOUND_TYPE_EFFECT);

	return true;
}

void SoundManager::PlaySong(UINT _songChoice)
{
	m_pMusic_DNB->Play();
}

void SoundManager::PlayMenuAccept()
{
	m_pSoundSystem->LoadFile(m_pEffect_menu_accept, m_pEffect_menu_accept->m_filename, SOUND_TYPE_EFFECT); // Have to reload the file to play it again.
	m_pEffect_menu_accept->Play();
}

void SoundManager::PlayMenuBack()
{
	m_pSoundSystem->LoadFile(m_pEffect_menu_back, m_pEffect_menu_back->m_filename, SOUND_TYPE_EFFECT);
	m_pEffect_menu_back->Play();
}

void SoundManager::PlayMenuNavigate()
{
	m_pSoundSystem->LoadFile(m_pEffect_menu_nav, m_pEffect_menu_nav->m_filename, SOUND_TYPE_EFFECT);
	m_pEffect_menu_nav->Play();
}

void SoundManager::PlayMenuToggles()
{
	m_pSoundSystem->LoadFile(m_pEffect_menu_tog, m_pEffect_menu_tog->m_filename, SOUND_TYPE_EFFECT);
	m_pEffect_menu_tog->Play();
}

void SoundManager::PlayPlayerDeath()
{
	m_pSoundSystem->LoadFile(m_pEffect_player_death, m_pEffect_player_death->m_filename, SOUND_TYPE_EFFECT);
	m_pEffect_player_death->Play();
}

void SoundManager::PlayPlayerHit()
{
	m_pSoundSystem->LoadFile(m_pEffect_player_hit, m_pEffect_player_hit->m_filename, SOUND_TYPE_EFFECT);
	m_pEffect_player_hit->Play();
}

void SoundManager::PlayPhenomenaSplash()
{
	m_pEffect_splash_ps->Play();
}

void SoundManager::Mute(bool _mute)
{
	m_pSoundSystem->MuteEffects(_mute);
	m_pSoundSystem->MuteMusic(_mute);
}