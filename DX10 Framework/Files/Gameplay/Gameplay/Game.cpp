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
	m_uiScale = 0.5f;
	m_uiWidth = 671.0f;
	m_uiHeight = 365.0f;
	m_uiSpace = 10.0f;
}

Game::~Game()
{
	ReleasePtr(m_pArenaFloor);
	ReleasePtr(m_pShader_LitTex);
	ReleasePtr(m_isConnected);

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

bool Game::Initialise(DX10_Renderer* _pDX10_Renderer, SoundManager* _pSoundManager, DX10_Shader_Sprite* _pSpriteShader, int _numPlayers, bool _AllowVibrate, bool* _pKeyDown)
{
	if ((_pDX10_Renderer == 0) || (_pSoundManager == 0) || (_pKeyDown == 0))
	{
		// If the Renderer or the sound manger is NULL the Game cannot be utilized
		return false;
	}

	// Set Timers
	m_matchTimer = 30.0f;
	m_startCountDown = 3.0f;
	m_firstProcess = true;

	m_allConnected = true;

	m_gameState = GAME_STATE_START;

	m_numPlayers = _numPlayers;
	m_numAlivePlayers = _numPlayers;
	m_pDX10_Renderer = _pDX10_Renderer;
	m_pSoundManager = _pSoundManager;

	// TO DO JUR: Temp all to be Remove
	m_isConnected = new bool[_numPlayers];

	m_pSpriteShader = _pSpriteShader;

	UINT victoryWidth = 760;
	UINT victoryHeight = 601;
	float xoffset = static_cast<float>(m_pDX10_Renderer->GetWidth()) / 2.0f;
	float yoffset = static_cast<float>(m_pDX10_Renderer->GetHeight()) / 2.0f;

	VALIDATE(m_number_first.Initialise(m_pDX10_Renderer, m_pSpriteShader, "Tron/UI/tron_numbers_fill.png", 1060, 424, 10, 4));
	m_number_first.SetSize(106.0f * m_uiScale, 106.0f * m_uiScale);
	//m_number_first.SetScale(m_uiScale);
	m_number_first.SetPosition(xoffset - 25.0f, 50.0f);

	VALIDATE(m_number_second.Initialise(m_pDX10_Renderer, m_pSpriteShader, "Tron/UI/tron_numbers_fill.png", 1060, 424, 10, 4));
	m_number_second.SetSize(106.0f * m_uiScale, 106.0f * m_uiScale);
	//m_number_second.SetScale(m_uiScale);
	m_number_second.SetPosition(xoffset + 25.0f, 50.0f);
	
	// Create the Shader for the Game Objects
	m_pShader_LitTex = new DX10_Shader_LitTex();
	VALIDATE(m_pShader_LitTex->Initialise(m_pDX10_Renderer));

	// Create and Initialise the Arena Floor
	m_pArenaFloor = new ArenaFloor();
	VALIDATE(m_pArenaFloor->Initialise(m_pDX10_Renderer, m_pShader_LitTex, 15, { 4.8f, 4.8f, 4.8f }, m_matchTimer));

	// Create the Orb Mesh
	float OrbRadius = 3.0f;
	m_pOrbMesh = new DX10_Mesh;
	v3float orbScale = { OrbRadius * 2, OrbRadius * 2, OrbRadius * 2 };
	VALIDATE(m_pOrbMesh->Initialise(m_pDX10_Renderer, MT_SPHERE, orbScale));

	// Create the Controllers and the player orbs
	std::string temp;
	for (int i = 0; i < m_numPlayers; i++)
	{
		m_pContollers.push_back(new InputGamePad());
		VALIDATE(m_pContollers[i]->Initialise((i + 1), _AllowVibrate));
		m_vibrateTimers[i] = 0.0f;

		if(m_pContollers[i]->Connected() == false)
		{
			m_allConnected = false;
			m_gameState = GAME_STATE_ERROR;
		}

		m_pOrbs.push_back(new Orb());
		int row, col;

		switch (i)
		{
			case 0:
			{
				row = 1;
				col = 13;

				DXSprite uiPlayer1;

				VALIDATE(uiPlayer1.Initialise(m_pDX10_Renderer, m_pSpriteShader, "Tron/UI/tron_ui_p1.png", (UINT)m_uiWidth, (UINT)m_uiHeight));
				uiPlayer1.SetPosition(m_uiSpace, m_uiSpace);
				uiPlayer1.SetSize(m_uiWidth * m_uiScale, m_uiHeight * m_uiScale);
				m_uiPlayers.push_back(uiPlayer1);

				DXSprite uiVictory;

				VALIDATE(uiVictory.Initialise(m_pDX10_Renderer, m_pSpriteShader, "Tron/UI/tron_victory_p1.png", victoryWidth, victoryHeight));
				uiVictory.SetPosition(xoffset - victoryWidth * 0.5f, yoffset - victoryHeight * 0.5f);
				uiVictory.SetSize((float)victoryWidth, (float)victoryHeight);

				m_uiVictory.push_back(uiVictory);
			}
			break;
			case 1:
			{
				row = 13;
				col = 1;

				DXSprite uiPlayer2;
				VALIDATE(uiPlayer2.Initialise(m_pDX10_Renderer, m_pSpriteShader, "Tron/UI/tron_ui_p2.png", (UINT)m_uiWidth, (UINT)m_uiHeight));
				uiPlayer2.SetPosition(m_pDX10_Renderer->GetWidth() - (m_uiWidth * m_uiScale) - m_uiSpace, m_pDX10_Renderer->GetHeight() - (m_uiHeight * m_uiScale) - m_uiSpace);
				uiPlayer2.SetSize(m_uiWidth * m_uiScale, m_uiHeight * m_uiScale);
				m_uiPlayers.push_back(uiPlayer2);

				DXSprite uiVictory;

				VALIDATE(uiVictory.Initialise(m_pDX10_Renderer, m_pSpriteShader, "Tron/UI/tron_victory_p2.png", victoryWidth, victoryHeight));
				uiVictory.SetPosition(xoffset - victoryWidth * 0.5f, yoffset - victoryHeight * 0.5f);
				uiVictory.SetSize((float)victoryWidth, (float)victoryHeight);

				m_uiVictory.push_back(uiVictory);
			}
			break;
			case 2:
			{
				row = 13;
				col = 13;

				DXSprite uiPlayer3;
				VALIDATE(uiPlayer3.Initialise(m_pDX10_Renderer, m_pSpriteShader, "Tron/UI/tron_ui_p3.png", (UINT)m_uiWidth, (UINT)m_uiHeight));
				uiPlayer3.SetPosition(m_pDX10_Renderer->GetWidth() - (m_uiWidth * m_uiScale) - m_uiSpace, m_uiSpace);
				uiPlayer3.SetSize(m_uiWidth * m_uiScale, m_uiHeight * m_uiScale);
				m_uiPlayers.push_back(uiPlayer3);

				DXSprite uiVictory;

				VALIDATE(uiVictory.Initialise(m_pDX10_Renderer, m_pSpriteShader, "Tron/UI/tron_victory_p3.png", victoryWidth, victoryHeight));
				uiVictory.SetPosition(xoffset - victoryWidth * 0.5f, yoffset - victoryHeight * 0.5f);
				uiVictory.SetSize((float)victoryWidth, (float)victoryHeight);

				m_uiVictory.push_back(uiVictory);
			}
			break;
			case 3:
			{
				row = 1;
				col = 1;

				DXSprite uiPlayer4;
				VALIDATE(uiPlayer4.Initialise(m_pDX10_Renderer, m_pSpriteShader, "Tron/UI/tron_ui_p4.png", (UINT)m_uiWidth, (UINT)m_uiHeight));
				uiPlayer4.SetPosition(m_uiSpace, m_pDX10_Renderer->GetHeight() - (m_uiHeight * m_uiScale) - m_uiSpace);
				uiPlayer4.SetSize(m_uiWidth * m_uiScale, m_uiHeight * m_uiScale);
				m_uiPlayers.push_back(uiPlayer4);

				DXSprite uiVictory;

				VALIDATE(uiVictory.Initialise(m_pDX10_Renderer, m_pSpriteShader, "Tron/UI/tron_victory_p4.png", victoryWidth, victoryHeight));
				uiVictory.SetPosition(xoffset - victoryWidth * 0.5f, yoffset - victoryHeight * 0.5f);
				uiVictory.SetSize((float)victoryWidth, (float)victoryHeight);

				m_uiVictory.push_back(uiVictory);
			}
			break;
		}

		// Set the Orbs Positions
		v3float	OrbPos = m_pArenaFloor->GetTilePos(row, col);
		OrbPos.z = -OrbRadius;
		m_pOrbs[i]->SetPosition(OrbPos);

		VALIDATE(m_pOrbs[i]->Initialise(m_pDX10_Renderer, m_pOrbMesh, m_pShader_LitTex, (i + 1), 1.0f, 0.2f));
				
		m_pOrbs[i]->Process(0.016f);

	

	}
		
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
				//while (distance < combinedRadius)
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
				// No Collision
				return false;
			}
		}
		else
		{
			// No Collision
			return false;
		}
	}
	else
	{
		// No Collision
		return false;
	}
}

