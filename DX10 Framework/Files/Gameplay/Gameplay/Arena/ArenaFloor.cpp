/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : ArenaFloor.cpp
* Description : Controlling class for all arena related processes. Holds and controls all individual tiles.
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "ArenaFloor.h"

ArenaFloor::ArenaFloor()
{
	m_pDX10_Renderer = 0;
	m_timeElapsed = 0;
}

ArenaFloor::~ArenaFloor()
{
	while (m_arenaTiles.empty() == false)
	{
		while (m_arenaTiles.back()->empty() == false)
		{
			ReleasePtr(m_arenaTiles.back()->back());
			m_arenaTiles.back()->pop_back();
		}

		ReleasePtr(m_arenaTiles.back());
		m_arenaTiles.pop_back();
	}

	ReleasePtr(m_pTileMesh);
}

bool ArenaFloor::Initialise(DX10_Renderer* _pDX10_Renderer, DX10_Shader_LitTex* _pShader, UINT rowCount, UINT colCount, v3float _tileScale)
{
	m_pDX10_Renderer = _pDX10_Renderer;

	// Create the Mesh for the Arena Tiles
	m_pTileMesh = new DX10_Mesh_Rect_Prism();
	TVertexNormalUV vertNormalUV;
	VALIDATE(m_pTileMesh->Initialise(m_pDX10_Renderer, vertNormalUV, _tileScale));

	// Create the 2D vector of Arena Tiles
	for (UINT row = 0; row < rowCount; row++)
	{
		std::vector<ArenaTile*>* pRowOfTiles = new std::vector<ArenaTile*>;

		for (UINT col = 0; col < colCount; col++)
		{
			// Create a new Tile
			ArenaTile* pTile = new ArenaTile();
			VALIDATE(pTile->Initialise(m_pDX10_Renderer, m_pTileMesh, _pShader, "WoodCrate01.dds"));

			// Set the Position based upon the position in the 2D grid
			v3float tilePos;
			tilePos.x = ((float)row - ((float)rowCount / 2.0f)) * _tileScale.x + (_tileScale.x / 2.0f);
			tilePos.y = ((float)col - ((float)colCount / 2.0f)) * _tileScale.y + (_tileScale.y / 2.0f);
			tilePos.z = 0.0f;
			pTile->SetPosition(tilePos);

			// Calculate the World matrix at Initialization only
			pTile->CalcWorldMatrix();

			// Add the new Tile to the Row
			pRowOfTiles->push_back(pTile);
		}
		m_arenaTiles.push_back(pRowOfTiles);
	}

	return true;
}

void ArenaFloor::Process(float _dt)
{
	m_timeElapsed += _dt;
	if (m_timeElapsed >= 0.5f)	// TO DO make variable
	{
		//DestroyOuterLayer();

		m_timeElapsed -= 0.5f;
	}


	// Process all the Tiles in the 2D vector
	// Cycle through all rows
	for (UINT row = 0; row < m_arenaTiles.size(); row++)
	{
		// Cycle through all columns (elements in the rows)
		for (UINT col = 0; col < m_arenaTiles[row]->size(); col++)
		{
			(*m_arenaTiles[row])[col]->Process(_dt);
		}
	}
}

void ArenaFloor::Render()
{
	// Process all the Tiles in the 2D vector
	// Cycle through all rows
	for (UINT row = 0; row < m_arenaTiles.size(); row++)
	{
		// Cycle through all columns (elements in the rows)
		for (UINT col = 0; col < m_arenaTiles[row]->size(); col++)
		{
			(*m_arenaTiles[row])[col]->Render();
		}
	}
}

void ArenaFloor::DestroyOuterLayer()
{
	if (m_arenaTiles.size() != 0)
	{
		ReleasePtr(m_arenaTiles[0]);
		m_arenaTiles.erase(m_arenaTiles.begin());

		if (m_arenaTiles.size() != 0)
		{
			ReleasePtr(m_arenaTiles[m_arenaTiles.size() - 1]);
			m_arenaTiles.erase(m_arenaTiles.begin() + m_arenaTiles.size() - 1);
		}

		for (int row = (m_arenaTiles.size() - 1); row >= 0; row--)
		{
			if (m_arenaTiles[row]->size() != 0)
			{
				ReleasePtr((*m_arenaTiles[row])[0]);
				m_arenaTiles[row]->erase(m_arenaTiles[row]->begin());

				if (m_arenaTiles[row]->size() != 0)
				{
					ReleasePtr((*m_arenaTiles[row])[m_arenaTiles[row]->size() - 1]);
					m_arenaTiles[row]->erase(m_arenaTiles[row]->begin() + m_arenaTiles[row]->size() - 1);
				}
			}
		}
	}
}