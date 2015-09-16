/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : ArenaTile.h
* Description : Tile to hold information about a small section of the Arena Floor
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __ARENATILE_H__
#define __ARENATILE_H__

// Local Includes
#include "../../../DX10/DX10/3D Objects/DX10_Obj_Generic.h"
#include "../../../DX10/DX10/Meshes/DX10_Mesh_Generic.h"
#include "../../../DX10/DX10/Shaders/DX10_Shader_LitTex.h"

class ArenaTile :
	public DX10_Obj_Generic
{
public:

	/***********************
	* ArenaTile: Default Constructor for Arena Tile class
	* @author: Callan Moore
	********************/
	ArenaTile();

	/***********************
	* ~ArenaTile: Default Destructor for Arena Tile class
	* @author: Callan Moore
	********************/
	~ArenaTile();

	// TO DO
	bool Initialise(DX10_Renderer* _pDX10_Renderer, DX10_Mesh_Generic* _pMesh, DX10_Shader_LitTex* _pShader, UINT _texID);
	void Process(float _dt);
	void Render();

private:
	DX10_Shader_LitTex* m_pShader_LitTex;
	UINT m_texID;
};
#endif	// __ARENATILE_H__
