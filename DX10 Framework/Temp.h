


bool Game::Process(float _dt)
{
	// Set up the shader
	m_pShader_LitTex->SetUpPerFrame();


	if (m_gameState == GAME_STATE_START)
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
	else
	{
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
				m_gameState = GAME_STATE_END;
				// No Winner the Game is a Draw
			}
			else
			{
				m_gameState = GAME_STATE_END;
				// Player 'Winner' has won
			}
			// This Player has won
			// TO DO JUR: Print Winner and Press A to Continue

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
		else // Game Still running
		{
			if (m_gameState == GAME_STATE_PAUSED)
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
			else
			{
				// Alter the Match Timer
				m_matchTimer -= _dt;
				m_number_first.SetImageIndex((int)((m_matchTimer + 1) / 10) + 10);
				m_number_second.SetImageIndex((int)(m_matchTimer + 1) % 10 + 10);
			}

			// Process the Orbs
			for (UINT i = 0; i < m_pOrbs.size(); i++)
			{
				if (m_pOrbs[i]->GetAlive())
				{
					// Process Inputs
					HandleInput(i);

					if (m_gameState == GAME_STATE_PROCESS)
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
			}

			if (m_gameState == GAME_STATE_PROCESS)
			{
				// Process the arena Floor
				m_pArenaFloor->Process(_dt);
			}
		}
	}

	return true;
}