void Game::HandleCollisions(Orb* _pOrbA, Orb* _pOrbB)
{
	// TO DO CAL: COMMENT and clean
	// 2 functions 
	// CalcNewVelocity()
	// RotateRoundZAxis()

	if ((_pOrbA != 0) && (_pOrbB != 0))
	{
		if (_pOrbA->m_collidable == true && _pOrbB->m_collidable == true)
		{
			_pOrbA->m_collidable = false;
			_pOrbA->m_collideCountdown = _pOrbA->m_collideStartTime;
			_pOrbB->m_collidable = false;
			_pOrbB->m_collideCountdown = _pOrbB->m_collideStartTime;


			v3float orbPosA = _pOrbA->GetPosition();
			v3float orbPosB = _pOrbB->GetPosition();

			v3float dir = orbPosB - orbPosA;
			v3float impactPoint = orbPosA + (dir / 2.0f);


			// ORB A
			v3float lineVecTemp = orbPosA - impactPoint;
			v3float lineVec;
			lineVec.x = lineVecTemp.x * cos(DegreesToRadians(90)) - lineVecTemp.y * sin(DegreesToRadians(90));
			lineVec.y = lineVecTemp.x * sin(DegreesToRadians(90)) + lineVecTemp.y * cos(DegreesToRadians(90));
			lineVec = lineVec.Normalise();

			v3float orbVelNormA = _pOrbA->GetVelocity().Normalise();
			float orbVelMagA = _pOrbA->GetVelocity().Magnitude();
			float orbImpactAngleA = RadiansToDegrees(acos(orbVelNormA.x * lineVec.x + orbVelNormA.y * lineVec.y + orbVelNormA.z * lineVec.z));

			v3float orbNewVelA;
			if (orbImpactAngleA > 90.0f)
			{
				orbNewVelA.x = lineVec.x * cos(DegreesToRadians(orbImpactAngleA)) - lineVec.y * sin(DegreesToRadians(orbImpactAngleA));
				orbNewVelA.y = lineVec.x * sin(DegreesToRadians(orbImpactAngleA)) + lineVec.y * cos(DegreesToRadians(orbImpactAngleA));

				if (orbNewVelA.Normalise().ApproxEqual(orbVelNormA.Normalise(), 0.0001f))
				{
					orbNewVelA.x = lineVec.x * cos(DegreesToRadians(-orbImpactAngleA)) - lineVec.y * sin(DegreesToRadians(-orbImpactAngleA));
					orbNewVelA.y = lineVec.x * sin(DegreesToRadians(-orbImpactAngleA)) + lineVec.y * cos(DegreesToRadians(-orbImpactAngleA));
				}
			}
			else
			{
				orbNewVelA.x = lineVec.x * cos(DegreesToRadians(-orbImpactAngleA)) - lineVec.y * sin(DegreesToRadians(-orbImpactAngleA));
				orbNewVelA.y = lineVec.x * sin(DegreesToRadians(-orbImpactAngleA)) + lineVec.y * cos(DegreesToRadians(-orbImpactAngleA));

				if (orbNewVelA.Normalise().ApproxEqual(orbVelNormA.Normalise(), 0.0001f))
				{
					orbNewVelA.x = lineVec.x * cos(DegreesToRadians(orbImpactAngleA)) - lineVec.y * sin(DegreesToRadians(orbImpactAngleA));
					orbNewVelA.y = lineVec.x * sin(DegreesToRadians(orbImpactAngleA)) + lineVec.y * cos(DegreesToRadians(orbImpactAngleA));
				}
			}

			// ORB B
			lineVecTemp = orbPosB - impactPoint;
			lineVec.x = lineVecTemp.x * cos(DegreesToRadians(90)) - lineVecTemp.y * sin(DegreesToRadians(90));
			lineVec.y = lineVecTemp.x * sin(DegreesToRadians(90)) + lineVecTemp.y * cos(DegreesToRadians(90));
			lineVec = lineVec.Normalise();

			v3float orbVelNormB = _pOrbB->GetVelocity().Normalise();
			float orbVelMagB = _pOrbB->GetVelocity().Magnitude();
			float orbImpactAngleB = RadiansToDegrees(acos(orbVelNormB.x * lineVec.x + orbVelNormB.y * lineVec.y + orbVelNormB.z * lineVec.z));

			v3float orbNewVelB;
			if (orbImpactAngleB > 90.0f)
			{
				orbNewVelB.x = lineVec.x * cos(DegreesToRadians(orbImpactAngleB)) - lineVec.y * sin(DegreesToRadians(orbImpactAngleB));
				orbNewVelB.y = lineVec.x * sin(DegreesToRadians(orbImpactAngleB)) + lineVec.y * cos(DegreesToRadians(orbImpactAngleB));

				if (orbNewVelB.Normalise().ApproxEqual(orbVelNormB.Normalise(), 0.0001f))
				{
					orbNewVelB.x = lineVec.x * cos(DegreesToRadians(-orbImpactAngleB)) - lineVec.y * sin(DegreesToRadians(-orbImpactAngleB));
					orbNewVelB.y = lineVec.x * sin(DegreesToRadians(-orbImpactAngleB)) + lineVec.y * cos(DegreesToRadians(-orbImpactAngleB));
				}
			}
			else
			{
				orbNewVelB.x = lineVec.x * cos(DegreesToRadians(-orbImpactAngleB)) - lineVec.y * sin(DegreesToRadians(-orbImpactAngleB));
				orbNewVelB.y = lineVec.x * sin(DegreesToRadians(-orbImpactAngleB)) + lineVec.y * cos(DegreesToRadians(-orbImpactAngleB));

				if (orbNewVelB.Normalise().ApproxEqual(orbVelNormB.Normalise(), 0.0001f))
				{
					orbNewVelB.x = lineVec.x * cos(DegreesToRadians(orbImpactAngleB)) - lineVec.y * sin(DegreesToRadians(orbImpactAngleB));
					orbNewVelB.y = lineVec.x * sin(DegreesToRadians(orbImpactAngleB)) + lineVec.y * cos(DegreesToRadians(orbImpactAngleB));
				}
			}

			_pOrbA->SetVelocity(orbNewVelA * orbVelMagB * 3.0f);
			_pOrbB->SetVelocity(orbNewVelB * orbVelMagA * 3.0f);
		}
	}
}

