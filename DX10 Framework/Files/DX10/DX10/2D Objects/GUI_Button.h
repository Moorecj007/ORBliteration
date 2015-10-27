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
	BUTTON_STATE_SELECTED,
	BUTTON_STATE_DEACTIVATED
};

class GUI_Button
{
	public:
		/***********************
		* GUI_Button: Default constructor for the GUI_Button class.
		* @author: Juran Griffith.
		********************/
		GUI_Button();

		/***********************
		* GUI_Button: Default destructor for the GUI_Button class.
		* @author: Juran Griffith.
		********************/
		virtual ~GUI_Button();

		/*******************
		* Initialise: Intialise the GUI_Button.
		* @author: Juran Griffith.
		* @parameter: _pDX10_Renderer: The graphics device.
		* @parameter: _pSprite: The sprite image.
		* @parameter: _x: The x position on the screen, from the top corner (0,0).
		* @parameter: _y: The y position on the screen, from the top corner (0,0).
		* @parameter: _width: How wide to make the button.
		* @parameter: _height: How tall to make the button.
		* @return: bool: Returns true if the intialization was a success, otherwise false
		********************/
		virtual bool Initialise(DX10_Renderer* _pDX10_Renderer, DXSprite* _pSprite, float _x, float _y, float _width, float _height);

		/*******************
		* GetPosition: Gets the position on the screen.
		* @author: Juran Griffith.
		* @return: v2float: The position of the object.
		********************/
		virtual v2float GetPosition();

		/*******************
		* GetState: Gets the current state.
		* @author: Juran Griffith.
		* @return: BUTTON_STATE: The current state the object is in.
		********************/
		virtual BUTTON_STATE GetState();

		/*******************
		* GetSprite: Gets the current sprite.
		* @author: Juran Griffith.
		* @return: DXSprite: A pointer to this objects sprite.
		********************/
		virtual DXSprite* GetSprite();

		/*******************
		* GetWidth: Gets the width.
		* @author: Juran Griffith.
		* @return: float: The width of the image.
		********************/
		float GetWidth();

		/*******************
		* GetHeight: Gets the height.
		* @author: Juran Griffith.
		* @return: float: The height of the image.
		********************/
		float GetHeight();

		/*******************
		* GetScale: Gets the scale.
		* @author: Juran Griffith.
		* @return: float: The scale of the image.
		********************/
		float GetScale();

		/*******************
		* SetPosition: Sets the objects new position.
		* @author: Juran Griffith.
		* @parameter: _position	:The new position to set
		* @return: void.
		********************/
		virtual void SetPosition(v2float _position);

		/*******************
		* SetState: Sets the objects new state.
		* @author: Juran Griffith.
		* @parameter: _state: The new state to set the object to.
		* @return: void.
		********************/
		virtual void SetState(BUTTON_STATE _state);

		/*******************
		* SetBoundsOffset: Set the bounding box. Currently useful for rectangles. It shrinks the box towards the centre of the object.
		* @author: Juran Griffith.
		* @parameter: _offset: The amount to shrink the bounding box to.
		* @return: void.
		********************/
		virtual void SetBoundsOffset(float _offset);

		/*******************
		* SetColumnIndex: Set the buttons column index to render a different part of the image along the column.
		* @author: Juran Griffith.
		* @parameter: _index: The index to change to.
		* @return: void.
		********************/
		virtual void SetColumnIndex(UINT _index);

		/*******************
		* SetRowIndex: Set the buttons row index to render a different part of the image along the row.
		* @author: Juran Griffith.
		* @parameter: _index: The index to change to.
		* @return: void.
		********************/
		virtual void SetRowIndex(UINT _index);

		/*******************
		* SetScale: Set the scale for this button
		* @author: Juran Griffith.
		* @parameter: _scale: The scale to change to.
		* @return: void.
		********************/
		void SetScale(float _scale);

		/*******************
		* Draw: Draws the object.
		* @author:	Juran Griffith.
		* @return:	void.
		********************/
		virtual void Draw();

		/*******************
		* IsInBounds: This checks if a position is within the object bounds.
		* @author: Juran Griffith.
		* @parameter: _point: The point to check if its within the objects bounds. Good for mouse input.
		* @return: void.
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
		v2float				m_position;
		//v2float				m_parentPosition;
		float				m_boundsOffset;
		BUTTON_STATE		m_state;

		float				m_width;
		float				m_height;
		float				m_scale;

		bool				m_horizontal;

		UINT				m_col;
		UINT				m_row;
	private:
};

#endif