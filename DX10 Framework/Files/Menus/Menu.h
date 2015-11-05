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
#include "..\DX10\DX10\2D Objects\GUI_Button.h"
#include "..\Input\InputGamePad.h"
#include "..\Sound\SoundManager.h"

enum eMenuState
{
	// Main menu states
	MENU_STATE_DEFAULT,
	MENU_STATE_START,
	MENU_STATE_INSTRUCTIONS,
	MENU_STATE_OPTIONS,
	MENU_STATE_EXIT,

	// Options menu states
	MENU_STATE_FULL_SCREEN,
	MENU_STATE_SOUND,
	MENU_STATE_RUMBLE,

	// Pause menu states (reuses the main menu states)
	MENU_STATE_RESUME,

	// Return to the previous menu
	MENU_STATE_BACK
};

enum eMenuLayout
{
	MENU_LAYOUT_CENTRE,
	MENU_LAYOUT_CUSTOM
};

struct TButton
{
	~TButton()
	{
		ReleasePtr(m_pButton);
	}

	void SetButton(GUI_Button* _pButton, eMenuState _option, UINT _index, float _scale, UINT _col, UINT _row)
	{
		m_pButton = _pButton;
		m_option = _option;
		m_index = _index;
		m_scale = _scale;
		m_pButton->SetColumnIndex(_col);
		m_pButton->SetRowIndex(_row);
	}

	GUI_Button* m_pButton = 0;
	eMenuState m_option = MENU_STATE_DEFAULT;
	UINT m_index = 0;
	float m_scale = 1.0f;
};

class Menu
{
	// Functions
	public:
		/***********************
		* Menu: Default constructor for the menu class.
		* @author: Juran Griffith.
		********************/
		Menu();

		/***********************
		* Menu: Default destructor for the menu class.
		* @author: Juran Griffith.
		********************/
		~Menu();

		/***********************
		* Initialise: Initialise the menu.
		* @author:	Juran Griffith.
		* @parameter: _pDX10_Renderer: The graphics device.
		* @parameter: _pShader: The shader object to use for rendering the sprite.
		* @parameter: _pSoundManager: The sound manager to help play sound effects in the menu.
		* @parameter: _pKeyDown
		* @parameter: _layout: How to layout the menu items.
		* @return:	bool: Successful or not
		********************/
		bool Initialise(DX10_Renderer* _pDX10_Renderer, DX10_Shader_Sprite* _pShader, SoundManager*	_pSoundManager, bool* _pKeyDown, eMenuLayout _layout = MENU_LAYOUT_CENTRE);

		/*******************
		* Draw: Draws the object
		* @author:	Juran Griffith.
		* @return:	void
		********************/
		void Draw();

		/*******************
		* Process: Process the object.
		* @author:	Juran Griffith.
		* @parameter: _deltaTime: The application change in time.
		* @return:	void.
		********************/
		void Process(float _deltaTime);

		/***********************
		* AddSprite: Adds a new sprite to the menus list of sprites (may need to improve this). This also loads the sprite aswell.
		* @author:	Juran Griffith.
		* @parameter: _filename: The filename/path of the image
		* @parameter: _imageWidth: The image width.
		* @parameter: _imageHeight: The image height.
		* @parameter: _sliceWidth: How many slices along the width of the image to do to split the image up into sections. (For sprite sheets etc...)
		* @parameter: _sliceHeight: How many slices along the height of the image to do to split the image up into sections. (For sprite sheets etc...)
		* @return:	bool: True if added otherwise false.
		********************/
		bool AddSprite(std::string _filename, UINT _imageWidth, UINT _imageHeight, UINT _sliceWidth = 1, UINT _sliceHeight = 1);

		/***********************
		* AddButton: Adds a new button to the menu. By default it is placed after the last button added.
		* @author:	Juran Griffith.
		* @parameter: _option: The menu state option this button refers to.
		* @parameter: _spriteIndex: The sprite index to set as the buttons image. (make sure to load a sprite to the menu object using the add sprite function)
		* @parameter: _scale: How big to make the object.
		* @parameter: _col: The col image index.
		* @parameter: _row: The row image index.
		* @return:	bool: True if added otherwise false.
		********************/
		bool AddButton(eMenuState _option, UINT _spriteIndex, float _scale = 1.0f, UINT _col = 0, UINT _row = 0);

		/***********************
		* AddToggleButton: Adds a new button to the menu. By default it is placed after the last button added.
		* @author:	Juran Griffith.
		* @parameter: _button: The button struct to attach this toggle to.
		* @parameter: _spriteIndex: The sprite index to set as the buttons image. (make sure to load a sprite to the menu object using the add sprite function)
		* @parameter: _toggled: Sets the toggled buttons intial state.
		* @parameter: _scale: How big to make the object.
		* @return:	bool: True if added otherwise false.
		********************/
		bool AddToggleButton(TButton* _button, UINT _spriteIndex, bool _toggled = true, float _scale = 1.0f);