void Game::KillOrb(Orb* _pOrb)
{
	if (_pOrb->GetAlive())
	{
		_pOrb->SetAlive(false);
		m_numAlivePlayers--;
		m_pSoundManager->PlayPlayerDeath();
	}
}

void Game::WinCheck()
{
	if (m_numAlivePlayers <= 1)
	{
		if (m_numAlivePlayers == 1)
		{
			// Game has Ended with a Winner 
			for (UINT i = 0; i < m_pOrbs.size(); i++)
			{
				if (m_pOrbs[i]->GetAlive())
				{
					m_winner = i + 1;
					break;
				}
			}
		}

		m_gameState = GAME_STATE_END;

	}
}

bool Game::Process(float _dt)
{
	// Set up the shader
	m_pShader_LitTex->SetUpPerFrame();

	switch(m_gameState)
	{
		case GAME_STATE_START:
		{
			if (m_firstProcess == false)
			{
				m_startCountDown -= _dt;
			}
			else
			{
				m_startCountDown -= 0.016f;
				m_firstProcess = false;
			}

			m_number_first.SetImageIndex((int)(m_startCountDown + 1) + 10);

			if (m_startCountDown <= 0.0f)
			{
				m_gameState = GAME_STATE_PROCESS;
				//m_number_first.SetPosition(420, 50);
				//m_number_second.SetPosition(480, 50);
			}
		}
		break;
		case GAME_STATE_PROCESS:
		{
			// Process the game time
			// Alter the Match Timer
			m_matchTimer -= _dt;
			m_number_first.SetImageIndex((int)((m_matchTimer + 1) / 10) + 10);
			m_number_second.SetImageIndex((int)(m_matchTimer + 1) % 10 + 10);

			// Check if the game has been won
			WinCheck();

			m_allConnected = true;

			// Process the Orbs
			for (UINT i = 0; i < m_pOrbs.size(); i++)
			{
				if (m_pOrbs[i]->GetAlive())
				{
					// Process Inputs
					m_isConnected[i] = HandleInput(i);

					if (m_isConnected[i] == false)
					{
						m_allConnected = false;
					};


					if (m_allConnected)
					{
						if (m_gameState == GAME_STATE_ERROR)
						{
							m_gameState = GAME_STATE_PAUSED;
						}
					}
					else
					{
						m_gameState = GAME_STATE_ERROR;
					}



					// Check Collisions
					for (UINT j = 0; j < m_pOrbs.size(); j++)
					{
						if ((i != j))
						{
							if (m_pOrbs[j]->GetAlive())
							{
								if (IsOrbsColliding(m_pOrbs[i], m_pOrbs[j]))
								{
									m_pContollers[i]->Vibrate(1.0f, 1.0f);
									m_pContollers[j]->Vibrate(1.0f, 1.0f);
									HandleCollisions(m_pOrbs[i], m_pOrbs[j]);

									m_pSoundManager->PlayPlayerHit();
								}
							}
						}
					}

					// Stop the Vibrations after half a second
					if (m_pContollers[i]->GetVibrate())
					{
						m_vibrateTimers[i] += _dt;
						if (m_vibrateTimers[i] >= 0.5f)
						{
							m_pContollers[i]->StopVibrate();
							m_vibrateTimers[i] = 0.0f;
						}
					}

					v3float OrbPos = m_pOrbs[i]->GetPosition();
					ArenaTile* collidingTile = 0;
					if (m_pArenaFloor->GetTile(OrbPos, collidingTile))
					{
						m_pOrbs[i]->SetTile(collidingTile);
						m_pOrbs[i]->Process(_dt);

					}
					else
					{
						KillOrb(m_pOrbs[i]);
					}
				}
			}

			// Process the Arean Floor
			m_pArenaFloor->Process(_dt);

		}
		break;
		case GAME_STATE_PAUSED:
		{
			m_pPauseMenu->Process(_dt);
			switch (m_pPauseMenu->GetMenuState())
			{
			case MENU_STATE_RESUME:
				m_gameState = GAME_STATE_PROCESS;
				m_pPauseMenu->Reset();
				break;
			case MENU_STATE_INSTRUCTIONS:
			{
				m_pPauseMenu->GetController()->PreProcess();
				if (m_pPauseMenu->GetController()->GetButtonDown(m_XButtons.ActionButton_B))
				{
					m_pPauseMenu->Reset();
				}
				m_pPauseMenu->GetController()->PostProcess();
			}
			break;
			case MENU_STATE_OPTIONS:
			{
				m_pOptionsMenu->Process(_dt);
				m_pOptionsMenu->GetController()->PreProcess();
				if (m_pOptionsMenu->GetController()->GetButtonDown(m_XButtons.ActionButton_B))
				{
					m_pPauseMenu->Reset();
				}
				m_pOptionsMenu->GetController()->PostProcess();
				//m_pOptionsMenu->Process(_dt);
				
			}
			break;
			case MENU_STATE_EXIT:
				return false;
				break;
			}
		}
		break;
		/*case GAME_STATE_OPTIONS:
		{

		}
		break;*/
		case GAME_STATE_ERROR:
		{
			m_allConnected = true;

			for (UINT i = 0; i < m_pOrbs.size(); i++)
			{
				// Process Inputs
				if (HandleInput(i) == false)
				{
					m_PausedPlayer = i;
					m_allConnected = false;
				};
			}

			if (m_allConnected)
			{
				if (m_gameState == GAME_STATE_ERROR)
				{
					m_gameState = GAME_STATE_PAUSED;
					m_pPauseMenu->SetController(m_pContollers[m_PausedPlayer]);
					m_pOptionsMenu->SetController(m_pContollers[m_PausedPlayer]);
				}
			}
			else
			{
				m_gameState = GAME_STATE_ERROR;
			}
		}
		break;
		case GAME_STATE_END:
		{
			// Process the Inputs for the end state of the game
			for (UINT i = 0; i < m_pContollers.size(); i++)
			{
				m_pContollers[i]->PreProcess();
				m_pContollers[i]->StopVibrate();
				if (m_pContollers[i]->GetButtonPressed(m_XButtons.ActionButton_A))
				{
					// Return the game end
					return false;
				}
				m_pContollers[i]->PostProcess();
			}
		}
		break;
		default:break;
	}

	return true;

}

