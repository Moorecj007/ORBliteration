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
#include "../../../DX10/DX10/3D Objects/DX10_Obj_LitTex.h"

/***********************
* eBaseTileImages: The image list of textures that can be applied to the base of a tile
* @author: Callan Moore
********************/
enum eBaseTileImages
{
	BTI_STANDARD,	
	BTI_SLIPPERY,	
	BTI_ROUGH
};

class ArenaTile :
	public DX10_Obj_LitTex
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

	/***********************
	* Initialise: Initialise the ArenaTilefor use
	* @author: Callan Moore
	* @parameter: _pDX10_Renderer: The renderer for this object
	* @parameter: _pMesh: The mesh to create this tile with
	* @parameter: _pShader: The shader to use for rendering this tile
	* @parameter: _baseImage: Enum to indicate which image to display as the tiles base image
	* @return: bool: Successful or not
	********************/
	bool Initialise(DX10_Renderer* _pDX10_Renderer, DX10_Mesh* _pMesh, DX10_Shader_LitTex* _pShader, eBaseTileImages _baseImage);
	
	/***********************
	* Process: Process the Tile
	* @author: Callan Moore
	* @parameter: _dt: The current delta tick
	* @return: void
	********************/
	void Process(float _dt);
	
	/***********************
	* Render: Render the Tile
	* @author: Callan Moore
	* @return: void
	********************/
	void Render();
	
	/***********************
	* GetBaseImage: Retrieve the Base Image indicator for which image is displayed on the tile
	* @author: Callan Moore
	* @return: eBaseTileImages: Enum for the displayed image
	********************/
	eBaseTileImages GetBaseImageEnum() { return m_baseImage; };
	
	/***********************
	* GetActive: Retrieve the active state of the tile
	* @author: Callan Moore
	* @return: bool: The active state
	********************/
	bool GetActive() { return m_active; };

	/***********************
	* SetBaseImageEnum: Sets the Base Image indicator for which image is displayed on the tile
	* @author: Jc Fowles
	* @parameter: _baseImage: The new enum to determine the base image
	* @return: bool: Successful set
	********************/
	bool SetBaseImageEnum(eBaseTileImages _baseImage);

	/***********************
	* SetActive: Set the Active state of the tile
	* @author: Callan Moore
	* @parameter: _active: The new active state
	* @return: void
	********************/
	void SetActive(bool _active){ m_active = _active; };

	/***********************
	* SetDeathTimer: Set the time taken for this tile to die and start that timer
	* @author: Callan Moore
	* @parameter: _deathTime: The time taken for this tile to die and become inactive
	* @return: void
	********************/
	void SetDeathTimer(float _deathTime) { m_deathTimer = 0; m_deathIncrement = _deathTime / 10;};

private:
	eBaseTileImages m_baseImage;

	bool m_active;
	float m_deathTimer;
	float m_deathIncrement;
	float m_reduceAlpha;
	int m_deathFrameCount;
};
#endif	// __ARENATILE_H__
