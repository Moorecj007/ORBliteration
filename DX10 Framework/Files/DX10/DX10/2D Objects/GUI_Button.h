//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	:	GUI_Button.h
// Description	:	This wraps all the bitmap functions and variables
// Author		:	Juran Griffith
// Mail			:	juran.griffith@mediadesignschool.com
//

#pragma once
#ifndef __GUI_BUTTON_H__
#define __GUI_BUTTON_H__

// Library Includes

// Local Includes
#include "DXSprite.h"

enum BUTTON_STATE
{
	BUTTON_STATE_DEFAULT,
	BUTTON_STATE_HOVER,
	BUTTON_SELECTED,
	BUTTON_DEACTIVATED
};

class GUI_Button
{
	public:
		/*******************
		-> Default constructor.
		@author:	Juran Griffith.
		@parameter:	None.
		@return:	...
		********************/
		GUI_Button();

		/*******************
		-> Default destructor.
		@author:	Juran Griffith.
		@parameter:	None.
		@return:	...
		********************/
		virtual ~GUI_Button();

		/*******************
		-> Intializes the object
		@author:	Juran Griffith.
		@parameter:
		@return:	bool - Returns true if the intialization was a success, otherwise false
		********************/
		virtual bool Initialize(DX10_Renderer* _pDX10_Renderer, DXSprite* _pSprite, float _x, float _y, float _width, float _height);

		/*******************
		-> Gets the objects position on the screen.
		@author:	Juran Griffith.
		@parameter:	_out		- The position of the object
		@return:	void
		********************/
		virtual void GetPosition(v2float& _out);

		/*******************
		-> Gets the objects current state
		@author:	Juran Griffith.
		@parameter:	None.
		@return:	eState		- The current state the object is in.
		********************/
		virtual BUTTON_STATE GetState();

		/*******************
		-> Sets the objects new position
		@author:	Juran Griffith.
		@parameter:	_x	- The x position on the screen.
		@parameter:	_y	- The y position on the screen.
		@return:	void
		********************/
		virtual void SetPosition(float _x, float _y);

		/*******************
		-> Sets the objects new state
		@author:	Juran Griffith.
		@parameter:	_state	- The new state to set the object to.
		@return:	void
		********************/
		virtual void SetState(BUTTON_STATE _state);

		/*******************
		-> Set the bounding box. Currently useful for rectangles. It shrinks the box towards the centre of the object
		@author:	Juran Griffith.
		@parameter:	_offset	- The amount to shrink the bounding box to.
		@return:	void
		********************/
		virtual void SetBoundsOffset(float _offset);

		/*******************
		-> Draws the objects
		@author:	Juran Griffith.
		@parameter:	None.
		@return:	void
		********************/
		virtual void Draw();

		/*******************
		-> Updates the object
		@author:	Juran Griffith.
		@parameter:	_deltaTime	- The application change in time.
		@return:	void
		********************/
		virtual void Process(float _deltaTime);

		/*******************
		-> This checks if a position is within the object bounds
		@author:	Juran Griffith.
		@parameter:	_point	- The point to check if its within the objects bounds. Good for mouse input.
		@return:	void
		********************/
		virtual bool IsInBounds(v2float _point);
	protected:
	private:
	
	// Variables
	public:
	protected:
		DX10_Renderer*		m_pDX10_Renderer;
		DXSprite*			m_pSprite;

		shape::Rectangle	m_bounds;
		D3DXVECTOR2			m_position;
		float				m_boundsOffset;
		BUTTON_STATE		m_state;

		float				m_width;
		float				m_height;

		bool				m_horizontal;
	private:
};

#endif