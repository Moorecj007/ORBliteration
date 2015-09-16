/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Game.cpp
* Description : Runs all Game play processes
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
	ReleasePtr(m_pArenaFloor);
	ReleasePtr(m_pShader_LitTex);
}

bool Game::Initialise(DX10_Renderer* _pDX10_Renderer)
{
	if (_pDX10_Renderer == 0)
	{
		// If the Renderer is NULL the Game cannot be utilized
		return false;
	}

	m_pDX10_Renderer = _pDX10_Renderer;

	// Create the Shader for the Game Objects
	m_pShader_LitTex = new DX10_Shader_LitTex();
	VALIDATE(m_pShader_LitTex->Initialise(m_pDX10_Renderer));

	// Create and Initialise the Arena Floor
	m_pArenaFloor = new ArenaFloor();
	v3float tileScale = { 10, 10, 1 };
	VALIDATE(m_pArenaFloor->Initialise(m_pDX10_Renderer, m_pShader_LitTex, 21, 21, tileScale));
	
	return true;
}

void Game::Process(float _dt)
{
	m_pShader_LitTex->SetUpPerFrame();
	m_pArenaFloor->Process(_dt);
}

void Game::Render()
{
	m_pArenaFloor->Render();
}