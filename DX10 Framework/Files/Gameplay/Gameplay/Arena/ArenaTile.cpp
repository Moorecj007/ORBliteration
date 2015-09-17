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
			VALIDATE(m_pRenderer->CreateTexture("Tile/Slippery.dds", m_pBaseTex));
		}
		break;
		case BTI_ROUGH:
		{
			VALIDATE(m_pRenderer->CreateTexture("Tile/Rough.dds", m_pBaseTex));
		}
		break;
		case BTI_STANDARD:	// Fall through
		default:
		{
			VALIDATE(m_pRenderer->CreateTexture("Tile/Standard.dds", m_pBaseTex));
		}
	}	// End Switch

	// Create the array of the overlay textures
	VALIDATE(m_pRenderer->CreateTexture("Tile/Powerup_Blank.dds", m_pOverlayTex[OTI_BLANK]));
	VALIDATE(m_pRenderer->CreateTexture("Tile/Powerup_Confusion.dds", m_pOverlayTex[OTI_POWER_CONFUSION]));
	VALIDATE(m_pRenderer->CreateTexture("Tile/Powerup_SizeIncrease.dds", m_pOverlayTex[OTI_POWER_SIZEINCREASE]));
	VALIDATE(m_pRenderer->CreateTexture("Tile/Powerup_SpeedIncrease.dds", m_pOverlayTex[OTI_POWER_SPEEDINCREASE]));

	m_currentOverlay = OTI_BLANK;
	m_baseImage = _baseImage;
	m_active = true;
	return true;
}

void ArenaTile::Process(float _dt)
{
	if (m_active)
	{
		// TO DO CAL - Determine the correct image to be displayed
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
		_litTex.pTex2 = m_pOverlayTex[m_currentOverlay];

		// Set the Shader to Render the Tile
		m_pShader_LitTex->Render(_litTex, TECH_LITTEX_BLENDTEX2);
	}
}