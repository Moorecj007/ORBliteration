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

bool ArenaTile::Initialise(DX10_Renderer* _pDX10_Renderer, DX10_Mesh_Generic* _pMesh, DX10_Shader_LitTex* _pShader, std::string _pTexName)
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
	
	VALIDATE(m_pRenderer->CreateTexture(_pTexName, m_pTex));
	
	return true;
}

void ArenaTile::Process(float _dt)
{
	// TO DO CAL - Determine the correct image to be displayed
}

void ArenaTile::Render()
{
	// Put the Object Specific variables into a structure
	TLitTex _litTex;
	_litTex.pMesh = m_pMesh;
	_litTex.pMatWorld = &m_matWorld;
	_litTex.pTex = m_pTex;

	// Set the Shader to Render the Tile
	m_pShader_LitTex->Render(_litTex, TECH_LITTEX_STANDARD);
}