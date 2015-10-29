/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Application.h
* Description : The core of the Application is run here. The link between all components.
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

// Library Files
#pragma comment(lib, "Winmm.lib")

// Defines and Macros
#define WIN32_LEAN_AND_MEAN
#define WINDOW_CLASS_NAME L"DX10 FRAMEWORK"

#ifdef _DEBUG
// Visual Leak Detector to be run only if in DEBUG mode
#pragma comment(lib, "vld.lib")
#include <vld.h>
#define D3D_DEBUG_INFO
#endif // _DEBUG

// Local Includes
#include "Utility\Utilities.h"
#include "Utility\Timer.h"
#include "DX10\DX10.h"
#include "DX10\DX10\2D Objects\GUI_Button.h"
#include "Input\InputGamePad.h"
#include "Menus\Menu.h"
#include "Gameplay\Gameplay.h"
#include "Sound\SoundManager.h"

enum APP_STATE
{
	APP_STATE_SPLASH,
	APP_STATE_TITLE,
	APP_STATE_MAIN_MENU,
	APP_STATE_MATCH_MENU,
	APP_STATE_OPTION_MENU,
	APP_STATE_INSTRUCTIONS_MENU,
	//APP_STATE_PAUSE_MENU,
	APP_STATE_GAME
};

struct TLobbyPlayer
{
	enum LOBBY_STATE
	{
		LOBBY_STATE_NOT_CONNECTED,
		LOBBY_STATE_NOT_READY,
		LOBBY_STATE_READY
	};

	/***********************
	* TLobbyPlayer: Default constructor for a lobby player.
	* @author: Juran Griffith.
	********************/
	TLobbyPlayer()
	{
	}

	/***********************
	* TLobbyPlayer: constructor for a lobby player.
	* @parameter: _sprite: The sprite to render for the lobby player
	* @parameter: _position: The position to put the UI.
	* @parameter: _offset: The image offset when using a sprite sheet.
	* @author: Juran Griffith.
	********************/
	TLobbyPlayer(DXSprite* _sprite, D3DXVECTOR2 _position, UINT _offset = 0)
		: m_sprite(_sprite)
		, m_position(_position)
		, m_offset(_offset)
	{
		m_state = LOBBY_STATE_NOT_CONNECTED;
		UpdateImage();
		m_ready = false;
	}

	/***********************
	* SetState: Set the state the lobby player is in.
	* @parameter: _state: The state for the lobby player.
	* @author: Juran Griffith.
	* @return: void.
	********************/
	void SetState(LOBBY_STATE _state)
	{
		m_state = _state;
		m_ready = (m_state == LOBBY_STATE_READY);
	}

	/***********************
	* UpdateImage: Updates the image based on the state of the lobby player.
	* @author: Juran Griffith.
	* @return: void.
	********************/
	void UpdateImage()
	{
		switch (m_state)
		{
			case LOBBY_STATE_NOT_CONNECTED:
			{
				m_sprite->SetImageIndex(m_offset);
			}
			break;
			case LOBBY_STATE_NOT_READY:
			{
				m_sprite->SetImageIndex(m_offset + 1);
			}
			break;
			case LOBBY_STATE_READY:
			{
				m_sprite->SetImageIndex(m_offset + 2);
			}
			break;
		}
	}

	/***********************
	* Draw: Draw the lobby player UI.
	* @author: Juran Griffith.
	* @return: void.
	********************/
	void Draw()
	{
		UpdateImage();
		m_sprite->SetPosition(m_position.x, m_position.y);
		m_sprite->Render();
	}

	bool m_ready;
	LOBBY_STATE m_state;
	DXSprite* m_sprite;
	UINT m_offset;
	D3DXVECTOR2 m_position;
};

class Application
{
public:
	
	/***********************
	* WindowProc: Process the window
	* @author: Callan Moore
	* @Parameter: _hWnd: Handle to the Window sending the message
	* @Parameter: _uiMsg: The message ID being sent
	* @Parameter: _wParam: Additional detail about the message being sent
	* @Parameter: _lParam: Additional detail about the message being sent
	* @return: LRESULT: The resulting value
	********************/
	static LRESULT CALLBACK WindowProc(HWND _hWnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam);

	/***********************
	* CreateWindowApp: Creates the window for the Application
	* @author: Callan Moore
	* @Parameter: _clientWidth: Width of the screen
	* @Parameter: _clientHeight: Height of the screen
	* @Parameter: _hInstance: The instance of the application
	* @return: bool: Successful or not
	********************/
	bool CreateWindowApp(int _clientWidth, int _clientHeight, HINSTANCE _hInstance);

	/***********************
	* Execute: Runs the main loop of the Application
	* @author: Callan Moore
	* @return: int: Integer containing details about the last message resulting it termination
	********************/
	int Execute();

	/***********************
	* GetInstance: Returns the singleton instance of the Application, if it doesn't exist creates it.
	* @author: Callan Moore
	* @return: CApplication*: The current instance of the Application
	********************/
	static Application* GetInstance();
	
