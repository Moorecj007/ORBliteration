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
	while (!m_pMusic.empty())
	{
		ReleasePtr(m_pMusic.back());
		m_pMusic.pop_back();
	}

	ReleasePtr(m_pEffect_splash_ps);
	ReleasePtr(m_pEffect_menu_nav);
	ReleasePtr(m_pEffect_menu_back);
	ReleasePtr(m_pEffect_menu_accept);
	ReleasePtr(m_pEffect_menu_tog);
	ReleasePtr(m_pEffect_player_hit);
	ReleasePtr(m_pEffect_player_death);
	ReleasePtr(m_pEffect_player_phase);
	ReleasePtr(m_pEffect_player_boost);

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
	m_pMusic.push_back(new TSound());
	VALIDATE(m_pSoundSystem->LoadFile(m_pMusic.back(), "tron_thebestdnbsongever.mp3", SOUND_TYPE_MUSIC));
	m_pMusic.back()->SetLooped(true);

	m_pMusic.push_back(new TSound());
	VALIDATE(m_pSoundSystem->LoadFile(m_pMusic.back(), "tron_intothefire.mp3", SOUND_TYPE_MUSIC));
	m_pMusic.back()->SetLooped(true);

	m_pMusic.push_back(new TSound());
	VALIDATE(m_pSoundSystem->LoadFile(m_pMusic.back(), "tron_victory.mp3", SOUND_TYPE_MUSIC));
	m_pMusic.back()->SetLooped(true);

	// Create the effects
	m_pEffect_splash_ps = new TSound();
	m_pEffect_menu_nav = new TSound();
	m_pEffect_menu_back = new TSound();
	m_pEffect_menu_accept = new TSound();
	m_pEffect_menu_tog = new TSound();
	m_pEffect_player_hit = new TSound();
	m_pEffect_player_death = new TSound();
	m_pEffect_player_phase = new TSound();
	m_pEffect_player_boost = new TSound();

	VALIDATE(m_pSoundSystem->LoadFile(m_pEffect_splash_ps, "splash_phenomena.wav", SOUND_TYPE_EFFECT));
	VALIDATE(m_pSoundSystem->LoadFile(m_pEffect_menu_accept, "tron_menuaccept.wav", SOUND_TYPE_EFFECT));
	VALIDATE(m_pSoundSystem->LoadFile(m_pEffect_menu_back, "tron_menuback.wav", SOUND_TYPE_EFFECT));
	VALIDATE(m_pSoundSystem->LoadFile(m_pEffect_menu_nav, "tron_menunavigation.wav", SOUND_TYPE_EFFECT));
	VALIDATE(m_pSoundSystem->LoadFile(m_pEffect_menu_tog, "tron_menutoggles.wav", SOUND_TYPE_EFFECT));
	VALIDATE(m_pSoundSystem->LoadFile(m_pEffect_player_death, "tron_playerdeath.wav", SOUND_TYPE_EFFECT));
	VALIDATE(m_pSoundSystem->LoadFile(m_pEffect_player_hit, "tron_playerhit_lightsaber.wav", SOUND_TYPE_EFFECT));
	VALIDATE(m_pSoundSystem->LoadFile(m_pEffect_player_phase, m_pEffect_player_hit->m_filename, SOUND_TYPE_EFFECT));
	VALIDATE(m_pSoundSystem->LoadFile(m_pEffect_player_boost, m_pEffect_player_hit->m_filename, SOUND_TYPE_EFFECT));

	return true;
}

void SoundManager::Update()
{
	m_pSoundSystem->Update();
}

void SoundManager::PlaySong(UINT _songChoice)
{
	for (UINT i = 0; i < m_pMusic.size(); ++i)
	{
		if (i != _songChoice)
		{
			m_pMusic[i]->Pause();
			m_pMusic[i]->Reset();
		}
	}

	if (_songChoice >= 0 && _songChoice < m_pMusic.size())
	{
		if (m_pMusic[_songChoice]->IsPaused())
		{
			m_pMusic[_songChoice]->Play();
		}
	}
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

void SoundManager::PlayPlayerPhase()
{
	m_pSoundSystem->LoadFile(m_pEffect_player_phase, m_pEffect_player_phase->m_filename, SOUND_TYPE_EFFECT);
	m_pEffect_player_phase->Play();
}

void SoundManager::PlayPlayerBoost()
{
	m_pSoundSystem->LoadFile(m_pEffect_player_boost, m_pEffect_player_boost->m_filename, SOUND_TYPE_EFFECT);
	m_pEffect_player_boost->Play();
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