void Game::Render()
{
	// Draw the Game

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

		m_uiPlayers[i].Render();
	}

	// TO DO JUR
	// Draw the 2D UI Assest

	m_pDX10_Renderer->TurnZBufferOff();

	switch (m_gameState)
	{
		case GAME_STATE_START:
		{
			m_number_first.Render();
		}
		break;
		case GAME_STATE_PROCESS:
		{
			m_number_first.Render();
			m_number_second.Render();
		}
		break;
		case GAME_STATE_PAUSED:
		{
			switch (m_pPauseMenu->GetMenuState())
			{
			case MENU_STATE_INSTRUCTIONS:
				{
					m_pInstructions->Render();
				}
				break;
			case MENU_STATE_OPTIONS:
				{
					m_pOptionsMenu->Draw();
				}
				break;
			default:
				{
					m_pPauseMenu->Draw();
				}
				break;
			}
		}
		break;
		/*case GAME_STATE_OPTIONS:
		{

		}
		break;*/
		case GAME_STATE_ERROR:
		{
			/*for (int i = 0; i < m_numPlayers; ++i)
			{
				if (!m_isConnected[i])
				{
					if (i == 0 || i == 2)
					{
						m_uiControllerMissing->SetPosition(m_uiPlayers[i].GetPosition().x, m_uiPlayers[i].GetPosition().y + m_uiPlayers[i].GetHeight());
					}
					else if (i == 1 || i == 3)
					{
						m_uiControllerMissing->SetPosition(m_uiPlayers[i].GetPosition().x, m_uiPlayers[i].GetPosition().y - m_uiPlayers[i].GetHeight() - m_uiControllerMissing.GetHeight());
					}
					m_uiControllerMissing->Render();
				}
			}*/
		}
		break;
		case GAME_STATE_END:
		{
			for (UINT i = 0; i < m_pOrbs.size(); ++i)
			{
				if (m_pOrbs[i]->GetAlive())
				{
					m_uiVictory[i].Render();
					break;
				}
			}
		}
		break;
		default:break;
	}

	for (int i = 0; i < m_numPlayers; ++i)
	{
		if (!m_pContollers[i]->Connected())
		{
			if (i == 0 || i == 2)
			{
				m_uiControllerMissing->SetPosition(m_uiPlayers[i].GetPosition().x, m_uiPlayers[i].GetPosition().y + m_uiPlayers[i].GetHeight());
			}
			else if (i == 1 || i == 3)
			{
				m_uiControllerMissing->SetPosition(m_uiPlayers[i].GetPosition().x, m_uiPlayers[i].GetPosition().y - m_uiControllerMissing->GetHeight() - m_uiSpace);
			}
			m_uiControllerMissing->Render();
		}
	}

	m_pDX10_Renderer->TurnZBufferOn();

}

