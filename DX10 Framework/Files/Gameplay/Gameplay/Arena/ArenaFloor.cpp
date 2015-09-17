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
	while (m_pArenaTiles->empty() == false)
	{
		while (m_pArenaTiles->back()->empty() == false)
		{
			ReleasePtr(m_pArenaTiles->back()->back());
			m_pArenaTiles->back()->pop_back();
		}

		ReleasePtr(m_pArenaTiles->back());
		m_pArenaTiles->pop_back();
	}
	ReleasePtr(m_pArenaTiles);

	ReleasePtr(m_pTileMesh);
}

bool ArenaFloor::Initialise(DX10_Renderer* _pDX10_Renderer, DX10_Shader_LitTex* _pShader, UINT _arenaSize, v3float _tileScale, float _matchLength)
{
	m_pDX10_Renderer = _pDX10_Renderer;
	m_matchLength = _matchLength;

	// Create the Mesh for the Arena Tiles
	m_pTileMesh = new DX10_Mesh_Rect_Prism();
	TVertexNormalUV vertNormalUV;
	VALIDATE(m_pTileMesh->Initialise(m_pDX10_Renderer, vertNormalUV, _tileScale));

	// Create the 2D vector of Arena Tiles
	m_pArenaTiles = new std::vector<std::vector<ArenaTile*>*>;
	for (UINT row = 0; row < _arenaSize; row++)
	{
		std::vector<ArenaTile*>* pRowOfTiles = new std::vector<ArenaTile*>;

		for (UINT col = 0; col < _arenaSize; col++)
		{
			// Create a new Tile
			ArenaTile* pTile = new ArenaTile();

			eBaseTileImages eBaseImage = (eBaseTileImages)(rand() % 3);

			VALIDATE(pTile->Initialise(m_pDX10_Renderer, m_pTileMesh, _pShader, eBaseImage));

			// Set the Position based upon the position in the 2D grid
			v3float tilePos;
			tilePos.x = ((float)row - ((float)_arenaSize / 2.0f)) * _tileScale.x + (_tileScale.x / 2.0f);
			tilePos.y = ((float)col - ((float)_arenaSize / 2.0f)) * _tileScale.y + (_tileScale.y / 2.0f);
			tilePos.z = 0.0f;
			pTile->SetPosition(tilePos);

			// Calculate the World matrix at Initialization only
			pTile->CalcWorldMatrix();

			// Add the new Tile to the Row
			pRowOfTiles->push_back(pTile);
		}
		m_pArenaTiles->push_back(pRowOfTiles);
	}

	// Calculate the time interval for destroying the outer layer
	int evenSize;
	// Ensure the Arena size is even for the next calculation
	(_arenaSize % 2 == 1) ? evenSize = ++_arenaSize : evenSize = _arenaSize;
	int layerCount = evenSize / 2;
	m_destroyOutsideTime = m_matchLength / (float)layerCount;

	return true;
}

void ArenaFloor::Process(float _dt)
{
	m_timeElapsed += _dt;
	if (m_timeElapsed >= m_destroyOutsideTime)	// TO DO make variable
	{
		DestroyOuterLayer();

		m_timeElapsed = 0.0f;
	}

	// Process all the Tiles in the 2D vector
	// Cycle through all rows
	for (UINT row = 0; row < m_pArenaTiles->size(); row++)
	{
		// Cycle through all columns (elements in the rows)
		for (UINT col = 0; col < (*m_pArenaTiles)[row]->size(); col++)
		{
			(*(*m_pArenaTiles)[row])[col]->Process(_dt);
		}
	}
}

void ArenaFloor::Render()
{
	if (m_pArenaTiles->size() == 0)
	{
		// Nothing to render, return.
		return;
	}

	// Cycle through all rows
	for (UINT row = 0; row < m_pArenaTiles->size(); row++)
	{
		// Cycle through all columns (elements in the rows)
		for (UINT col = 0; col < (*m_pArenaTiles)[row]->size(); col++)
		{
			(*(*m_pArenaTiles)[row])[col]->Render();
		}
	}
}

void ArenaFloor::DestroyOuterLayer()
{
	if (m_pArenaTiles->size() != 0)
	{
		// Delete the Entire first rows individual elements
		while ((*m_pArenaTiles)[0]->empty() == false)
		{
			ReleasePtr((*(*m_pArenaTiles)[0])[0]);
			(*m_pArenaTiles)[0]->erase((*m_pArenaTiles)[0]->begin());
		}
		// Delete the row
		ReleasePtr((*m_pArenaTiles)[0]);
		m_pArenaTiles->erase(m_pArenaTiles->begin());

		// Ensure the last row was not deleted
		if (m_pArenaTiles->size() != 0)
		{
			// Delete the entire last rows individual elements
			while ((*m_pArenaTiles)[m_pArenaTiles->size() - 1]->empty() == false)
			{
				ReleasePtr((*(*m_pArenaTiles)[m_pArenaTiles->size() - 1])[0]);
				(*m_pArenaTiles)[m_pArenaTiles->size() - 1]->erase((*m_pArenaTiles)[m_pArenaTiles->size() - 1]->begin());
			}

			// Delete the Last row
			ReleasePtr((*m_pArenaTiles)[m_pArenaTiles->size() - 1]);
			m_pArenaTiles->erase(m_pArenaTiles->begin() + m_pArenaTiles->size() - 1);
		}

		// Delete the first and last elements within each remaining row
		for (int row = (m_pArenaTiles->size() - 1); row >= 0; row--)
		{
			// ensure the row has one element in it
			if ((*m_pArenaTiles)[row]->size() != 0)
			{
				// Delete the first element
				ReleasePtr((*(*m_pArenaTiles)[row])[0]);
				(*m_pArenaTiles)[row]->erase((*m_pArenaTiles)[row]->begin());

				// ensure the row has one element in it
				if ((*m_pArenaTiles)[row]->size() != 0)
				{
					// Delete the last element
					ReleasePtr((*(*m_pArenaTiles)[row])[(*m_pArenaTiles)[row]->size() - 1]);
					(*m_pArenaTiles)[row]->erase((*m_pArenaTiles)[row]->begin() + (*m_pArenaTiles)[row]->size() - 1);
				}
			}
		}
	}
}