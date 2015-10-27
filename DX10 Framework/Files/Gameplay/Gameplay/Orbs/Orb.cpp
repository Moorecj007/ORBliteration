//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : Orb.h
// Description : Class to handle all funtionality of the Player Controlled Orbs
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//

// This Include
#include "Orb.h"


//REMOVE
#include <iostream>
 
Orb::Orb()
{
	m_pTile = 0;
	m_score = 0;

	m_acceleration = {0.0f,0.0f,0.0f};
	m_velocity = { 0.0f, 0.0f, 0.0f };
	m_bounce = 0.0f;
	m_isAlive = true;

	m_collidable = true;
	m_collideCountdown = 0.0f;
	m_collideStartTime = 0.05f;
}

Orb::~Orb()
{
	// Delete the Glow Light from the Renderer
	m_pRenderer->RemoveLight(m_glowName);
}

bool Orb::Initialise(DX10_Renderer* _pRenderer, DX10_Mesh* _pMesh, DX10_Shader_LitTex* _pShader, int _playerNum, float _bounce, float _speed)
{	
	std::string texName;
	switch (_playerNum)
	{
		case 1:
		{
			texName = "Tron/Orb/tron_orb_player1.png";
			m_glowName = "zOrbGlow1";

			m_pGlowLight = new Light();
			m_pGlowLight->type = LT_GLOW;
			m_pGlowLight->pos_range = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 6.0f);
			m_pGlowLight->diffuse = D3DXCOLOR(1.0f, (108.0f / 255.0f), 0.0f, 1.0f);
			VALIDATE(_pRenderer->AddLight(m_glowName, m_pGlowLight));
		}
		break;
		case 2:
		{
			texName = "Tron/Orb/tron_orb_player2.png";
			m_glowName = "zOrbGlow2";

			m_pGlowLight = new Light();
			m_pGlowLight->type = LT_GLOW;
			m_pGlowLight->pos_range = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 6.0f);
			m_pGlowLight->diffuse = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			VALIDATE(_pRenderer->AddLight(m_glowName, m_pGlowLight));
		}
		break;
		case 3:
		{
			texName = "Tron/Orb/tron_orb_player3.png";
			m_glowName = "zOrbGlow3";

			m_pGlowLight = new Light();
			m_pGlowLight->type = LT_GLOW;
			m_pGlowLight->pos_range = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 6.0f);
			m_pGlowLight->diffuse = D3DXCOLOR((47.0f / 255.0f), (241.0f / 46.0f), (108.0f / 255.0f), 1.0f);
			VALIDATE(_pRenderer->AddLight(m_glowName, m_pGlowLight));
		}
		break;
		case 4:
		{
			texName = "Tron/Orb/tron_orb_player4.png";
			m_glowName = "zOrbGlow4";

			m_pGlowLight = new Light();
			m_pGlowLight->type = LT_GLOW;
			m_pGlowLight->pos_range = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 6.0f);
			m_pGlowLight->diffuse = D3DXCOLOR((230.0f / 255.0f), (46.0f / 255.0f), (241.0f / 255.0f), 1.0f);
			VALIDATE(_pRenderer->AddLight(m_glowName, m_pGlowLight));
		}
		break;
	}



 	// Initialise the object this is derived from
	VALIDATE(DX10_Obj_LitTex::Initialise(_pRenderer, _pMesh, _pShader, texName));

	// Check the passed in parameters
	if ((_bounce < 0.0f) )
	{
		return false;
	}

	// Store the initial state of the variables
	m_bounce = _bounce;
	m_radius = _pMesh->GetScale().x / 2 ;
	m_speed = _speed;
	m_isAlive = true;

	m_boostAmount = 6.0f;
	m_boostCooldown = 1.0f;
	m_boostLimit = 0.1f;
	m_boostActiveTime = 0.0f;
	m_boostCoolDownTime = 0.0f;
	m_AllowBoost = true;
	m_boost = false;
	   
	m_phaseCooldown = 2.0f;
	m_phaseMaxTime = 3.0f;
	m_phaseActiveTime = 0.0f;
	m_phaseCoolDownTime = 0.0f;
	m_AllowPhase = true;
	m_phase = false;

	// Successful Initialization
	return true;
}
  
