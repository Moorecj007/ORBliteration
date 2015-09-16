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
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __GAME_H__
#define __GAME_H__

// Local Includes
#include "../../DX10/DX10/DX10_Renderer.h"
#include "Arena/ArenaFloor.h"

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

	// TO DO CAL
	bool Initialise(DX10_Renderer* _pDX10_Renderer);
	void Process(float _dt);
	void Render();

private:
	DX10_Renderer* m_pDX10_Renderer;
	ArenaFloor* m_pArenaFloor;
	DX10_Shader_LitTex* m_pShader_LitTex;
};
#endif	//__GAME_H__

