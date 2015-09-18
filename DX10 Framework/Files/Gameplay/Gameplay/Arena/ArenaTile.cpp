/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : ArenaTile.cpp
* Description : Tile to hold information about a small section of the Arena Floor
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#include "ArenaTile.h"

ArenaTile::ArenaTile()
{
	m_reduceAlpha = 0.0f;
}

ArenaTile::~ArenaTile()
{
}

bool ArenaTile::Initialise(DX10_Renderer* _pDX10_Renderer, DX10_Mesh_Generic* _pMesh, DX10_Shader_LitTex* _pShader, eBaseTileImages _baseImage)
{
	if (_pDX10_Renderer == 0 || _pMesh == 0 || _pShader == 0)
	{
		// Initialization is invalid if any passed pointers are NULL
		return false;
	}
	BaseInitialise();
	m_pRenderer = _pDX10_Renderer;
	m_pMesh = _pMesh;
	m_pShader_LitTex = _pShader;
	
	// Create the Base Tile Image texture
	switch (_baseImage)
	{
		case BTI_SLIPPERY:
		{
			VALIDATE(m_pRenderer->CreateTexture("Tron/Tile/tron_tile_blue.png", m_pBaseTex[0]));
			//VALIDATE(m_pRenderer->CreateTexture("Tron/Tile/tron_tile_blue.png", m_pBaseTex[1]));
		}
		break;
		case BTI_ROUGH:
		{
			VALIDATE(m_pRenderer->CreateTexture("Tron/Tile/tron_tile_red.png", m_pBaseTex[0]));
			//VALIDATE(m_pRenderer->CreateTexture("Tron/Tile/tron_tile_red.png", m_pBaseTex[1]));
		}
		break;
		case BTI_STANDARD:	// Fall through
		default:
		{
			VALIDATE(m_pRenderer->CreateTexture("Tron/Tile/tron_tile_white.png", m_pBaseTex[0]));
			/*VALIDATE(m_pRenderer->CreateTexture("Tile/Standard_Dying1.dds", m_pBaseTex[1]));
			VALIDATE(m_pRenderer->CreateTexture("Tile/Standard_Dying2.dds", m_pBaseTex[2]));
			VALIDATE(m_pRenderer->CreateTexture("Tile/Standard_Dying3.dds", m_pBaseTex[3]));
			VALIDATE(m_pRenderer->CreateTexture("Tile/Standard_Dying4.dds", m_pBaseTex[4]));
			VALIDATE(m_pRenderer->CreateTexture("Tile/Standard_Dying5.dds", m_pBaseTex[5]));
			VALIDATE(m_pRenderer->CreateTexture("Tile/Standard_Dying6.dds", m_pBaseTex[6]));
			VALIDATE(m_pRenderer->CreateTexture("Tile/Standard_Dying7.dds", m_pBaseTex[7]));
			VALIDATE(m_pRenderer->CreateTexture("Tile/Standard_Dying8.dds", m_pBaseTex[8]));
			VALIDATE(m_pRenderer->CreateTexture("Tile/Standard_Dying9.dds", m_pBaseTex[9]));
			VALIDATE(m_pRenderer->CreateTexture("Tile/Standard_Dying10.dds", m_pBaseTex[10]));*/
		}
	}	// End Switch

	// Create the array of the overlay textures
	VALIDATE(m_pRenderer->CreateTexture("Tile/Powerup_Blank.dds", m_pOverlayTex[OTI_BLANK]));
	VALIDATE(m_pRenderer->CreateTexture("Tile/Powerup_Confusion.dds", m_pOverlayTex[OTI_POWER_CONFUSION])); 
	VALIDATE(m_pRenderer->CreateTexture("Tile/Powerup_SizeIncrease.dds", m_pOverlayTex[OTI_POWER_SIZEINCREASE]));
	VALIDATE(m_pRenderer->CreateTexture("Tile/Powerup_SpeedIncrease.dds", m_pOverlayTex[OTI_POWER_SPEEDINCREASE]));

	m_currentOverlay = OTI_BLANK;
	m_baseImage = _baseImage;
	m_currentBaseImage = 0;
	m_active = true;
	m_deathTimer = -1.0f;
	return true;
}

void ArenaTile::Process(float _dt)
{
	if (m_active)
	{
		if (m_deathTimer != -1.0f)
		{
			m_deathTimer += _dt;

			if (m_deathTimer >= m_deathIncrement)
			{
				m_reduceAlpha += 0.1;
				
				if (m_reduceAlpha >= 1.0f)
				{
					m_reduceAlpha = 0.0f;
					SetActive(false);
				}

				m_deathTimer -= m_deathIncrement;
			}
		}		
	}	
}

void ArenaTile::Render()
{
	if (m_active == true)
	{
		// Put the Object Specific variables into a structure
		TLitTex _litTex;
		_litTex.pMesh = m_pMesh;
		_litTex.pMatWorld = &m_matWorld;
		_litTex.pTexBase = m_pBaseTex[m_currentBaseImage];
		_litTex.pTex2 = m_pOverlayTex[m_currentOverlay];
		_litTex.reduceAlpha = m_reduceAlpha;

		// Set the Shader to Render the Tile
		m_pShader_LitTex->Render(_litTex, TECH_LITTEX_BLENDTEX2);
	}
}