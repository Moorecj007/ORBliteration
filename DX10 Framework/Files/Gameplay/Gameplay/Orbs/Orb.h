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

	//void Accelerate();
	void Decelerate()
	{
		v3float deceleration;
		deceleration = (m_velocity * m_DecelerationSpeed) * -1.0f;
		if ((deceleration.x > -0.0001f) && (deceleration.x < 0.0001f))
		{
			deceleration.x = 0.0f;
		}
		if ((deceleration.y > -0.0001f) && (deceleration.y < 0.0001f))
		{
			deceleration.y = 0.0f;
		}

		m_acceleration = deceleration;
	};

	void SetAcceleration(v3float _acceleration){ m_acceleration = _acceleration; };
	v3float GetAcceleration(){ return m_acceleration; };

	void SetVelocity(v3float _velocity){ m_velocity = _velocity; };
	v3float GetVelocity(){ return m_velocity; };
protected:
private:
	// Member Variables
public:
protected:
private:

	v3float m_acceleration;
	v3float m_velocity;
	float m_AccelerationSpeed;
	float m_DecelerationSpeed;
	float m_maxSpeed;
	float m_density;
	bool m_isAlive;


	
};

#endif;		// __ORB_H__
