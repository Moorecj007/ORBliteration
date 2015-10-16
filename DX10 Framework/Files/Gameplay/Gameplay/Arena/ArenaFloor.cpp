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
	if (m_pArenaTiles != 0)
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
	}

	ReleasePtr(m_pTileMesh);
}

bool ArenaFloor::Initialise(DX10_Renderer* _pDX10_Renderer, DX10_Shader_LitTex* _pShader, UINT _arenaSize, v3float _tileScale, float _matchLength)
{
	m_pDX10_Renderer = _pDX10_Renderer;
	m_matchLength = _matchLength;

	m_tileScale = _tileScale;
	m_arenaSize = _arenaSize;

	// Create the Mesh for the Arena Tiles
	m_pTileMesh = new DX10_Mesh();
	VALIDATE(m_pTileMesh->Initialise(m_pDX10_Renderer, MT_FINITEPLANE, _tileScale));

	// Create the 2D vector of Arena Tiles
	m_pArenaTiles = new std::vector<std::vector<ArenaTile*>*>;
	for (UINT row = 0; row < _arenaSize; row++)
	{
		std::vector<ArenaTile*>* pRowOfTiles = new std::vector<ArenaTile*>;

		for (UINT col = 0; col < _arenaSize; col++)
		{
			// Create a new Tile
			ArenaTile* pTile = new ArenaTile();

			// Create the smooth/rough tiles
			eBaseTileImages eBaseImage;
			int randomChance =  rand() % 10;
			if (randomChance < 1)
			{
				eBaseImage = BTI_ROUGH;
			}
			else if (randomChance < 3)
			{
				eBaseImage = BTI_SLIPPERY;
			}
			else
			{
				eBaseImage = BTI_STANDARD;
			}

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

	// Set Spawning tiles to standard
	(*(*m_pArenaTiles)[ 1])[1]->SetBaseImageEnum(BTI_STANDARD);
	(*(*m_pArenaTiles)[1])[_arenaSize - 2]->SetBaseImageEnum(BTI_STANDARD);
	(*(*m_pArenaTiles)[_arenaSize - 2])[1]->SetBaseImageEnum(BTI_STANDARD);
	(*(*m_pArenaTiles)[_arenaSize - 2])[_arenaSize - 2]->SetBaseImageEnum(BTI_STANDARD);

	// Calculate the time interval for destroying the outer layer
	int evenSize;
	// Ensure the Arena size is even for the next calculation
	(_arenaSize % 2 == 1) ? evenSize = ++_arenaSize : evenSize = _arenaSize;
	m_layerCount = evenSize / 2;
	m_destroyOutsideTime = m_matchLength / (float)m_layerCount + 1;

	m_destroyedLayers = 0;

	return true;
}

void ArenaFloor::Process(float _dt)
{
	m_timeElapsed += _dt;
	if (m_timeElapsed >= m_destroyOutsideTime)
	{
		StartDeathOuterLayer();

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

void ArenaFloor::StartDeathOuterLayer()
{
	if (m_destroyedLayers <= m_layerCount)
	{	
		for (UINT col = m_destroyedLayers; col < (*m_pArenaTiles)[m_destroyedLayers]->size() - m_destroyedLayers - 1; col++)
		{
			int row = m_destroyedLayers;
			StartTileDeath(row, col);		
		}

		for (UINT col = m_destroyedLayers; col < (*m_pArenaTiles)[m_destroyedLayers]->size() - m_destroyedLayers - 1; col++)
		{
			int row = (*m_pArenaTiles)[m_destroyedLayers]->size() - m_destroyedLayers - 1;
			StartTileDeath(row, col);
		}

		for (UINT row = m_destroyedLayers; row < m_pArenaTiles->size() - m_destroyedLayers; row++)
		{
			int col = m_destroyedLayers;
			StartTileDeath(row, col);

			col = (*m_pArenaTiles)[row]->size() - 1 - m_destroyedLayers;
			StartTileDeath(row, col);
		}
	}

	// Increase the Count of destroyed layers
	m_destroyedLayers++;	
}

void ArenaFloor::StartTileDeath(UINT _row, UINT _col)
{
	float modifier = (float)(rand() % 80 - 40);
	modifier = modifier / 100.0f + 1.0f;

	(*(*m_pArenaTiles)[_row])[_col]->SetDeathTimer(m_destroyOutsideTime / 2 * modifier);
}

bool ArenaFloor::GetTile(v3float _orbPos, ArenaTile*& _pReturnTile)
{
	// Calculate the tile the Orb is on
	_orbPos += m_tileScale / 2;
	float row = (float)((_orbPos.x / m_tileScale.x) + ((m_arenaSize - 1) / 2));
	float col = (float)((_orbPos.y / m_tileScale.y) + ((m_arenaSize - 1) / 2));

	// Check if the orb is with in the Arena if not return false;
	//bool result;

	// Check if it in the bounds of the Rows
	if (row < 0)
	{
		row = 0;
		_pReturnTile = 0;
		return false;
	}
	else if (row >= m_pArenaTiles->size())
	{
		row = (float)(m_pArenaTiles->size() - 1);
		_pReturnTile = 0;
		return false;
	}
	// Check if it in the bounds of the Columns
	if (col < 0)
	{
		col = 0;
		_pReturnTile = 0;
		return false;
	}
	else if (col >= m_pArenaTiles->size())
	{
		col = (float)(m_pArenaTiles->size() - 1);
		_pReturnTile = 0;
		return false;
	}

	int iRow = (int)row;
	int iCol = (int)col;

	// Return false if the orb is on a Dead Tile
	if ((*(*m_pArenaTiles)[iRow])[iCol]->GetActive() == false)
	{
		_pReturnTile = 0;
		return false;
	}
	// Orb is still alive return the true and the tile
	else
	{
		_pReturnTile = (*(*m_pArenaTiles)[iRow])[iCol];
		return true;
	}
}

