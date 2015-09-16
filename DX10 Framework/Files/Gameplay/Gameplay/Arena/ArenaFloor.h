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

	// TO DO CAL
	bool Initialise(DX10_Renderer* _pDX10_Renderer, DX10_Shader_LitTex* _pShader, UINT rowCount, UINT colCount, v3float _tileScale);
	void Process(float _dt);
	void Render();
	void DestroyOuterLayer();



private:
	DX10_Renderer* m_pDX10_Renderer;
	DX10_Mesh_Rect_Prism* m_pTileMesh;
	std::vector<std::vector<ArenaTile*>*> m_arenaTiles;	

	float m_timeElapsed;

	std::vector<std::thread> m_threadPool;
};
#endif	// __ARENAFLOOR_H__