	/***********************
	* Initialise: Initialise the Application
	* @author: Callan Moore
	* @parameter: _clientWidth: Width of the client window
	* @parameter: _clientHeight: Height of the client window
	* @Parameter: _hInstance: Handle to the current instance
	* @return: bool: Successful or not
	********************/
	bool Initialise(int _clientWidth, int _clientHeight, HINSTANCE _hInstance);

	/***********************
	* Initialise_DX10: Initialise the DX10 for the Application
	* @author: Callan Moore
	* @Parameter: _hInstance: Handle to the current instance
	* @return: bool: Successful or not
	********************/
	bool Initialise_DX10(HINSTANCE _hInstance);

	/***********************
	* ~CApplication: Default Destructor for Application class
	* @author: Callan Moore
	********************/
	~Application();

	/***********************
	* DestroyInstance: Deletes the instance of the Application.
	* @author: Callan Moore
	* @return: void
	********************/
	static void DestroyInstance();

	/***********************
	* ShutDown: Shuts down and releases all memory created by Application
	* @author: Callan Moore
	* @return: void
	********************/
	void ShutDown();

	/***********************
	* ExecuteOneFrame: Executes one frame of the Application
	* @author: Callan Moore
	* @return: void
	********************/
	void ExecuteOneFrame();

	/***********************
	* Process: Process the Application
	* @author: Callan Moore
	* @parameter: _dt: The current delta tick
	* @return: bool: Successful or not
	********************/
	bool Process(float _dt);

	/***********************
	* ProcessShaders: Process all the Shaders to set up all once per frame variables
	* @author: Callan Moore
	* @return: void
	********************/
	void ProcessShaders();

	/***********************
	* Render: Render the Application components
	* @author: Callan Moore
	* @return: void
	********************/
	void Render();

	/***********************
	* HandleInput: Handle all input for the application
	* @author: Callan Moore
	* @return: Successful or not
	********************/
	bool HandleInput();

	/***********************
	* SetKeyDown: Sets the Key down boolean for input Key
	* @author: Callan Moore
	* @parameter: _index: Index of the Key to set
	* @parameter: _down: True/false setting
	* @return: void
	********************/
	void SetKeyDown(int _index, bool _down) { m_pKeyDown[_index] = _down; };

	/***********************
	* SetMouseDown: Set the state of the mouse button being clicked
	* @author: Callan Moore
	* @parameter: _mouseDown: The new state of the mouse button
	* @return: void.
	********************/
	void SetMouseDown(bool _mouseDown) { m_mouseDown = _mouseDown; };

private:
	// Preventing copies and extra constructions
	Application() {}
	Application(const Application& _kr);
	Application& operator= (const Application& _kr);

	/*******************
	* ExitApp: Exits the application.
	* @author:	Juran Griffith.
	* @return:	void.
	********************/
	void ExitApp();

	/*******************
	* UpdateState: Updates the application based on the menu item selected
	* @author:	Juran Griffith.
	* @return:	bool: Successful or not
	********************/
	bool UpdateState(MENU_STATE _state);

	// TO DO CAL
	void UpdateClientSize();

	/*******************
	* ResetsControllerUI: Resets the controller ui to the correct position.
	* @author:	Juran Griffith.
	* @return:	void
	********************/
	void ResetControllerUI();

	/*******************
	* GetNumberOfConnectedControllers: Gets the number of connected controllers
	* @author:	Juran Griffith.
	* @return:	short: The number of connected controllers.
	********************/
	short GetNumberOfConnectedControllers();

private:
	// Singleton Instance
	static Application* s_pApp;

	// Window Variables
	HWND m_hWnd;
	int m_clientWidth;
	int m_clientHeight;
	bool m_online;
	
	// Timer Variables
	Timer* m_pTimer;
	float m_deltaTick;
	int m_fps;
	float m_fpsTimer; 

	// Input Variables
	bool* m_pKeyDown;
	bool m_mouseDown;

	// Renderer Variables
	DX10_Renderer* m_pDX10_Renderer;

	// Camera
	DX10_Camera_Debug* m_pCamera;

	// Game Pad Input
	XButtonIDs m_XButtons;	
	XStickDirectionIDs m_XStickDirections;
	std::vector<InputGamePad*> m_pContollers;

	// Game play Objects
	Game* m_pGame;

	// App State
	APP_STATE m_state;
	bool m_isFullscreen;
	bool m_isSoundOn;
	bool m_isRumbleOn;

	// Sound
	SoundManager* m_pSoundManager;

	// 2D Objects
	DX10_Shader_Sprite*	m_pShader_Sprite;
	//std::vector<Menu*> m_menus;
	Menu* m_menuMain;
	Menu* m_menuOptions;

	DXSprite m_splash_ps;
	DXSprite m_splash_orb;
	float m_animationTime;
	float m_animationSpeed;
	float m_waitTime;
	bool m_wait;

	DXSprite m_uiInstructions;
	DXSprite m_uiControllerMissing;
	DXSprite m_uiPressStart;
	DXSprite* m_uiLobby;

	std::vector<TLobbyPlayer> m_lobbyPlayers;

	UINT m_playersReady;
};

#endif // __APPLICATION_H__