void Orb::ProcessFriction()
{
	if (m_pTile != 0)
	{
		// Calculate the fiction on the tile and whether or not t process inputs
		switch (m_pTile->GetBaseImageEnum())
		{
		case BTI_SLIPPERY:
		{
			m_surfaceFriction = 0.0f;
		}
			break;
		case BTI_ROUGH:
		{
			m_surfaceFriction = 0.01f;
		}
			break;
		case BTI_STANDARD:
		{
			m_surfaceFriction = 0.005f;
		}
			break;
		default:
		{
			m_surfaceFriction = 0.005f;
		}
			break;
		}

		
	}
}

void Orb::Process(float _dt)
{
	if (m_collidable == false)
	{
		m_collideCountdown -= _dt;

		if (m_collideCountdown <= 0.0f)
		{
			m_collidable = true;
		}
	}


	ProcessFriction();

	float speedLimit = 0.5f;

	// Boost 
	if (m_boost)
	{
		m_boostActiveTime += _dt;
		m_pGlowLight->pos_range.w = 10.0f;

		if (m_boostActiveTime < m_boostLimit)
		{
			m_acceleration = m_acceleration * m_boostAmount;
			speedLimit = 0.6f;

		}
		else
		{
			m_boostActiveTime = 0.0f;
			m_boost = false;
			speedLimit = 0.5f;
		}
	}
	else
	{
		m_pGlowLight->pos_range.w = 6.0f;
		if (m_AllowBoost == false)
		{
			m_boostCoolDownTime += _dt;
	
			if (m_boostCoolDownTime > m_boostCooldown)
			{
				m_boostCoolDownTime = 0.0f;
				m_AllowBoost = true;
			}
		}
	}

	m_velocity += ((m_acceleration * _dt)* m_speed);
	m_acceleration *= 0.0f;
	m_velocity = m_velocity - (m_velocity * m_surfaceFriction);

	if (m_velocity.Magnitude() < 0.0001f)
	{
		m_velocity = { 0.0f, 0.0f, 0.0f };
	}

	m_velocity = m_velocity.Limit(speedLimit);

	m_pos += m_velocity;

	// Phase 
	if (m_phase)
	{
		m_phaseActiveTime += _dt;
	
		if (m_phaseActiveTime < m_phaseMaxTime)
		{			
			
		}
		else
		{
			m_phaseActiveTime = 0.0f;
			m_phase = false;
			m_AllowPhase = false;
		}
	}
	else
	{
		if (m_AllowPhase == false)
		{
			m_phaseCoolDownTime += _dt;
	
			if (m_phaseCoolDownTime > m_phaseCooldown)
			{
				m_phaseCoolDownTime = 0.0f;
				m_AllowPhase = true;
			}
		}
	}

	// Update the Glow lights position
	m_pGlowLight->pos_range.x = m_pos.x;
	m_pGlowLight->pos_range.y = m_pos.y;
	m_pGlowLight->pos_range.z = m_pos.z;


	DX10_Obj_LitTex::Process(_dt);
}

void Orb::Render()
{
	TLitTex _litTex;
	_litTex.pMesh = m_pMesh;
	_litTex.pMatWorld = &m_matWorld;
	_litTex.pTexBase = (*m_pTextures)[m_texIndex];
	
	if (m_phase)
	{
		m_pGlowLight->active = false;
		_litTex.reduceAlpha = 0.5f;
		m_pShader->Render(_litTex, TECH_LITTEX_FADE);
	}
	else
	{
		m_pGlowLight->active = true;
		_litTex.reduceAlpha = 0.0f;
		m_pShader->Render(_litTex, TECH_LITTEX_STANDARD);
	}
}

void Orb::SetAcceleration(v3float _acceleration)
{
	if (m_pTile != 0)
	{
		if (m_pTile->GetBaseImageEnum() != BTI_SLIPPERY)
		{
			// Only Set the Acceleration if the Orb is not on a Slippery tile
			m_acceleration += _acceleration;
		}
		else if (m_velocity.Magnitude() == 0)
		{
			// Only Set the Acceleration if the Orb is not on a Slippery tile
			m_acceleration += _acceleration * 5.0f;

		}
	}
};

void Orb::Boost()
{
	if (m_AllowBoost == true)
	{
		// Boost
		m_boost = true;	
		m_AllowBoost = false;

	}

}

void Orb::Phase()
{
	if (m_AllowPhase == true)
	{
		// Phase
		m_phase = true;
		m_AllowPhase = false;

	}
}