		/***********************
		* AddTitle: Adds a title to the menu.
		* @author:	Juran Griffith.
		* @parameter: _spriteIndex: The sprite index to set as the buttons image. (make sure to load a sprite to the menu object using the add sprite function)
		* @parameter: _scale: How big to make the object.
		* @parameter: _position: Where to place the item. (make sure to set the menu layout to custom to use this)
		* @return:	bool: True if added otherwise false.
		********************/
		bool AddTitle(UINT _spriteIndex, float _scale = 1.0f, v2float _position = v2float(0.0f, 0.0f));

		/***********************
		* GetButtonState: Gets the current button state based on the menu index provided.
		* @author:	Juran Griffith.
		* @parameter: _index: The button index to check.
		* @return:	BUTTON_STATE: The current buttons state.
		********************/
		eButtonState GetButtonState(UINT _index);

		/***********************
		* GetButton: Gets the button at the current index.
		* @author:	Juran Griffith.
		* @parameter: _index: The button index.
		* @return:	TButton: The pointer to the button struct in this menu.
		********************/
		TButton* GetButton(UINT _index);

		/***********************
		* GetCurrentMenuItem: Gets the current menu item the menu is hovering over(for mouse) or at(xbox).
		* @author:	Juran Griffith.
		* @return:	UINT	- The current menu item
		********************/
		UINT GetCurrentMenuItem();

		/***********************
		* GetMenuState: Gets the menus current state.
		* @author:	Juran Griffith.
		* @return:	MENU_STATE	- The current menu state
		********************/
		eMenuState GetMenuState();

		/***********************
		* SetPosition: Sets the menus position.
		* @author:	Juran Griffith.
		* @parameter: _position: The new position to set the menu
		* @return:	void.
		********************/
		void SetPosition(v2float _position);

		/***********************
		* SetButtonPosition: Sets the current index buttons position.
		* @author:	Juran Griffith.
		* @parameter: _index: The button index.
		* @parameter: _position: The new position for the button.
		* @return:	void.
		********************/
		void SetButtonPosition(UINT _index, v2float _position);

		/***********************
		* SetDrawBackground: Sets the draw backgorund flag for this menu.
		* @author:	Juran Griffith.
		* @parameter: _flag: True if you want to draw the background, otherwise false.
		* @return:	void.
		********************/
		void SetDrawBackground(bool _flag);

		/***********************
		* Reset: Resets all variables to default
		* @author:	Juran Griffith.
		* @return:	void
		********************/
		void FullReset();

		/***********************
		* Reset: Resets the menu states
		* @author:	Juran Griffith.
		* @return:	void
		********************/
		void Reset();

		/***********************
		* ToggleButton: Toggles the button at the given index
		* @author:	Juran Griffith.
		* @parameter: _index: The toggle button index.
		* @return:	void.
		********************/
		void ToggleButton(UINT _index);

		/***********************
		* SetController: Sets which controller has control of this menu
		* @author:	Juran Griffith.
		* @parameter: _index: Set which controller has control over the menu
		* @return:	void.
		********************/
		//void SetController(UINT _index);

		/***********************
		* OnResize: This updates the menu items position when the application has been resized.
		* @author:	Juran Griffith.
		* @return:	void.
		********************/
		void OnResize();

		/***********************
		* GetController: Gets controller 
		* @author:	Juran Griffith.
		* @parameter: 
		* @return:	void.
		********************/
		//InputGamePad* GetController() { return m_pGamepad; }

		/***********************
		* AddController: Adds a controller the menu will listen for
		* @author:	Juran Griffith.
		* @parameter: _controller: The controller to add.
		* @return:	void.
		********************/
		void AddController(InputGamePad* _controller);

	protected:
	private:

	// Variables
	public:
	protected:
	private:
		DX10_Renderer*				m_pDX10_Renderer;
		DX10_Shader_Sprite*			m_pShader_Sprite;

		float						m_space;

		// Sprite
		UINT						m_menuItem;
		std::vector<DXSprite*>		m_sprites;
		DXSprite					m_background;

		std::vector<TButton*>		m_buttons;
		std::vector<GUI_Button*>	m_toggleButtons;

		v2float						m_position;
		GUI_Button*					m_title;

		// Input
		XButtonIDs					m_XButtons;
		XStickDirectionIDs			m_XStickDirections;
		std::vector<InputGamePad*>  m_pContollers;

		// State
		eMenuState					m_state;
		eMenuLayout					m_layout;

		float						m_elaspedTime;
		UINT						m_imageIndex;

		bool*						m_pKeyDown;
		bool						m_exitPushed;	// For main menu exit button
		bool						m_drawBackground;

		// Sound
		SoundManager*				m_pSoundManager;
};

#endif