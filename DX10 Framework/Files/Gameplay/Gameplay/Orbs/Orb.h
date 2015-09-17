//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : Orb.h
// Description : Class to handle all funtionality of the Player Controlled Orbs
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//
#pragma once

#ifndef __ORB_H__
#define __ORB_H__

// Library Include

// Local Includes
#include "../../../DX10/DX10/3D Objects/DX10_Obj_Generic.h"
#include "../../../DX10/DX10/3D Objects/DX10_Obj_LitTex.h"



class Orb :
	public DX10_Obj_LitTex
{
	// Member Functions
public:

	/***********************
	* Orb: Default Constructor for the Player Controlled Orbs
	* @author: Jc Fowles
	* @return:
	********************/
	Orb();

	/***********************
	* Orb: Default Destructor for the Player Controlled Orbs
	* @author: Jc Fowles
	* @return:
	********************/
	~Orb();

	/***********************
	* Initialise: Initialise the Player Controlled Orbs
	* @author: Jc Fowles
	* @parameter: _pRenderer: Renderer for the Orb
	* @parameter: _pMesh: Mesh for the Orb
	* @parameter: _pShader: The Shader for the Orb
	* @parameter: _texName: file name of the single texture for the Orb to display
	* @parameter: _density: The Density of the Orb to calculate its mass
	* @parameter: _speed: The speed at which the Orb accelerates 
	* @parameter: _maxSpeed: The max speed the Orb is allowed to travel at
	* @return: bool : Successful initialization
	********************/
	bool Initialise(DX10_Renderer* _pRenderer, DX10_Mesh_Generic* _pMesh, DX10_Shader_LitTex* _pShader, std::string _texName, float _density, float _Speed, float _maxSpeed);


	// TO DO JC:
	void Process(float _dt);

   	void SetSurfaceFriction(float _surfaceFriction){ m_surfaceFriction = _surfaceFriction; };
	void SetAcceleration(v3float _acceleration){ m_acceleration = _acceleration; };
	
		
protected:
private:
	// Member Variables
public:
	float m_surfaceFriction;
protected:
private:

	v3float m_acceleration;
	v3float m_velocity;
	float m_speed;
	float m_maxSpeed;
	float m_density;
	bool m_isAlive;


	
};

#endif;		// __ORB_H__
