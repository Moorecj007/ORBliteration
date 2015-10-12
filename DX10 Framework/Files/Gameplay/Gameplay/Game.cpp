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
* Author :	Jc Fowles
* Mail :	Callan.Moore@mediadesign.school.nz
* Mail :	Jc.Fowles@mediadesign.school.nz
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

bool Game::Initialise(DX10_Renderer* _pDX10_Renderer, SoundManager* _pSoundManager, DX10_Shader_Sprite* _pSpriteShader, int _numPlayers)
{
	if ((_pDX10_Renderer == 0) || (_pSoundManager == 0))
	{
		// If the Renderer or the sound manger is NULL the Game cannot be utilized
		return false;
	}

	m_numPlayers = _numPlayers;
	m_numAlivePlayers = _numPlayers;
	m_pDX10_Renderer = _pDX10_Renderer;
	m_pSoundManager = _pSoundManager;

	m_matchWon = false;

	// TO DO JUR: Temp all to be Remove
	m_pSpriteShader = _pSpriteShader;
	VictroyPlayerOne = new DXSprite();
	VictroyPlayerOne->Initialise(m_pDX10_Renderer, m_pSpriteShader, "Tron/UI/Tron_Victory_P1.png", 2280, 601, 3, 1);
	VictroyPlayerOne->SetSize(800, 800);
	VictroyPlayerOne->SetPosition(100,100);

	// Create the Shader for the Game Objects
	m_pShader_LitTex = new DX10_Shader_LitTex();
	VALIDATE(m_pShader_LitTex->Initialise(m_pDX10_Renderer));

	// Create and Initialise the Arena Floor
	m_pArenaFloor = new ArenaFloor();
	m_tileScale = { 4, 4, 4 };
	m_areaSize = 15;
	VALIDATE(m_pArenaFloor->Initialise(m_pDX10_Renderer, m_pShader_LitTex, m_areaSize, m_tileScale, 90.0f));

	m_pArenaTiles = m_pArenaFloor->GetArenaTiles();


	// Create the Orb Mesh
	float OrbRadius = 2.0f;
	m_pOrbMesh = new DX10_Mesh;
	v3float orbScale = { OrbRadius * 2, OrbRadius * 2, OrbRadius * 2 };
	VALIDATE(m_pOrbMesh->Initialise(m_pDX10_Renderer, MT_SPHERE, orbScale));

	
	// Create the Controllers and the player orbs
	std::string temp;
	for (int i = 0; i < m_numPlayers; i++)
	{
		m_pContollers.push_back(new InputGamePad());
		VALIDATE(m_pContollers[i]->Initialise(i + 1));

		m_pOrbs.push_back(new Orb());
		int row, col;
		
		switch (i)
		{
		case 0:
			{
				temp = "pBall.png";
				row = 1;
				col = 13;
	
			}
			break;
		case 1:
			{
				temp = "gBall.png";
				row = 1;
				col = 1;
			}
			break;
		case 2:
			{
				temp = "Tron/Tile/tron_tile_green.png";
				row = 13;
				col = 13;
			}
			break;
		case 3:
			{
				temp = "Tron/Tile/tron_tile_white.png";
				row = 13;
				col = 1;
			}
			break;
		}
		VALIDATE(m_pOrbs[i]->Initialise(m_pDX10_Renderer, m_pOrbMesh, m_pShader_LitTex, temp, 2.0f, 1.0f, 100.0f));
		
		// Set the Orbs Postions
		v3float	OrbPos = (*(*m_pArenaTiles)[row])[col]->GetPosition();
		OrbPos.z = -2.0f;
		m_pOrbs[i]->SetPosition(OrbPos);

		//VALIDATE(m_pContollers[i]->Connected());
	}
   


	// TO DO JUR: remove to  UI Temp


	return true;
}

