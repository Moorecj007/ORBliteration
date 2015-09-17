/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : ArenaFloor.h
* Description : Controlling class for all arena related processes. Holds and controls all individual tiles.
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __ARENAFLOOR_H__
#define __ARENAFLOOR_H__

// Library Includes
#include <thread>

// Local Includes
#include "../../../Utility/Utilities.h"
#include "../../../DX10/DX10/DX10_Renderer.h"
#include "../../../DX10/DX10/Meshes/DX10_Mesh_Rect_Prism.h"
#include "ArenaTile.h"
#include "../../../Utility/Mutex_Semaphore.h"

class ArenaFloor
{
public:

	/***********************
	* ArenaFloor: Default Constructor for Arena Floor class
	* @author: Callan Moore
	********************/
	ArenaFloor();

	/***********************
	* ~ArenaFloor: Default Destructor for Arena Floor class
	* @author: Callan Moore
	********************/
	~ArenaFloor();

	/***********************
	* Initialise: Initialise the ArenaFloor for use
	* @author: Callan Moore
	* @parameter: _pDX10_Renderer: The renderer to use for this object
	* @parameter: _pShader: The shader to use for each tile in the arena floor
	* @parameter: _arenaSize: The size of each side of the arena
	* @parameter: _tileScale: The scale for each tile of the arena floor
	* @parameter: _matchLength: The maximum time for the match to run for. Default to 60 seconds
	* @return: bool: Successful or not
	********************/
	bool Initialise(DX10_Renderer* _pDX10_Renderer, DX10_Shader_LitTex* _pShader, UINT _arenaSize, v3float _tileScale, float _matchLength = 60.0f);
	
	/***********************
	* Process: Process the ArenaFloor
	* @author: Callan Moore
	* @parameter: _dt: The current delta tick
	* @return: void
	********************/
	void Process(float _dt);
	
	/***********************
	* Render:Render the Arena floor
	* @author: Callan Moore
	* @return: void
	********************/
	void Render();

	/***********************
	* GetArenaTiles: Retrieve the Arena Tiles vector
	* @author: Callan Moore
	* @return: std::vector<std::vector<ArenaTile*>*>*: The vector of all Arena Tiles
	********************/
	std::vector<std::vector<ArenaTile*>*>* GetArenaTiles() { return m_pArenaTiles; };

	// TO DO JC
	v2float GetTile(v3float _orbPos);
	
private:

	/***********************
	* StartDeathOuterLayer: Destroy the Outer layer of the Arena
	* @author: Callan Moore
	* @return: void
	********************/
	void StartDeathOuterLayer();

	// TO DO
	void StartTileDeath(UINT _row, UINT _col);

private:
	DX10_Renderer* m_pDX10_Renderer;
	DX10_Mesh_Rect_Prism* m_pTileMesh;
	std::vector<std::vector<ArenaTile*>*>* m_pArenaTiles;	


	float m_timeElapsed;
	float m_matchLength;
	float m_destroyOutsideTime;
	int m_layerCount;
	int m_destroyedLayers;
};
#endif	// __ARENAFLOOR_H__

