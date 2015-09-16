//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	:	Menu.h
// Description	:	This wraps all the menu objects, handles the menu item interactions and its current state.
// Author		:	Juran Griffith
// Mail			:	juran.griffith@mediadesignschool.com
//

// Inclusion Guards
#pragma once
#ifndef __MENU_H__
#define __MENU_H__

// Local Includes
#include "../DX10/DX10/2D Objects/GUI_Button.h"
//#include "../DX10/DX10/DX10_Utilities.h"
//#include "../DX10/DX10/DX10_Renderer.h"

struct Sprite
{
	Sprite();
	~Sprite()
	{
		ReleasePtr(m_sprite);
	}
	UINT m_spriteID;
	DXSprite* m_sprite;
};

class Menu
{
	// Functions
	public:
		/*******************
		-> Default constructor.
		@author:	Juran Griffith.
		@parameter:	None.
		@return:	...
		********************/
		Menu();

		/*******************
		-> Default destructor.
		@author:	Juran Griffith.
		@parameter:	None.
		@return:	...
		********************/
		~Menu();

		/***********************
		-> Initialize the object
		@author:	Juran Griffith.
		@parameter: _pDX10_Renderer	- The graphics device.
		@parameter: _pHWnd			- The handler to the window.
		@parameter: _scale			- How much to scale the menu by. (The ability to shrink all the menu items)
		@return:	bool			- Successful or not
		********************/
		bool Initialize(DX10_Renderer* _pDX10_Renderer, HWND* _pHWnd);

		/*******************
		-> Draws the object
		@author:	Juran Griffith.
		@parameter:	None.
		@return:	void
		********************/
		void Draw();

		/*******************
		-> Process the object
		@author:	Juran Griffith.
		@parameter:	_deltaTime	- The application change in time.
		@return:	void
		********************/
		void Process(float _deltaTime);

		/***********************
		-> Adds a new sprite to the menus list of sprites (may need to improve this). This also loads the sprite aswell.
		@author:	Juran Griffith.
		@parameter: _filename		- The filename/path of the image
		@parameter: _imageWidth		- The image width.
		@parameter: _imageHeight	- The image height.
		@parameter: _sliceWidth		- How many slices along the width of the image to do to split the image up into sections. (For sprite sheets etc...)
		@parameter: _sliceHeight	- How many slices along the height of the image to do to split the image up into sections. (For sprite sheets etc...)
		@return:	bool			- True if added otherwise false.
		********************/
		bool AddSprite(std::string _filename, UINT _imageWidth, UINT _imageHeight, UINT _sliceWidth = 1, UINT _sliceHeight = 1);

		/***********************
		-> Adds a new button to the menu. By default it is placed after the last button added.
		@author:	Juran Griffith.
		@parameter: _index		- The sprite index to set as the buttons image.
		@return:	bool		- True if added otherwise false.
		********************/
		bool AddButton(UINT _index = 0, float _scale = 1.0f);

		/***********************
		-> Gets the current button state based on the menu index provided.
		@author:	Juran Griffith.
		@parameter: _index			- The button index to check.
		@return:	BUTTON_STATE	- The current buttons state.
		********************/
		BUTTON_STATE GetButtonState(UINT _index);

		/***********************
		-> Gets the current menu item the menu is hovering over(for mouse) or at(xbox)
		@author:	Juran Griffith.
		@parameter: None.
		@return:	UINT	- The current menu item
		********************/
		UINT GetCurrentMenuItem();

		/***********************
		-> Sets the menus position
		@author:	Juran Griffith.
		@parameter: _position		- The new position to set the menu
		@return:	void
		********************/
		void SetPosition(v2float _position);

		/***********************
		-> Scales all the menu objects by a set amount
		@author:	Juran Griffith.
		@parameter: _scale			- How much to scale the menu objects by
		@return:	void
		********************/
		void SetScale(float _scale);
	protected:
	private:

	// Variables
	public:
	protected:
	private:
		HWND*						m_pHWnd;
		DX10_Renderer*				m_pDX10_Renderer;

		float						m_scale;

		// Sprite
		int							m_menuItem;
		std::vector<DXSprite*>		m_sprites;
		std::vector<GUI_Button*>	m_buttons;
		DX10_Shader_Sprite*			m_pShader_Sprite;
		v2float						m_position;
};

#endif