bool Game::IsOrbsColliding(Orb* _pOrbA, Orb* _pOrbB)
{
	if ((_pOrbA != _pOrbB))
	{
		if (_pOrbA->GetPhase() == _pOrbB->GetPhase())
		{
			// Calculate the distance between the two orbs
			float distance = (_pOrbA->GetPosition() - _pOrbB->GetPosition()).Magnitude();
			// Calculate the combined Radius of the two orbs
			float combinedRadius = _pOrbA->GetRadius() + _pOrbB->GetRadius();

			// Check if the orbs are colliding
			if (distance < combinedRadius)
			{
				
				// Move objects out of colliding with each other
				while (distance < combinedRadius)
				{
					// Calculate Peirce
					float peirce = combinedRadius - distance;

					// Calculate the direction of collision
					v3float Direction = (_pOrbA->GetPosition() - _pOrbB->GetPosition());
			
					_pOrbA->SetPosition(_pOrbA->GetPosition() + (Direction *  peirce));
					_pOrbB->SetPosition(_pOrbB->GetPosition() + (Direction *  -peirce));
					
					// Calculate the distance between the two orbs
					distance = (_pOrbA->GetPosition() - _pOrbB->GetPosition()).Magnitude();
					// Calculate the combined Radius of the two orbs
					combinedRadius = _pOrbA->GetRadius() + _pOrbB->GetRadius();
				}
				

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
	else
	{
		return false;
	}
}

void Game::HandleCollisions(Orb* _pOrbA, Orb* _pOrbB)
{
	v3float orbVelocity_A = _pOrbA->GetVelocity();
	v3float orbVelocity_B = _pOrbB->GetVelocity();

	float velocityMax = orbVelocity_A.Magnitude();

	if (velocityMax < orbVelocity_B.Magnitude())
	{
		_pOrbA->SetVelocity((orbVelocity_B * _pOrbB->GetBounce()));
		// TO DO JC: Take a small multiple of of the other orbs velocity 
		_pOrbB->SetVelocity({ 0.0f, 0.0f, 0.0f });
	}
	else
	{
		_pOrbA->SetVelocity({ 0.0f, 0.0f, 0.0f });
		// TO DO JC: Take a small multiple of of the other orbs velocity 
		_pOrbB->SetVelocity((orbVelocity_A * _pOrbA->GetBounce()));

	}

}

void Game::KillOrb(Orb* _pOrb)
{
	if (_pOrb->GetAlive())
	{
		_pOrb->SetAlive(false);
		m_numAlivePlayers--;
	}
}

bool Game::Process(float _dt)
{
	// Set up the shader
	m_pShader_LitTex->SetUpPerFrame();

	// Process Inputs
	//HandleInput();

	// Game has Ended
	if (m_numAlivePlayers <= 1)
	{
		int Winner = 0;
		if (m_numAlivePlayers == 1)
		{
			// Game has Ended with a Winner 
			for (UINT i = 0; i < m_pOrbs.size(); i++)
			{
				if (m_pOrbs[i]->GetAlive())
				{
					Winner = i + 1;
					break;
				}
			}
		}

		if (Winner == 0)
		{
			// No Winner the Game is a Draw
		}
		else
		{
			m_matchWon = true;
			// Player 'Winner' has won
		}
		// This Player has won
		// TO DO JUR: Print Winner and Press A to Continue

		for (UINT i = 0; i < m_pContollers.size(); i++)
		{
			m_pContollers[i]->PreProcess();
			if (m_pContollers[i]->GetButtonPressed(m_XButtons.ActionButton_A))
			{
				// Return the game end
				return false;
			}
			m_pContollers[i]->PostProcess();
		}
		
	}
	else // Game Still running
	{
		
		// Process the arena Floor
		m_pArenaFloor->Process(_dt);

		// Process the Orbs
		for (UINT i = 0; i < m_pOrbs.size(); i++)
		{
			if (m_pOrbs[i]->GetAlive())
			{
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

				// Calculate the tile the Orb is on
				v3float OrbPos = m_pOrbs[i]->GetPosition();
				OrbPos += m_tileScale / 2;
				int row = (int)((OrbPos.x / m_tileScale.x) + ((m_areaSize - 1) / 2));
				int col = (int)((OrbPos.y / m_tileScale.y) + ((m_areaSize - 1) / 2));

				// Check if the orb is with in the Arena if not Kill it
				
				// Check if it in the bounds of the Rows
				if (row < 0)
				{
					row = 0;
					KillOrb(m_pOrbs[i]);
				}
				else if (row > (int)m_pArenaTiles->size() - 1)
				{
					row = m_pArenaTiles->size() - 1;
					KillOrb(m_pOrbs[i]);
				}
				// Check if it in the bounds of the Columns
				if (col < 0)
				{
					col = 0;
					KillOrb(m_pOrbs[i]);
				}
				else if (col > (int)m_pArenaTiles->size() - 1)
				{
					col = m_pArenaTiles->size() - 1;
					KillOrb(m_pOrbs[i]);
				}

				// Kill Orb if its on a Dead Tile
				if ((*(*m_pArenaTiles)[row])[col]->GetActive() == false)
				{
					KillOrb(m_pOrbs[i]);
				}
				// Orb is still alive 
				else
				{
					// Set the tile the Orb is On
					m_pOrbs[i]->SetTile((*(*m_pArenaTiles)[row])[col]);
				}
				
				HandleInput(i);

				m_pOrbs[i]->Process(_dt);
			}
		}
	}

	return true;
}

void Game::Render()
{
	m_pArenaFloor->Render();

	// Draw all un phased Orbs
	for (UINT i = 0; i < m_pOrbs.size(); i++)
	{
		if (m_pOrbs[i]->GetAlive())
		{
			if (m_pOrbs[i]->GetPhase() == false)
			{
				m_pOrbs[i]->Render();
			}
		}
	}

	// Draw all phased Orbs
	for (UINT i = 0; i < m_pOrbs.size(); i++)
	{
		if (m_pOrbs[i]->GetAlive())
		{
			if (m_pOrbs[i]->GetPhase() == true)
			{
				m_pOrbs[i]->Render();
			}
		}
	}

	if (m_matchWon)
	{
		m_pDX10_Renderer->TurnZBufferOff();

		VictroyPlayerOne->Render();

		m_pDX10_Renderer->TurnZBufferOn();
	}
}

void Game::HandleInput(int _playerNum)
{
	if (m_pOrbs[_playerNum]->GetAlive())
	{
		if (m_pContollers[_playerNum]->Connected())
		{
			m_pContollers[_playerNum]->PreProcess();

			// Movement
			v2float LeftAxis;
			if (!m_pContollers[_playerNum]->LStick_InDeadZone())
			{
				LeftAxis = m_pContollers[_playerNum]->GetLStickAxis();
				m_pOrbs[_playerNum]->SetAcceleration({ LeftAxis.x, LeftAxis.y, 0.0f });
			}

			// Boost
			m_pOrbs[_playerNum]->Boost(m_pContollers[_playerNum]->GetButtonPressed(m_XButtons.Bumper_R));
				
			m_pOrbs[_playerNum]->Phase(m_pContollers[_playerNum]->GetButtonPressed(m_XButtons.Bumper_L));

			m_pContollers[_playerNum]->PostProcess();
		}
	}
	else
	{
		// 
	}
		
	
}