bool Game::HandleInput(int _playerNum)
{

	if (m_pContollers[_playerNum]->Connected())
	{

		m_pContollers[_playerNum]->PreProcess();

		if (m_gameState != GAME_STATE_ERROR)
		{


			// Movement
			v2float LeftAxis;
			if (!m_pContollers[_playerNum]->LStick_InDeadZone())
			{
				LeftAxis = m_pContollers[_playerNum]->GetLStickAxis();
				m_pOrbs[_playerNum]->SetAcceleration({ LeftAxis.x, LeftAxis.y, 0.0f });
			}

			// Boost
			if (m_pContollers[_playerNum]->GetButtonPressed(m_XButtons.Bumper_R))
			{
				m_pOrbs[_playerNum]->Boost();
			}


			if (m_pContollers[_playerNum]->GetButtonPressed(m_XButtons.Bumper_L))
			{
				m_pOrbs[_playerNum]->Phase();
			}
		}
		//m_pOrbs[_playerNum]->Phase(m_pContollers[_playerNum]->GetButtonPressed(m_XButtons.Bumper_L));

		// Return false if start has been pressed
		if (m_pContollers[_playerNum]->GetButtonDown(m_XButtons.Start))
		{
			if (m_gameState == GAME_STATE_PAUSED)
			{
	
			}
			else
			{
				m_gameState = GAME_STATE_PAUSED;
				m_PausedPlayer = _playerNum;
				m_pPauseMenu->SetController(m_pContollers[_playerNum]);
				m_pOptionsMenu->SetController(m_pContollers[_playerNum]);
			}
		}

		m_pContollers[_playerNum]->PostProcess();

		return true;
	}
	else
	{
		return false;
	}
}

