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
	float OrbRadius = 0.5f;
	m_pOrbMesh = new DX10_Mesh();
	TVertexNormalUV tempVertNormUV;
	v3float orbScale = { OrbRadius * 2, OrbRadius * 2, OrbRadius * 2 };
	VALIDATE(m_pOrbMesh->Initialise(m_pDX10_Renderer, MT_SPHERE, orbScale));

	// TO DO JC: This will be based on the number of players selected for a given match
	// Create the Controllers and the player orbs
	for (int i = 0; i < m_maxPlayers; i++)
	{
		m_pContollers.push_back(new InputGamePad());
		VALIDATE(m_pContollers[i]->Initialise(i + 1));

		m_pOrbs.push_back(new Orb());
		VALIDATE(m_pOrbs[i]->Initialise(m_pDX10_Renderer, m_pOrbMesh, m_pShader_LitTex, "flare.dds", 2.0f, 1.0f, 100.0f));
		m_pOrbs[i]->SetPosition({ (float(i)*5.0f), 0.0f, -2.0f });
	}
   
	// Create and Initialise the Arena Floor
	m_pArenaFloor = new ArenaFloor();
	m_tileScale = { 4, 4, 4 };
	m_areaSize = 15;
	VALIDATE(m_pArenaFloor->Initialise(m_pDX10_Renderer, m_pShader_LitTex, m_areaSize, m_tileScale, 90.0f));
	
	m_pArenaTiles = m_pArenaFloor->GetArenaTiles();

	return true;
}

bool Game::IsOrbsColliding(Orb* _OrbA, Orb* _OrbB)
{
	if ((_OrbA != _OrbB))
	{
		// Calculate the distance between the two orbs
		float distance = (_OrbA->GetPosition() - _OrbB->GetPosition()).Magnitude();
		// Calculate the combined Radius of the two orbs
		float combinedRadius = _OrbA->GetRadius() + _OrbB->GetRadius();

		

		// Check if the orbs are colliding
		if (distance <= combinedRadius)
		{
						 
			

			return true;

		}
		else
		{
			return false;
		}

	}
	else
	{
		return false;
	}
}

void Game::HandleCollisions(Orb* _OrbA, Orb* _OrbB)
{
	v3float orbVelocity_A = _OrbA->GetVelocity();
	v3float orbVelocity_B = _OrbB->GetVelocity();

	_OrbA->SetVelocity(orbVelocity_B * _OrbB->GetBounce());
	_OrbB->SetVelocity(orbVelocity_A * _OrbA->GetBounce());
}

void Game::Process(float _dt)
{

	HandleInput();

	m_pShader_LitTex->SetUpPerFrame();
	m_pArenaFloor->Process(_dt);
	 	
	/*for (UINT row = 0; row < m_pArenaTiles->size(); row++)
	{
		for (UINT col = 0; col < m_pArenaTiles->size(); col++)
		{
			(*(*m_pArenaTiles)[row])[col]->SetOverlayImage(OTI_BLANK);
		}
	}*/
	

	for (UINT i = 0; i < m_pOrbs.size(); i++)
	{
		if (m_pOrbs[i]->GetAlive())
		{
			// Get and set the surface friction
			

			// Check Collisions
			for (UINT j = 0; j < m_pOrbs.size(); j++)
			{
				if ((i != j))
				{
					if (m_pOrbs[j]->GetAlive())
					{
						if (IsOrbsColliding(m_pOrbs[i], m_pOrbs[j]))
						{
							HandleCollisions(m_pOrbs[i], m_pOrbs[j]);
						}
					}
				}
			}



			v3float OrbPos = m_pOrbs[i]->GetPosition();
			v3float tilePos;
			OrbPos += m_tileScale / 2;
			int row = (int)((OrbPos.x / m_tileScale.x) + ((m_areaSize - 1) / 2));
			int col = (int)((OrbPos.y / m_tileScale.y) + ((m_areaSize - 1) / 2));

			if (row < 0)
			{
				row = 0;
				m_pOrbs[i]->SetAlive(false);
			}
			else if (row > (int)m_pArenaTiles->size() - 1)
			{
				row = m_pArenaTiles->size() - 1;
				m_pOrbs[i]->SetAlive(false);
			}
			if (col < 0)
			{
				col = 0;
				m_pOrbs[i]->SetAlive(false);
			}
			else if (col > (int)m_pArenaTiles->size() - 1)
			{
				col = m_pArenaTiles->size() - 1;
				m_pOrbs[i]->SetAlive(false);
			}

			//(*(*m_pArenaTiles)[row])[col]->SetOverlayImage(OTI_POWER_CONFUSION);

			switch ((*(*m_pArenaTiles)[row])[col]->GetBaseImageEnum())
			{
			case BTI_SLIPPERY:
			{
				m_pOrbs[i]->SetSurfaceFriction(0.0f);
				//m_pOrbs[i]->SetSurfaceFriction(0.05f / _dt);
			}
				break;
			case BTI_ROUGH:
			{
				m_pOrbs[i]->SetSurfaceFriction(0.5f / _dt);
				//m_pOrbs[i]->SetSurfaceFriction(0.05f / _dt);
			}
				break;
			case BTI_STANDARD:
			{
				m_pOrbs[i]->SetSurfaceFriction(0.05f / _dt);
			}
				default: break;
			}
			

			if ((*(*m_pArenaTiles)[row])[col]->GetActive() == false)
			{
				m_pOrbs[i]->SetAlive(false);
			}




			m_pOrbs[i]->Process(_dt);
		}
	}
}

void Game::Render()
{
	m_pArenaFloor->Render();

	for (UINT i = 0; i < m_pOrbs.size(); i++)
	{
		if (m_pOrbs[i]->GetAlive())
		{
			m_pOrbs[i]->Render();
		}
	}
}

void Game::HandleInput()
{
	// Handle the Controller Inputs
	for (UINT i = 0; i < m_pContollers.size(); i++)
	{
		if (m_pOrbs[i]->GetAlive())
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

				m_pContollers[i]->PostProcess();
			}
		}
	}
}
