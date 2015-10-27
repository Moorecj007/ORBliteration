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
#include "../Arena/ArenaFloor.h"

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
	* @author: Callan Moore
	* @parameter: _pRenderer: Renderer for the Orb
	* @parameter: _pMesh: Mesh for the Orb
	* @parameter: _pShader: The Shader for the Orb
	* @parameter: _playerNum: The player number that owns this Orb
	* @parameter: _bounce: The Bounce strength of the Orb 
	* @parameter: _speed: The speed at which the Orb accelerates 
	* @parameter: _maxSpeed: The max speed the Orb is allowed to travel at
	* @return: bool : Successful initialization
	********************/
	bool Initialise(DX10_Renderer* _pRenderer, DX10_Mesh* _pMesh, DX10_Shader_LitTex* _pShader, int _playerNum, float _bounce, float _Speed);


	/***********************
	* ProcessFriction: Process the friction of the Orb based on the tile its on
	* @author: Jc Fowles
	* @return: void 
	********************/
	void ProcessFriction();

	/***********************
	* Process: Process the Orb
	* @author: Jc Fowles
	* @parameter: _dt: The delta tick - time since last frame
	* @return: void
	********************/
	void Process(float _dt);

	/***********************
	* Render: Draw the Orb
	* @author: Jc Fowles
	* @return: void
	********************/
	void Render();

	/***********************
	* SetAcceleration: Sets the Orbs Acceleration
	* @author: Jc Fowles
	* @parameter: _acceleration: Value to set acceleration too
	* @return: void
	********************/
	void SetAcceleration(v3float _acceleration);

	/***********************
	* SetTile: Sets the tile the Orb is on
	* @author: Jc Fowles
	* @parameter: _pTile: The Tile the Orb is on
	* @return: void
	********************/
	void SetTile(ArenaTile* _pTile){ m_pTile = _pTile; };
	
	/***********************
	* SetVelocity: Sets the Orbs Velocity
	* @author: Jc Fowles
	* @parameter: _velocity: Value to set Velocity too
	* @return: void
	********************/
	void SetVelocity(v3float _velocity){ m_velocity = _velocity; };

	/***********************
	* SetAlive: Sets the Orbs Alive state
	* @author: Jc Fowles
	* @parameter: _alive: Value to set Alive state too
	* @return: void
	********************/
	void SetAlive(bool _alive){ m_isAlive = _alive; m_pGlowLight->active = false; };

	/***********************
	* GetTile: Returns the tile the Orb is on
	* @author: Jc Fowles
	* @return: ArenaTile* : The Tile the Orb is On
	********************/
	ArenaTile* GetTile(){ return m_pTile; };

	/***********************
	* GetAlive: Returns the Orbs Alive State
	* @author: Jc Fowles
	* @return: bool* : The Orbs Alive State
	********************/
	bool GetAlive(){ return m_isAlive; };
	
	/***********************
	* GetRadius: Returns the Orbs Radius
	* @author: Jc Fowles
	* @return: float : The Orbs Radius
	********************/
	float GetRadius(){ return m_radius; };

	/***********************
	* GetVelocity: Returns the Orbs Velocity
	* @author: Jc Fowles
	* @return: v3float : The Orbs Velocity
	********************/
	v3float GetVelocity(){ return m_velocity; };
	
	/***********************
	* GetBounce: Returns the Orbs Bounce Value
	* @author: Jc Fowles
	* @return: float : The Orbs Bounce Value
	********************/
	float GetBounce(){ return m_bounce; };

	/***********************
	* GetPhase: Returns the Orbs Phase state
	* @author: Jc Fowles
	* @return: bool : The Orbs Phase state
	********************/
	bool GetPhase(){ return m_phase; };

	/***********************
	* GetScore: Returns the Orbs Current Score
	* @author: Jc Fowles
	* @return: int : The Orbs Current Score
	********************/
	int GetScore(){ return m_score; };

	/***********************
	* SetScore: Sets the Orbs Current Score
	* @author: Jc Fowles
	* @parmater: _score: The amount to set score to
	* @return: void : 
	********************/
	void SetScore(int _score){ m_score = _score; };

	/***********************
	* Boost: Activate the Orbs Boost ablility 
	* @author: Jc Fowles
	* @return: void:
	********************/
	void Boost();

	/***********************
	* Phase: Activate the Orbs Phase ablility
	* @author: Jc Fowles
	* @parmeter: _phase: Whether to enter or exit Phase
	* @return: void:
	********************/
	void Phase();

protected:
private:
	// Member Variables
public:
	bool m_collidable;
	float m_collideCountdown;
	float m_collideStartTime;

protected:
private:

	ArenaTile* m_pTile;

	v3float m_acceleration;
	v3float m_velocity;
	
	int m_score;

	float m_surfaceFriction;
	float m_speed;

	float m_bounce;
	
	float m_radius;
	bool m_isAlive;

	bool m_boost;
	bool m_AllowBoost;
	float m_boostAmount;
	float m_boostCooldown;
	float m_boostLimit;
	float m_boostActiveTime;
	float m_boostCoolDownTime;

	bool m_phase;
	bool m_AllowPhase;
	float m_phaseCooldown;
	float m_phaseMaxTime;
	float m_phaseActiveTime;
	float m_phaseCoolDownTime;

	Light* m_pGlowLight;
	std::string m_glowName;
};

#endif;		// __ORB_H__
