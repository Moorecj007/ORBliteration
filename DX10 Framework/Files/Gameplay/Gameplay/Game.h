/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Game.h
* Description : Runs all Game play processes
* Author :	Callan Moore
* Author :	Jc Fowles
* Mail :	Callan.Moore@mediadesign.school.nz
* Mail :	Jc.Fowles@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __GAME_H__
#define __GAME_H__

// Local Includes
#include "../../DX10/DX10/DX10_Renderer.h"
#include "Arena/ArenaFloor.h"
#include "Orbs\Orb.h"
#include "../../Input/InputGamePad.h"
#include "../../DX10/DX10.h"
#include "../../Sound/SoundManager.h"
#include "../../Menus/Menu.h"

enum eGameState
{
	GAME_STATE_START,
	GAME_STATE_RESTART,
	GAME_STATE_PROCESS,
	GAME_STATE_PAUSED,
	GAME_STATE_OPTIONS,
	GAME_STATE_ERROR,
	GAME_STATE_END
};

class Game
{
public:

	/***********************
	* Game: Default Constructor for Game class
	* @author: Callan Moore
	********************/
	Game();

	/***********************
	* ~Game: Default Destructor for Game class
	* @author: Callan Moore
	********************/
	~Game();

	/***********************
	* Initialise: Initialise the Game for use
	* @author: Callan Moore
	* @author: Jc Fowles
	* @parameter: _pDX10_Renderer: The renderer for this Game
	* @parameter: _pSoundManager: The Sound manager for the Game
	* @parameter: TO DO JUR: UI Manager
	* @parameter: _numPlayers: Number of players to play this match
	* @parameter: _AllowVibrate: Allows or dissAllows vibration
	* @return: bool: Successful or not
	********************/
	bool Initialise(DX10_Renderer* _pDX10_Renderer, SoundManager* _pSoundManager, DX10_Shader_Sprite* _pSpriteShader, int _numPlayers, bool _AllowVibrate, bool* _pKeyDown);
	
	// TO DO JC:
	bool Reset();


	/***********************
	* Process: Process the Game
	* @author: Jc Fowles
	* @parameter: _dt: The current delta tick
	* @return: void
	********************/
	bool Process(float _dt);
	
	/***********************
	* Render: Render the Game
	* @author: Jc Fowles
	* @return: void
	********************/
	void Render();

	/***********************
	* HandleInput: Handle the input for the given Orb
	* @author: Jc Fowles
	* @return: bool: Whether the contoler passed in is connected
	********************/
	bool HandleInput(int _playerNum);

	/***********************
	* IsOrbsColliding: Checks to see if the two Orbs collide
	* @author: Jc Fowles
	* @parameter: _pOrbA: Pointer to the first Orb to check
	* @parameter: _pOrbB: Pointer to the Second Orb to check
	* @return: void
	********************/
	bool IsOrbsColliding(Orb* _pOrbA, Orb* _pOrbB);

	/***********************
	* HandleCollisions: Processes the collisions between two Orbs
	* @author: Jc Fowles
	* @parameter: _pOrbA: Pointer to the first Orb to Process
	* @parameter: _pOrbB: Pointer to the Second Orb to Process
	* @return: void
	********************/
	void HandleCollisions(Orb* _pOrbA, Orb* _pOrbB);

	/***********************
	* KillOrb: Kills a Orb
	* @author: Jc Fowles
	* @parameter: _pOrb: The Orb to kill
	* @return: void
	********************/
	void KillOrb(Orb* _pOrb);

	/***********************
	* WinCheck: Checks the win condition of the game
	* @author: Jc Fowles
	* @return: void
	********************/
	void WinCheck();

	// TO DO CAL
	void UpdateClientSize();

	// TO DO - Juran
	bool AttachMenuComponents(Menu* _pPauseMenu, Menu* _pOptionsMenu, DXSprite* _pInstructionsUI, DXSprite* _pControllerUI);

private:

	DX10_Renderer* m_pDX10_Renderer;
	DX10_Shader_LitTex* m_pShader_LitTex;

	// Gmae States
	eGameState m_PreviousState;
	eGameState m_gameState;
	int m_roundNumber;

	bool m_allConnected;
		
	// Players
	int m_numPlayers;
	int m_numAlivePlayers;
	int m_PausedPlayer;
	int m_winner;
	int m_winningScore;

	// Timers
	bool m_firstProcess;
	float m_startCountDown;
	float m_matchTimer;

	// Contollers
	XButtonIDs m_XButtons;
	XStickDirectionIDs m_XStickDirections;
	std::vector<InputGamePad*> m_pContollers;

	float m_vibrateTimers[4];

	// Player Controller Orbs
	std::vector<Orb*> m_pOrbs;
	DX10_Mesh* m_pOrbMesh;

	// Arena
	ArenaFloor* m_pArenaFloor;
	
	SoundManager* m_pSoundManager;

	// TO DO JUR: below code to be removed when UI manager is implemented
	DX10_Shader_Sprite* m_pSpriteShader;

	// Menu Components
	Menu* m_pPauseMenu;

	// UI Components
	std::vector<DXSprite> m_uiPlayers;
	std::vector<DXSprite> m_uiVictory;
	DXSprite* m_pInstructions;
	DXSprite* m_uiControllerMissing;
	DXSprite m_number_first;
	DXSprite m_number_second;
	float m_uiScale;
	float m_uiWidth;
	float m_uiHeight;
	float m_uiSpace;
	bool* m_isConnected;
};
#endif	//__GAME_H__

