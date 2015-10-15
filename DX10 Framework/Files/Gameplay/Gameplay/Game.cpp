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
	//m_pOrbs.reserve(4);
	//m_pContollers.reserve(4);
	//
	//for (UINT i = 0; i < 4; i++)
	//{
	//	m_pOrbs[i] = 0;
	//	m_pContollers[i] = 0;
	//}
}

Game::~Game()
{
	ReleasePtr(m_pArenaFloor);
	ReleasePtr(m_pShader_LitTex);
	ReleasePtr(VictroyPlayerOne);
	ReleasePtr(TempPause);
	ReleasePtr(m_pPausesMenu);
	ReleasePtr(TempError);

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
	m_matchTimer = 90.0f;
	m_startCountDown = 3.0f;
	m_firstProcess = true;

	// JC TO DO: Change to start
	m_gameState = GAME_STATE_START;

	m_numPlayers = _numPlayers;
	m_numAlivePlayers = _numPlayers;
	m_pDX10_Renderer = _pDX10_Renderer;
	m_pSoundManager = _pSoundManager;

	// TO DO JUR: Temp all to be Remove
	m_pSpriteShader = _pSpriteShader;
	VictroyPlayerOne = new DXSprite();
	VictroyPlayerOne->Initialise(m_pDX10_Renderer, m_pSpriteShader, "Tron/UI/Tron_Victory_P1.png", 760, 601);
	VictroyPlayerOne->SetSize(800, 800);
	VictroyPlayerOne->SetPosition(100,100);

	TempPause = new DXSprite();
	TempPause->Initialise(m_pDX10_Renderer, m_pSpriteShader, "Tron/UI/Tron_Victory_P2.png", 760, 601);
	TempPause->SetSize(800, 800);
	TempPause->SetPosition(100, 100);

	TempError = new DXSprite();
	TempError->Initialise(m_pDX10_Renderer, m_pSpriteShader, "Tron/UI/Tron_Victory_P3.png", 760, 601);
	TempError->SetSize(800, 800);
	TempError->SetPosition(100, 100);

	m_number_first.Initialise(m_pDX10_Renderer, m_pSpriteShader, "Tron/UI/tron_numbers_fill.png", 1060, 424, 10, 4);
	m_number_first.SetSize(106.0f * 0.5f, 106.0f * 0.5f);
	m_number_first.SetPosition(450, 50);
	m_number_second.Initialise(m_pDX10_Renderer, m_pSpriteShader, "Tron/UI/tron_numbers_fill.png", 1060, 424, 10, 4);
	m_number_second.SetSize(106.0f * 0.5f, 106.0f * 0.5f);
	m_number_second.SetPosition(450, 50);
	
	// Create the Shader for the Game Objects
	m_pShader_LitTex = new DX10_Shader_LitTex();
	VALIDATE(m_pShader_LitTex->Initialise(m_pDX10_Renderer));

	// Create and Initialise the Arena Floor
	m_pArenaFloor = new ArenaFloor();
	VALIDATE(m_pArenaFloor->Initialise(m_pDX10_Renderer, m_pShader_LitTex, 15, { 4, 4, 4 }, m_matchTimer));

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
		VALIDATE(m_pContollers[i]->Initialise((i + 1), _AllowVibrate));
		m_vibrateTimers[i] = 0.0f;

		m_pOrbs.push_back(new Orb());
		int row, col;
		
		float scale = 0.5f;
		float width = 671.0f;
		float height = 457.0f;

		switch (i)
		{
			case 0:
			{
				row = 1;
				col = 13;

				DXSprite uiPlayer1;
				VALIDATE(uiPlayer1.Initialise(m_pDX10_Renderer, m_pSpriteShader, "Tron/UI/tron_ui_p1.png", (UINT)width, (UINT)height));
				uiPlayer1.SetPosition(10, 10);
				uiPlayer1.SetSize(width * scale, height * scale);
				m_uiPlayers.push_back(uiPlayer1);
			}
			break;
			case 1:
			{
				row = 1;
				col = 1;

				DXSprite uiPlayer2;
				VALIDATE(uiPlayer2.Initialise(m_pDX10_Renderer, m_pSpriteShader, "Tron/UI/tron_ui_p2.png", (UINT)width, (UINT)height));
				uiPlayer2.SetPosition(990 - (width*scale), 10);
				uiPlayer2.SetSize(width * scale, height * scale);
				m_uiPlayers.push_back(uiPlayer2);
			}
			break;
			case 2:
			{
				row = 13;
				col = 13;

				DXSprite uiPlayer3;
				VALIDATE(uiPlayer3.Initialise(m_pDX10_Renderer, m_pSpriteShader, "Tron/UI/tron_ui_p3.png", (UINT)width, (UINT)height));
				uiPlayer3.SetPosition(10, 990 - (height*scale));
				uiPlayer3.SetSize(width * scale, height * scale);
				m_uiPlayers.push_back(uiPlayer3);

			}
			break;
			case 3:
			{
				row = 13;
				col = 1;

				DXSprite uiPlayer4;
				VALIDATE(uiPlayer4.Initialise(m_pDX10_Renderer, m_pSpriteShader, "Tron/UI/tron_ui_p4.png", (UINT)width, (UINT)height));
				uiPlayer4.SetPosition(990 - (width*scale), 990 - (height*scale));
				uiPlayer4.SetSize(width * scale, height * scale);
				m_uiPlayers.push_back(uiPlayer4);
			}
			break;
		}
		VALIDATE(m_pOrbs[i]->Initialise(m_pDX10_Renderer, m_pOrbMesh, m_pShader_LitTex, (i + 1), 2.0f, 5.0f, 1000.0f));
		
		// Set the Orbs Positions
		v3float	OrbPos = m_pArenaFloor->GetTilePos(row, col);
		OrbPos.z = -2.0f;
		m_pOrbs[i]->SetPosition(OrbPos);
		m_pOrbs[i]->Process(0.016f);

	}
   

	m_pPausesMenu = new Menu();
	VALIDATE(m_pPausesMenu->Initialise(m_pDX10_Renderer, m_pSpriteShader, m_pSoundManager, m_pContollers[0], _pKeyDown));

	m_pPausesMenu->AddSprite("Tron/Button/tron_button_resume_fill.png", 575, 424, 1, 4);
	m_pPausesMenu->AddSprite("Tron/Button/tron_button_instructions_fill.png", 1137, 424, 1, 4);
	m_pPausesMenu->AddSprite("Tron/Button/tron_button_options_fill.png", 669, 424, 1, 4);
	m_pPausesMenu->AddSprite("Tron/Button/tron_button_exit_fill.png", 387, 424, 1, 4);
	m_pPausesMenu->AddButton(MENU_STATE_RESUME, 0, 0.5f);
	m_pPausesMenu->AddButton(MENU_STATE_INSTRUCTIONS, 1, 0.5f);
	m_pPausesMenu->AddButton(MENU_STATE_OPTIONS, 2, 0.5f);
	m_pPausesMenu->AddButton(MENU_STATE_EXIT, 3, 0.5f);

	VALIDATE(m_instructions.Initialise(m_pDX10_Renderer, m_pSpriteShader, "Tron/UI/tron_orbliteration_instructions.png", 3000, 3000));
	//m_instructions.SetPosition(100, 100);
	//m_instructions.SetSize(800, 800);

	int width = m_pDX10_Renderer->GetWidth();
	int height = m_pDX10_Renderer->GetHeight();

	float diff = max(width, height) - min(width, height);
	if (min(width, height) == width)
	{
		m_instructions.SetPosition(0, diff / 2.0f);
	}
	else
	{
		m_instructions.SetPosition(diff / 2.0f, 0);
	}

	
	m_instructions.SetSize(min(width, height), min(width, height));
		
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
	if ((_pOrbA != 0) && (_pOrbB != 0))
	{
		v3float orbVelocity_A, orbVelocity_B;

		orbVelocity_A = _pOrbA->GetVelocity();
		orbVelocity_B = _pOrbB->GetVelocity();


		if (orbVelocity_A.Magnitude() < orbVelocity_B.Magnitude())
		{
			// Orb B has the Higher Velocity

			_pOrbA->SetVelocity((orbVelocity_B * _pOrbB->GetBounce()));
			// TO DO JC: Take a small multiple of of the other orbs velocity 
			_pOrbB->SetVelocity(orbVelocity_A * 0.0f);
		}
		else if (orbVelocity_A.Magnitude() > orbVelocity_B.Magnitude())
		{
			// Orb A has the Higher Velocity

			_pOrbB->SetVelocity((orbVelocity_A * _pOrbA->GetBounce()));
			// TO DO JC: Take a small multiple of of the other orbs velocity 
			_pOrbA->SetVelocity(orbVelocity_B * 0.0f);
		}
		else
		{
			// Velocities are the same
			_pOrbA->SetVelocity((orbVelocity_B * _pOrbB->GetBounce()));
			_pOrbB->SetVelocity((orbVelocity_A * _pOrbA->GetBounce()));
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
				m_number_first.SetPosition(420, 50);
				m_number_second.SetPosition(480, 50);
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

			// Process the Orbs
			for (UINT i = 0; i < m_pOrbs.size(); i++)
			{
				if (m_pOrbs[i]->GetAlive())
				{
					// Process Inputs
					HandleInput(i);

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
			m_pPausesMenu->Process(_dt);
			switch (m_pPausesMenu->GetMenuState())
			{
			case MENU_STATE_RESUME:
				m_gameState = GAME_STATE_PROCESS;
				m_pPausesMenu->Reset();
				break;
			case MENU_STATE_INSTRUCTIONS:
				break;
			case MENU_STATE_OPTIONS:
				break;
			case MENU_STATE_EXIT:
				return false;
				break;
			}
		}
		break;
		case GAME_STATE_ERROR:
		{

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
			switch (m_pPausesMenu->GetMenuState())
			{
			case MENU_STATE_INSTRUCTIONS:
				m_instructions.Render();
				break;
			case MENU_STATE_OPTIONS:
				break;
			default:
				m_pPausesMenu->Draw();
				break;
			}
		}
		break;
		case GAME_STATE_ERROR:
		{
			TempError->Render();
		}
		break;
		case GAME_STATE_END:
		{
			VictroyPlayerOne->Render();
		}
		break;
		default:break;
	}

	m_pDX10_Renderer->TurnZBufferOn();

}

bool Game::HandleInput(int _playerNum)
{
	bool allConnected = true;

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

		// Return false if start has been pressed
		if (m_pContollers[_playerNum]->GetButtonDown(m_XButtons.Start))
		{
			if (m_gameState == GAME_STATE_PAUSED)
			{
				/*if (_playerNum == m_PausedPlayer)
				{
					m_gameState = GAME_STATE_PROCESS;
					m_pPausesMenu->Reset();
				}*/
			}
			else
			{
				m_gameState = GAME_STATE_PAUSED;
				m_PausedPlayer = _playerNum;
				m_pPausesMenu->SetController(m_pContollers[_playerNum]);
			}
		}

		m_pContollers[_playerNum]->PostProcess();

		//return true;
	}
	else
	{
		allConnected = false;
	}

	if (allConnected)
	{
		if (m_gameState == GAME_STATE_ERROR)
		{
			m_gameState = GAME_STATE_PAUSED;
		}
		return true;
	}
	else
	{
		m_gameState = GAME_STATE_ERROR;
		return false;
	}

}

void Game::UpdateClientSize()
{
	int width = m_pDX10_Renderer->GetWidth();
	int height = m_pDX10_Renderer->GetHeight();



	// TO DO JURAN: Update client size on UI stuff
}