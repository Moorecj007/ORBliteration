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

	// TO DO CAL: To move to a shut down

	while (m_pContollers.empty() == false)
	{
		ReleasePtr(m_pContollers.back());
		m_pContollers.pop_back();
	}

	while (m_pOrbs.empty() == false)
	{
		ReleasePtr(m_pOrbs.back());
		m_pOrbs.pop_back();
	}

	ReleasePtr(m_pOrbMesh);

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

	 // Create the Orb Mesh
	m_pOrbMesh = new DX10_Mesh_Rect_Prism();
	TVertexNormalUV tempVertNormUV;
	v3float orbScale = { 3, 3, 3 };
	VALIDATE(m_pOrbMesh->Initialise(m_pDX10_Renderer, tempVertNormUV, orbScale));

	// TO DO JC: This will be based on the number of players selected for a given match
	// Create the Contollers and the player orbs
	for (int i = 0; i < m_maxPlayers; i++)
	{
		m_pContollers.push_back(new InputGamePad());
		VALIDATE(m_pContollers[i]->Initialise(i + 1));

		m_pOrbs.push_back(new Orb());
		VALIDATE(m_pOrbs[i]->Initialise(m_pDX10_Renderer, m_pOrbMesh, m_pShader_LitTex, "flare.dds", 1.0f,1.0f, 10.0f));
		m_pOrbs[i]->SetPosition({ (float(i)*5.0f), 0.0f, -2.0f });
	}

	// Create and Initialise the Arena Floor
	m_pArenaFloor = new ArenaFloor();
	v3float tileScale = { 4, 4, 0.1f };
	VALIDATE(m_pArenaFloor->Initialise(m_pDX10_Renderer, m_pShader_LitTex, 15, tileScale, 15.0f));
	
	return true;
}

void Game::Process(float _dt)
{

	HandleInput();

	m_pShader_LitTex->SetUpPerFrame();
	m_pArenaFloor->Process(_dt);

	for (UINT i = 0; i < m_pOrbs.size(); i++)
	{
		m_pOrbs[i]->Process(_dt);
	}
}

void Game::Render()
{
	m_pArenaFloor->Render();

	for (UINT i = 0; i < m_pOrbs.size(); i++)
	{
		m_pOrbs[i]->Render();
	}
}

void Game::HandleInput()
{
	// Handle the Controller Inputs
	for (UINT i = 0; i < m_pContollers.size(); i++)
	{
		if (m_pContollers[i]->Connected())
		{
			m_pContollers[i]->PreProcess();
			
			v2float LeftAxis;

			if (!m_pContollers[i]->LStick_InDeadZone())
			{
				LeftAxis = m_pContollers[i]->GetLStickAxis();
				m_pOrbs[i]->SetAcceleration({ LeftAxis.x, LeftAxis.y, 0.0f });
			}
			else
			{
				//LeftAxis = { 0.0f, 0.0f };
				//
				//v3float OrbAccel;// = m_pOrbs[i]->GetAcceleration();
				//v3float OrbVelocity = m_pOrbs[i]->GetVelocity();
				//OrbAccel = (OrbVelocity / 2.0f) * -1.0f;
				//m_pOrbs[i]->SetAcceleration(OrbAccel);
				m_pOrbs[i]->Decelerate();
			}

		

			m_pContollers[i]->PostProcess();
		}
	}
}