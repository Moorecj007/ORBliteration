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
	* @parameter: _pSpriteShader: The sprite shader
	* @parameter: _numPlayers: Number of players to play this match
	* @parameter: _AllowVibrate: Allows or dissAllows vibration
	* @return: bool: Successful or not
	********************/
	bool Initialise(DX10_Renderer* _pDX10_Renderer, SoundManager* _pSoundManager, DX10_Shader_Sprite* _pSpriteShader, bool _AllowVibrate, bool* _pKeyDown);
	
	// TO DO JC:
	bool Reset(bool _full);


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
	* HandleInput: Handle the gampad input
	* @author: Jc Fowles & Juran
	* @return: void
	********************/
	void HandleInput();

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

	/*******************
	* UpdateClientSize: This updates the client size aswell as all the UI components to reposition them on window resize.
	* @author:	Juran Griffith.
	* @return:	void
	********************/
	void UpdateClientSize();

	/***********************
	* AttachUI: Attachs the instructions and controller ui
	* @author: Juran Griffith
	* @parameter: _pInstructionsUI: The instructions UI
	* @parameter: _pControllerUI: The controller UI
	* @return: bool: Checks if the parameters are vaild.
	********************/
	bool AttachUI(DXSprite* _pInstructionsUI, DXSprite* _pControllerUI);

	/***********************
	* AttachGamepad: Attachs a gamepad to the game.
	* @author: Juran Griffith
	* @parameter: _gamePad: The gamepad
	* @return: void.
	********************/
	void AttachGamepad(InputGamePad* _gamePad);

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

	// Player Controller Orbs
	std::vector<Orb*> m_pOrbs;
	DX10_Mesh* m_pOrbMesh;

	// Arena
	ArenaFloor* m_pArenaFloor;

	SoundManager* m_pSoundManager;
	DX10_Shader_Sprite* m_pSpriteShader;

	// UI Components
	std::vector<DXSprite> m_uiPlayers;
	DXSprite m_uiVictory;
	DXSprite m_uiRound;
	DXSprite* m_pInstructions;
	DXSprite* m_uiControllerMissing;
	DXSprite m_number_first;
	DXSprite m_number_second;

	POINT m_StartTimePos_A;
	POINT m_MatchTimePos_tens;
	POINT m_MatchTimePos_Units;
	POINT m_roundNumPos_tens;
	POINT m_roundNumPos_Units;


	float m_uiScale;
	float m_uiWidth;
	float m_uiHeight;
	float m_uiSpace;

	Menu* m_pPauseMenu;
};
#endif	//__GAME_H__

