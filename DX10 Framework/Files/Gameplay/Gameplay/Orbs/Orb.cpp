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

// This Include
#include "Orb.h"


Orb::Orb()
{
	m_acceleration = {0.0f,0.0f,0.0f};
	m_velocity = { 0.0f, 0.0f, 0.0f };
	m_density = 0.0f;
	m_maxSpeed = 0.0f;
	m_isAlive = true;
}

Orb::~Orb()
{
}

bool Orb::Initialise(DX10_Renderer* _pRenderer, DX10_Mesh_Generic* _pMesh, DX10_Shader_LitTex* _pShader, UINT _textureID, float _density, float _speed, float _maxSpeed)
{	
	// Initialise the object this is derived from
	VALIDATE(DX10_Obj_LitTex::Initialise(_pRenderer, _pMesh, _pShader, _textureID));

	// Check the passed in parameters
	if ((_density < 0.0f) || (_maxSpeed < 0.0f))
	{
		return false;
	}

	// Store the initial state of the variables
	m_density = _density;
	m_AccelerationSpeed = _speed;
	m_DecelerationSpeed = _speed;
	m_maxSpeed = _maxSpeed;
	m_isAlive = true;
	   
	// Succesful Initialization
	return true;
}
  
void Orb::Process(float _dt)
{
	// TO DO JC: Multiply by Delta Tick
	m_velocity += ((m_acceleration * _dt) * m_AccelerationSpeed);
	m_velocity.Limit(m_maxSpeed);
	m_pos += m_velocity;

	//Yaw 
	// Pitch
	SetRotPerSecondPitch(m_pos.y);
	SetRotPerSecondYaw(-m_pos.x);

	m_acceleration *= 0.0f;
	// TO DO JC: This will change based on the friction of the tiles
	//m_acceleration *= 0.0f;

	DX10_Obj_LitTex::Process(_dt);
}
