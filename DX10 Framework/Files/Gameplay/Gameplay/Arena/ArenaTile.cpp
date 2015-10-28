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

bool ArenaTile::Initialise(DX10_Renderer* _pDX10_Renderer, DX10_Mesh* _pMesh, DX10_Shader_LitTex* _pShader, eBaseTileImages _baseImage)
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

	SetRotationPitch(DegreesToRadians(-90));
	
	// Create the Base Tile Image texture
	switch (_baseImage)
	{
		case BTI_SLIPPERY:
		{
			VALIDATE(m_pRenderer->CreateTexture("Tron/Tile/tron_tile_blue.png", m_pBaseTex));
		}
		break;
		case BTI_ROUGH:
		{
			VALIDATE(m_pRenderer->CreateTexture("Tron/Tile/tron_tile_red.png", m_pBaseTex));
		}
		break;
		case BTI_STANDARD:	// Fall Through
		default:
		{
			VALIDATE(m_pRenderer->CreateTexture("Tron/Tile/tron_tile_white.png", m_pBaseTex));
		}
	}	// End Switch

	m_baseImage = _baseImage;
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
				m_reduceAlpha += 0.1f;
				
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
		_litTex.pTexBase = m_pBaseTex;
		_litTex.reduceAlpha = m_reduceAlpha;

		// Set the Shader to Render the Tile
		m_pShader_LitTex->Render(_litTex, TECH_LITTEX_BLENDTEX2);
	}
}

bool ArenaTile::SetBaseImageEnum(eBaseTileImages _baseImage)
{ 
	m_baseImage = _baseImage;

	// Create the Base Tile Image texture
	switch (_baseImage)
	{
	case BTI_SLIPPERY:
	{
		VALIDATE(m_pRenderer->CreateTexture("Tron/Tile/tron_tile_blue.png", m_pBaseTex));
	}
		break;
	case BTI_ROUGH:
	{
		VALIDATE(m_pRenderer->CreateTexture("Tron/Tile/tron_tile_red.png", m_pBaseTex));
	}
		break;
	case BTI_STANDARD:	// Fall Through
	default:
	{
		VALIDATE(m_pRenderer->CreateTexture("Tron/Tile/tron_tile_white.png", m_pBaseTex));
	}
	}	// End Switch

	//successful set
	return true;

};