void Game::UpdateClientSize()
{
	int width = m_pDX10_Renderer->GetWidth();
	int height = m_pDX10_Renderer->GetHeight();

	m_uiPlayers[1].SetPosition(m_pDX10_Renderer->GetWidth() - (m_uiWidth * m_uiScale) - m_uiSpace, m_pDX10_Renderer->GetHeight() - (m_uiHeight * m_uiScale) - m_uiSpace);
	m_uiPlayers[2].SetPosition(m_pDX10_Renderer->GetWidth() - (m_uiWidth * m_uiScale) - m_uiSpace, m_uiSpace);
	m_uiPlayers[3].SetPosition(m_uiSpace, m_pDX10_Renderer->GetHeight() - (m_uiHeight * m_uiScale) - m_uiSpace);
	// TO DO JURAN: Update client size on UI stuff
}

bool Game::AttachMenuComponents(Menu* _pPauseMenu, Menu* _pOptionsMenu, DXSprite* _pInstructionsUI, DXSprite* _pControllerUI)
{
	if (_pPauseMenu && _pOptionsMenu && _pInstructionsUI && _pControllerUI)
	{
		m_pPauseMenu = _pPauseMenu;
		m_pOptionsMenu = _pOptionsMenu;
		m_pInstructions = _pInstructionsUI;
		m_uiControllerMissing = _pControllerUI;

		return true;
	}
	
	return false;
}

