/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Application.cpp
* Description : The core of the Application is run here. The link between all components.
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "Application.h"

// Static Variables
Application* Application::s_pApp = 0;

// Global Variables
FILE* g_file;

int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _cmdShow)
{
	// Seed the random based on the time
	srand((UINT)time(NULL));

	if (AllocConsole())
	{
		freopen_s(&g_file, "conout$", "w", stdout);
	}

	// Set the client width and height
	int clientWidth = 1000;
	int clientHeight = 1000;

	// Create the Application 
	Application* pApp = Application::GetInstance();
	
	if (pApp->CreateWindowApp(clientWidth, clientHeight, _hInstance) == true)
	{
		if (pApp->Initialise(clientWidth, clientHeight, _hInstance) == false)
		{
			pApp->DestroyInstance();
			return 0;
		}
		pApp->Execute();
	}
	
	// Delete all contents of the Application
	pApp->DestroyInstance();
	return 0;
}

LRESULT CALLBACK Application::WindowProc(HWND _hWnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam)
{
	// Retrieve the Instance of the Application
	Application* pApp = Application::GetInstance();

	// Switch case dependent on the message sent
	switch (_uiMsg)
	{
		case WM_DESTROY:	// Fall Through
		case WM_CLOSE:
		{
			// Kill the application, this sends a WM_QUIT message.
			PostQuitMessage(0);	
		}
		break;
		case WM_KEYDOWN:
		{
			// Only accepts the input once per key press
			if (((_lParam >> 30) & 1) != 1)
			{
				pApp->SetKeyDown(_wParam, true);
			}
		}
		break;
		case WM_KEYUP:
		{
			pApp->SetKeyDown(_wParam, false);
		}
		break;
		case WM_LBUTTONDOWN:
		{
			pApp->SetMouseDown(true);
		}
		break;
		case WM_LBUTTONUP:
		{
			pApp->SetMouseDown(false);
			
		}
		break;
		default: break;
	} // End switch.

	// Process any messages left to process
	return (DefWindowProc(_hWnd, _uiMsg, _wParam, _lParam));
}

bool Application::CreateWindowApp(int _clientWidth, int _clientHeight, HINSTANCE _hInstance)
{
	WNDCLASSEX winClass;	// This will hold the class we create.

	// Fills in the window class structure.
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc = WindowProc;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	winClass.hInstance = _hInstance;
	winClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)WHITE_BRUSH;
	winClass.lpszMenuName = NULL;
	winClass.lpszClassName = WINDOW_CLASS_NAME;
	winClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// Registers the window class
	VALIDATE(RegisterClassEx(&winClass));

	m_hWnd = CreateWindowEx(NULL,								// Extended style.
		WINDOW_CLASS_NAME,					// Class.
		L"DX10 Framework",					// Title.
		WS_VISIBLE | WS_CAPTION | WS_BORDER | WS_SYSMENU,// Windowed Mode
		0, 0,								// Initial x,y position for the top left corner of the window
		_clientWidth, _clientHeight,		// Initial width, height of the window
		NULL,								// Handle to parent.
		NULL,								// Handle to menu.
		_hInstance,							// Instance of this application.
		NULL);								// Extra creation parameters.

	// Check the window was created successfully.
	VALIDATE(m_hWnd);

	return true;	
}

int Application::Execute()
{
	MSG uiMsg;				// Generic message.

	// Enter main event loop.
	while (m_online)
	{
		while (PeekMessage(&uiMsg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&uiMsg);
			DispatchMessage(&uiMsg);
		}

		if (uiMsg.message == WM_QUIT)
		{
			break;
		}

		ExecuteOneFrame();
	}

	// Return to Windows
	return (static_cast<int>(uiMsg.wParam));
}

Application* Application::GetInstance()
{
	if (s_pApp == 0)
	{
		// If the application does not exist, create the application
		s_pApp = new Application();
	}
	// Always return the current instance of the application
	return s_pApp;
}

bool Application::Initialise(int _clientWidth, int _clientHeight, HINSTANCE _hInstance)
{
	m_state = APP_STATE_SPLASH;

	m_isFullscreen = true;
	m_isSoundOn = true;
	m_isRumbleOn = true;
	m_playersReady = 0;

	// Save the client window sizes
	m_clientWidth = _clientWidth;
	m_clientHeight = _clientHeight;

	// Initialise boolean array for KeyDown to false
	m_pKeyDown = new bool[255];
	memset(m_pKeyDown, false, 255);

	// Create the controllers
	for (int i = 0; i < 4; i++)
	{
		m_pContollers.push_back(new InputGamePad());
		VALIDATE(m_pContollers[i]->Initialise((i + 1), m_isRumbleOn));
	}
	
	VALIDATE(Initialise_DX10(_hInstance));

	m_online = true;

	// Initialise all time keeping variables to default (zero) state
	m_pTimer = new Timer();
	m_pTimer->Reset();
	m_fps = 0;
	m_deltaTick = 0;
	m_fpsTimer = 0;

	// Animation variables
	m_animationTime = 0.0f;
	m_animationSpeed = 1.0f / 15.0f; // 15 FPS
	m_waitTime = 1.5f; // seconds 
	m_wait = false;
	
	return true;
}

bool Application::Initialise_DX10(HINSTANCE _hInstance)
{
	// Initialise the Renderer
	m_pDX10_Renderer = new DX10_Renderer();
	VALIDATE(m_pDX10_Renderer->Initialise(m_clientWidth, m_clientHeight, m_hWnd));
	if (m_pDX10_Renderer->IsFullscreen() != m_isFullscreen)
	{
		m_pDX10_Renderer->ToggleFullscreen();
	}

	// Initialise the Objects
	m_pCamera = new DX10_Camera_Debug();
	VALIDATE(m_pCamera->Initialise(m_pDX10_Renderer));
	m_pCamera->SetPostionVec({ 0, 0, -100.0f });
	m_pCamera->SetTargetVec({ 0, 0, 0 });
	m_pCamera->SetUpVec({ 0, 1, 0 });
	m_pCamera->Process();

	// Initialise the sprite shader
	m_pShader_Sprite = new DX10_Shader_Sprite();
	VALIDATE(m_pShader_Sprite->Initialise(m_pDX10_Renderer, &m_hWnd));

	// Initialize our sound system
	m_pSoundManager = new SoundManager();
	VALIDATE(m_pSoundManager->Intialise());

	// Initialise Main Menu
	m_menuMain = new Menu();
	VALIDATE(m_menuMain->Initialise(m_pDX10_Renderer, m_pShader_Sprite, m_pSoundManager, m_pKeyDown));

	m_menuMain->AddController(m_pContollers[0]);
	m_menuMain->AddController(m_pContollers[1]);
	m_menuMain->AddController(m_pContollers[2]);
	m_menuMain->AddController(m_pContollers[3]);

	m_menuMain->SetDrawBackground(false);

	m_menuMain->AddSprite("Tron/UI/tron_orbliteration_title.png", 1600, 800);
	m_menuMain->AddSprite("Tron/Button/tron_button_start_fill.png", 481, 424, 1, 4);
	m_menuMain->AddSprite("Tron/Button/tron_button_instructions_fill.png", 1137, 424, 1, 4);
	m_menuMain->AddSprite("Tron/Button/tron_button_options_fill.png", 669, 424, 1, 4);
	m_menuMain->AddSprite("Tron/Button/tron_button_exit_fill.png", 387, 424, 1, 4);
	m_menuMain->AddTitle(0, 0.5f);
	m_menuMain->AddButton(MENU_STATE_START, 1, 0.5f);
	m_menuMain->AddButton(MENU_STATE_INSTRUCTIONS, 2, 0.5f);
	m_menuMain->AddButton(MENU_STATE_OPTIONS, 3, 0.5f);
	m_menuMain->AddButton(MENU_STATE_EXIT, 4, 0.5f);

	// Initialise Options Menu
	m_menuOptions = new Menu();

	VALIDATE(m_menuOptions->Initialise(m_pDX10_Renderer, m_pShader_Sprite, m_pSoundManager, m_pKeyDown));

	m_menuOptions->AddController(m_pContollers[0]);
	m_menuOptions->AddController(m_pContollers[1]);
	m_menuOptions->AddController(m_pContollers[2]);
	m_menuOptions->AddController(m_pContollers[3]);

	m_menuOptions->SetDrawBackground(false);

	m_menuOptions->AddSprite("Tron/Button/toggle_button.png", 95, 61, 1, 2);
	m_menuOptions->AddSprite("Tron/Button/tron_button_fullscreen_fill.png", 945, 424, 1, 4);
	m_menuOptions->AddSprite("Tron/Button/tron_button_sound_fill.png", 481, 424, 1, 4);
	m_menuOptions->AddSprite("Tron/Button/tron_button_rumble_fill.png", 575, 424, 1, 4);
	m_menuOptions->AddButton(MENU_STATE_FULL_SCREEN, 1, 0.5f);
	m_menuOptions->AddToggleButton(m_menuOptions->GetButton(0), 0, m_isFullscreen);
	m_menuOptions->AddButton(MENU_STATE_SOUND, 2, 0.5f);
	m_menuOptions->AddToggleButton(m_menuOptions->GetButton(1), 0, m_isSoundOn);
	m_menuOptions->AddButton(MENU_STATE_RUMBLE, 3, 0.5f);
	m_menuOptions->AddToggleButton(m_menuOptions->GetButton(2), 0, m_isRumbleOn);

	float min = static_cast<float>(min(m_pDX10_Renderer->GetWidth(), m_pDX10_Renderer->GetHeight()));
	float max = static_cast<float>(max(m_pDX10_Renderer->GetWidth(), m_pDX10_Renderer->GetHeight()));
	float xoffset = static_cast<float>(m_pDX10_Renderer->GetWidth()) * 0.5f;
	float yoffset = static_cast<float>(m_pDX10_Renderer->GetHeight()) * 0.5f;

	VALIDATE(m_splash_ps.Initialise(m_pDX10_Renderer, m_pShader_Sprite, "Phenomena/phenomena_splash.png", 3504, 2173, 6, 5));
	m_splash_ps.SetSize(max - 300.0f, (max - 300.0f) * (434.6f / 584.0f));
	m_splash_ps.SetPosition(0.0f, 0.0f);
	m_splash_ps.SetImageIndex(0);

	VALIDATE(m_splash_orb.Initialise(m_pDX10_Renderer, m_pShader_Sprite, "Tron/UI/tron_orbliteration_splash.png", 4000, 1200, 5, 3));
	m_splash_orb.SetPosition(0, 0);
	m_splash_orb.SetSize(max, max * 0.5f);
	m_splash_orb.SetImageIndex(0);

	VALIDATE(m_uiInstructions.Initialise(m_pDX10_Renderer, m_pShader_Sprite, "Tron/UI/tron_orbliteration_instructions.png", 3000, 3000));
	m_uiInstructions.SetSize(min, min);
	m_uiInstructions.SetPosition(xoffset - m_uiInstructions.GetWidth() * 0.5f, 0.0f);

	m_uiControllerMissing.Initialise(m_pDX10_Renderer, m_pShader_Sprite, "Tron/Controller/controller_connect_an_xbox_controller.png", 625, 96);
	m_uiControllerMissing.SetScale(0.5f);
	m_uiControllerMissing.SetPosition(xoffset - m_uiControllerMissing.GetWidth() * 0.5f, static_cast<float>(m_pDX10_Renderer->GetHeight()) - m_uiControllerMissing.GetHeight());

	VALIDATE(m_uiPressStart.Initialise(m_pDX10_Renderer, m_pShader_Sprite, "Tron/Controller/controller_press_start.png", 461, 96));
	m_uiPressStart.SetPosition(xoffset - m_uiPressStart.GetWidth() * 0.5f, yoffset - m_uiPressStart.GetHeight() * 0.5f);

	VALIDATE(m_uiPressB.Initialise(m_pDX10_Renderer, m_pShader_Sprite, "Tron/Controller/controller_press_b_goback.png", 431, 96));
	m_uiPressB.SetPosition(xoffset - m_uiPressB.GetWidth() * 0.5f, m_pDX10_Renderer->GetHeight() - m_uiPressB.GetHeight() - m_uiControllerMissing.GetHeight());
	
	m_uiLobby = new DXSprite();
	VALIDATE(m_uiLobby->Initialise(m_pDX10_Renderer, m_pShader_Sprite, "Tron/Controller/controller_lobby_allplayers_ss.png", 840, 1300, 3, 4));

	m_uiPlayer = new DXSprite();
	VALIDATE(m_uiPlayer->Initialise(m_pDX10_Renderer, m_pShader_Sprite, "Tron/UI/tron_ui_players_ss.png", 2684, 365, 4));
	m_uiPlayer->SetScale(0.5f);

	float m_uiSpace = 10.0f;
	float width = static_cast<float>(m_pDX10_Renderer->GetWidth());
	float height = static_cast<float>(m_pDX10_Renderer->GetHeight());

	// ============== Lobby UI ============== //

	// == Player 1

	// Set the sprite positions
	m_uiPlayer->SetPosition(m_uiSpace, m_uiSpace);
	m_uiLobby->SetPosition(m_uiSpace + m_uiPlayer->GetWidth(), m_uiSpace);

	// Create lobby object and push to list
	m_lobbyPlayers.push_back(TLobbyPlayer(m_uiLobby, m_uiPlayer));

	// == Player 2

	// Set the sprite positions
	m_uiPlayer->SetPosition(width - m_uiPlayer->GetWidth() - m_uiSpace, height - m_uiPlayer->GetHeight() - m_uiSpace);
	m_uiLobby->SetPosition(width - m_uiPlayer->GetWidth() - m_uiSpace - m_uiLobby->GetWidth(), height - m_uiLobby->GetHeight() - m_uiSpace);

	// Create lobby object and push to list
	m_lobbyPlayers.push_back(TLobbyPlayer(m_uiLobby, m_uiPlayer, 3, 1));
	
	// == Player 3
	
	// Set the sprite positions
	m_uiPlayer->SetPosition(width - m_uiPlayer->GetWidth() - m_uiSpace, m_uiSpace);
	m_uiLobby->SetPosition(width - m_uiPlayer->GetWidth() - m_uiSpace - m_uiLobby->GetWidth(), m_uiSpace);

	// Create lobby object and push to list
	m_lobbyPlayers.push_back(TLobbyPlayer(m_uiLobby, m_uiPlayer, 6, 2));

	// == Player 4

	// Set the sprite positions
	m_uiPlayer->SetPosition(m_uiSpace, height - m_uiPlayer->GetHeight() - m_uiSpace);
	m_uiLobby->SetPosition(m_uiSpace + m_uiPlayer->GetWidth(), height - m_uiLobby->GetHeight() - m_uiSpace);

	// Create lobby object and push to list
	m_lobbyPlayers.push_back(TLobbyPlayer(m_uiLobby, m_uiPlayer, 9, 3));
	
	// Play FX
	if (m_state == APP_STATE_SPLASH)
	{
		m_pSoundManager->PlayPhenomenaSplash();
	}

	return true;
}

Application::~Application()
{
}

void Application::DestroyInstance()
{
	s_pApp->ShutDown();
	ReleasePtr(s_pApp);
}

void Application::ShutDown()
{
	// Delete and free memory for the Application variables
	ReleasePtr(m_pKeyDown);
	ReleasePtr(m_pTimer);

	ReleasePtr(m_pSoundManager);

	// Delete and free the memory from the Renderer
	if (m_pDX10_Renderer != 0)
	{ 
		// DX10 pointers to release
		ReleasePtr(m_pCamera);
		
		// Gamepad input memory release
		while (!m_pContollers.empty())
		{
			ReleasePtr(m_pContollers.back());
			m_pContollers.pop_back();
		}

		// Menu memory release
		ReleasePtr(m_uiLobby);
		ReleasePtr(m_uiPlayer);
		ReleasePtr(m_menuMain);
		ReleasePtr(m_menuOptions);

		ReleasePtr(m_pShader_Sprite);

		// Game play objects memory release
		ReleasePtr(m_pGame);

		// Release the renderers resources
		m_pDX10_Renderer->ShutDown();
		ReleasePtr(m_pDX10_Renderer);
	}	
}

void Application::ExecuteOneFrame()
{
	// Retrieve the Delta Tick of the last frame
	m_pTimer->Tick();
	float dt = m_pTimer->GetDeltaTime();
	m_deltaTick += dt;
	m_fpsTimer += dt;
		
	// Limit to 60 FPS for Renderering
	if (m_deltaTick > (1.0 / 60.0f))
	{

		if (Process(m_deltaTick) == false)
		{
			// A process failed to create something
			m_online = false;
			return;
		}

		Render();
		m_deltaTick = 0;
		m_fps++;
		
	}	

	// Reset FPS counters
	if (m_fpsTimer >= 1.0f)
	{
		COORD pos = { 0, 0 };
		HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(output, pos);
		std::cout << "Frames : " << m_fps << std::endl;
		m_fpsTimer -= 1.0f;
		m_fps = 0;
	}


}

bool Application::Process(float _dt)
{
	VALIDATE(HandleInput());

	m_pSoundManager->Update();

	// Processes to run when using DX10 Renderer
	if (m_pDX10_Renderer != 0)
	{		
		m_pShader_Sprite->Update(_dt);
		
		switch (m_state)
		{
		case APP_STATE_SPLASH:
			{
				m_animationTime += _dt;
				if (!m_splash_ps.IsAtLastFrame())
				{
					if (m_animationTime > m_animationSpeed)
					{
						m_splash_ps.IncrementIndex();
						m_animationTime = 0.0f;
					}
				}
				else if (m_waitTime < m_animationTime)
				{
					m_state = APP_STATE_TITLE;
					m_animationTime = 0.0f;
				}
			}
			break;
		case APP_STATE_TITLE:
			{
				m_animationTime += _dt;
				if (!m_splash_orb.IsAtLastFrame())
				{
					if (m_animationTime > m_animationSpeed)
					{
						m_splash_orb.IncrementIndex();
						m_animationTime = 0.0f;

						if (m_splash_orb.IsAtLastFrame())
						{
							m_wait = true;
						}
					}
				}
				else if (m_waitTime < m_animationTime)
				{
					m_state = APP_STATE_MAIN_MENU;

					// Play Menu Music
					m_pSoundManager->PlaySong(0);

					m_animationTime = 0.0f;
				}
			}
			break;
		case APP_STATE_MAIN_MENU:
			{
				// Play music
				m_pSoundManager->PlaySong(0);

				m_menuMain->Process(_dt);
				VALIDATE(UpdateState(m_menuMain->GetMenuState()));
			}
			break;
		case APP_STATE_MATCH_MENU:
			{
				// Play music
				//m_pSoundManager->PlaySong(0);

				if (m_pKeyDown[VK_BACK])
				{
					m_pKeyDown[VK_BACK] = false;
					m_state = APP_STATE_MAIN_MENU;
					m_menuMain->Reset();
					break;
				}
				
				bool isStart = false;
				m_playersReady = 0;

				for (UINT i = 0; i < m_pContollers.size(); ++i)
				{
					m_pContollers[i]->PreProcess();
					if (m_pContollers[i]->Connected())
					{
						if (m_pContollers[i]->GetButtonDown(m_XButtons.ActionButton_B))
						{
							if (m_lobbyPlayers[i].m_state == m_lobbyPlayers[i].LOBBY_STATE_NOT_READY)
							{
								m_state = APP_STATE_MAIN_MENU;
								m_menuMain->Reset();
								m_pContollers[i]->PostProcess(_dt);
								// Play FX
								m_pSoundManager->PlayMenuBack();
								break;
							}
							else if (m_lobbyPlayers[i].m_state == m_lobbyPlayers[i].LOBBY_STATE_READY)
							{
								m_lobbyPlayers[i].SetState(m_lobbyPlayers[i].LOBBY_STATE_NOT_READY);
								// Play FX
								m_pSoundManager->PlayMenuBack();
							}
						}
						else if (m_pContollers[i]->GetButtonDown(m_XButtons.ActionButton_A))
						{
							m_lobbyPlayers[i].SetState(m_lobbyPlayers[i].LOBBY_STATE_READY);
							// Play FX
							m_pSoundManager->PlayMenuNavigate();
						}
						
						if (m_lobbyPlayers[i].m_ready)
						{
							m_playersReady++;
							if (m_pContollers[i]->GetButtonDown(m_XButtons.Start))
							{
								isStart = true;
							}
						}
						else
						{
							m_lobbyPlayers[i].SetState(m_lobbyPlayers[i].LOBBY_STATE_NOT_READY);
						}
					}
					else
					{
						m_lobbyPlayers[i].SetState(m_lobbyPlayers[i].LOBBY_STATE_NOT_CONNECTED);
					}
					m_pContollers[i]->PostProcess(_dt);
				}

				if (m_playersReady >= 2 && isStart)
				{
					m_pGame = new Game();

					// Attach the correct gamepads
					for (UINT i = 0; i < m_pContollers.size(); ++i)
					{
						if (m_pContollers[i]->Connected() && m_lobbyPlayers[i].m_ready)
							m_pGame->AttachGamepad(m_pContollers[i]);
					}

					VALIDATE(m_pGame->Initialise(m_pDX10_Renderer, m_pSoundManager, m_pShader_Sprite, m_isRumbleOn, m_pKeyDown));
					VALIDATE(m_pGame->AttachUI(&m_uiInstructions, &m_uiControllerMissing));
					m_state = APP_STATE_GAME;
					m_playersReady = 0;
					
					// Play FX
					m_pSoundManager->PlayMenuAccept();
				}
			}
			break;
		case APP_STATE_INSTRUCTIONS_MENU:
			{
				// Play music
				//m_pSoundManager->PlaySong(0);

				if (m_pKeyDown[VK_BACK])
				{
					m_pKeyDown[VK_BACK] = false;
					m_state = APP_STATE_MAIN_MENU;
					m_menuMain->Reset();
					break;
				}

				for (UINT i = 0; i < m_pContollers.size(); ++i)
				{
					m_pContollers[i]->PreProcess();
					if (m_pContollers[i]->Connected())
					{
						if (m_pContollers[i]->GetButtonDown(m_XButtons.ActionButton_B))
						{
							m_state = APP_STATE_MAIN_MENU;
							m_menuMain->Reset();
							break;
						}
					}
					m_pContollers[i]->PostProcess(_dt);
				}
			}
			break;
		case APP_STATE_OPTION_MENU:
			{
				// Play music
				//m_pSoundManager->PlaySong(0);

				m_menuOptions->Process(_dt);
				VALIDATE(UpdateState(m_menuOptions->GetMenuState()));
			}
			break;
		/*case APP_STATE_PAUSE_MENU:
			{
				//m_menus[3]->Process(_dt);
				//VALIDATE(UpdateState(m_menus[3]->GetMenuState()));
			}
			break;*/
		case APP_STATE_GAME:
			{
				// Play music
				//m_pSoundManager->PlaySong(0);
				if (m_pGame->Process(_dt) == false)
				{
					// If the game has ended
					ReleasePtr(m_pGame);
					m_state = APP_STATE_MAIN_MENU;
					m_menuMain->Reset();
					ResetControllerUI();
				}
			}
			break;
		default:
			break;
		}
	}

	return true;
}

void Application::ProcessShaders()
{
	
}

void Application::Render()
{
	// Render calls when using the DX10 Renderer
	if (m_pDX10_Renderer != 0)
	{
		// Get the Renderer Ready to receive new data
		m_pDX10_Renderer->StartRender();

		if (m_state == APP_STATE_GAME)
		{
			m_pGame->Render();
		}
		else
		{
			m_pDX10_Renderer->TurnZBufferOff();
			switch (m_state)
			{
			case APP_STATE_SPLASH:
				{
					m_splash_ps.Render();
				}
				break;
			case APP_STATE_TITLE:
				{
					m_splash_orb.Render();
				}
				break;
			case APP_STATE_MAIN_MENU:
				{
					m_menuMain->Draw();
				}
				break;
			case APP_STATE_MATCH_MENU:
				{
					for (UINT i = 0; i < m_lobbyPlayers.size(); ++i)
					{
						m_lobbyPlayers[i].Draw();
					}
					if (m_playersReady >= 2)
					{
						m_uiPressStart.Render();
					}
					m_uiPressB.Render();
				}
				break;
			case APP_STATE_INSTRUCTIONS_MENU:
				{
					m_uiInstructions.Render();

					m_uiPressB.Render();
				}
				break;
			case APP_STATE_OPTION_MENU:
				{
					m_menuOptions->Draw();

					m_uiPressB.Render();
				}
				break;
			/*case APP_STATE_PAUSE_MENU:
				{
					//m_menus[3]->Draw();
				}
				break;*/
			/*case APP_STATE_GAME:
			{

			}
			break;*/
			default:
				break;
			}

			if (GetNumberOfConnectedControllers() == 0 && (m_state != APP_STATE_MATCH_MENU && m_state != APP_STATE_GAME))
			{
				m_uiControllerMissing.Render();
			}

			m_pDX10_Renderer->TurnZBufferOn();
		}

		// Tell the Renderer the data input is over and present the outcome
		m_pDX10_Renderer->EndRender();	
	}
}

bool Application::HandleInput()
{
	if (m_pKeyDown[VK_ESCAPE])
	{
		ExitApp();
	}

	// Template Inputs
	if (m_pKeyDown[VK_F1])
	{
		m_pDX10_Renderer->ToggleFullscreen();

		SetKeyDown(VK_F1, false);
	}

	if (m_pKeyDown[VK_F2])
	{
		m_pDX10_Renderer->ToggleFillMode();

		SetKeyDown(VK_F2, false);
	}

	if ((m_pKeyDown[VK_NUMPAD4]) && !(m_pKeyDown[VK_NUMPAD6]))
	{
		m_pCamera->Strafe(-1 * m_deltaTick);
	}

	if ((m_pKeyDown[VK_NUMPAD6]) && !(m_pKeyDown[VK_NUMPAD4]))
	{
		m_pCamera->Strafe(m_deltaTick);
	}

	if ((m_pKeyDown[VK_NUMPAD8]) && !(m_pKeyDown[VK_NUMPAD2]))
	{
		m_pCamera->Move(m_deltaTick);
	}

	if ((m_pKeyDown[VK_NUMPAD2]) && !(m_pKeyDown[VK_NUMPAD8]))
	{
		m_pCamera->Move(-1 * m_deltaTick);
	}

	if ((m_pKeyDown[VK_NUMPAD1]) && !(m_pKeyDown[VK_NUMPAD0]))
	{
		m_pCamera->Fly(m_deltaTick);
	}

	if ((m_pKeyDown[VK_NUMPAD0]) && !(m_pKeyDown[VK_NUMPAD1]))
	{
		m_pCamera->Fly(-1 * m_deltaTick);
	}

	if ((m_pKeyDown[VK_UP]) && !(m_pKeyDown[VK_DOWN]))
	{
		m_pCamera->Pitch(-1 * m_deltaTick);
	}

	if ((m_pKeyDown[VK_DOWN]) && !(m_pKeyDown[VK_UP]))
	{
		m_pCamera->Pitch(m_deltaTick);
	}

	if ((m_pKeyDown[VK_LEFT]) && !(m_pKeyDown[VK_RIGHT]))
	{
		m_pCamera->Yaw(-1 * m_deltaTick);
	}

	if ((m_pKeyDown[VK_RIGHT]) && !(m_pKeyDown[VK_LEFT]))
	{
		m_pCamera->Yaw(m_deltaTick);
	}

	return true;
}

void Application::ExitApp()
{
	if (m_pDX10_Renderer != 0)
	{
		if (m_pDX10_Renderer->GetFullScreenState() == true)
		{
			// Toggle the screen mode to windowed before exiting application
			m_pDX10_Renderer->ToggleFullscreen();
		}
	}
	m_online = false;	// Changing this to false will cause the main application loop to end -> quitting the application
}

bool Application::UpdateState(eMenuState _state)
{
	switch (_state)
	{
		// Main menu
	case MENU_STATE_START:
		{
			m_state = APP_STATE_MATCH_MENU;
			for (UINT i = 0; i < m_lobbyPlayers.size(); ++i)
			{
				m_lobbyPlayers[i].SetState(m_lobbyPlayers[i].LOBBY_STATE_NOT_READY);
			}
		}
		break;
	case MENU_STATE_INSTRUCTIONS:
		{
			m_state = APP_STATE_INSTRUCTIONS_MENU;
			m_menuMain->Reset();
		}
		break;
	case MENU_STATE_OPTIONS:
		{
			m_state = APP_STATE_OPTION_MENU;
			m_menuOptions->Reset();
		}
		break;
	case MENU_STATE_EXIT:
		{
			ExitApp();
		}
		break;
	case MENU_STATE_BACK:
		{
			switch (m_state)
			{
			case APP_STATE_MATCH_MENU:			// Fall through
			case APP_STATE_INSTRUCTIONS_MENU:	// Fall through
			case APP_STATE_OPTION_MENU:			// Fall through
				{
					if (m_state == APP_STATE_MATCH_MENU)
					{
						ResetControllerUI();
					}
					m_state = APP_STATE_MAIN_MENU;
					m_menuMain->Reset();
				}
				break;
			/*case APP_STATE_PAUSE_MENU:
				{
					m_state = APP_STATE_GAME;
				}
				break;*/
			default:
				{
					m_menuMain->Reset();
				}
				break;
			}
		}
		break;
		// Options menu
	case MENU_STATE_FULL_SCREEN:
		{
			// Set the menu state
			m_state = APP_STATE_OPTION_MENU;

			// Toggle
			m_isFullscreen = !m_isFullscreen;
			m_pDX10_Renderer->ToggleFullscreen();
			UpdateClientSize();

			// Update button
			m_menuOptions->ToggleButton(0);
			m_menuOptions->Reset();
		}
		break;
	case MENU_STATE_SOUND:
		{
			m_state = APP_STATE_OPTION_MENU;
			
			// Toggle
			m_isSoundOn = !m_isSoundOn;
			m_pSoundManager->Mute(!m_isSoundOn);

			// Update button
			m_menuOptions->ToggleButton(1);
			m_menuOptions->Reset();
		}
		break;
	case MENU_STATE_RUMBLE:
		{
			// Set the menu state
			m_state = APP_STATE_OPTION_MENU;
			
			// Toggle
			m_isRumbleOn = !m_isRumbleOn;

			if (m_isRumbleOn)
			{
				for (UINT i = 0; i < m_pContollers.size(); i++)
				{
					m_pContollers[i]->Vibrate(1.0f, 1.0f);
				}
			}
			// Update button
			m_menuOptions->ToggleButton(2);
			m_menuOptions->Reset();
		}
		break;
	/*case MENU_STATE_RESUME: // Pause menu states (reuses the main menu states)
		{
			//m_state = APP_STATE_GAME;
		}
		break;*/

	default:
		{

		}
		break;
	}

	return true;
}

void Application::UpdateClientSize()
{
	float width = static_cast<float>(m_pDX10_Renderer->GetWidth());
	float height = static_cast<float>(m_pDX10_Renderer->GetHeight());

	if (m_pGame != 0)
	{
		m_pGame->UpdateClientSize();
	}

	float diff = max(width, height) - min(width, height);

	if (max(width, height) == width)
	{
		m_uiInstructions.SetPosition((diff / 2.0f), 0.0f);
	}
	else
	{
		m_uiInstructions.SetPosition(0.0f, (diff / 2.0f));
	}

	m_uiInstructions.SetSize(min(width, height), min(width, height));

	float m_uiSpace = 10.0f;

	m_uiLobby->SetPosition(m_uiPlayer->GetPosition().x + m_uiPlayer->GetWidth(), m_uiPlayer->GetPosition().y);

	if (m_isFullscreen)
	{
		m_lobbyPlayers[0].m_positionPlayer = v2float(m_uiSpace, m_uiSpace);
		m_lobbyPlayers[0].m_positionLobby = v2float(m_uiSpace + m_uiPlayer->GetWidth(), m_uiSpace);

		m_lobbyPlayers[1].m_positionPlayer = v2float(width - m_uiPlayer->GetWidth() - m_uiSpace, height - m_uiPlayer->GetHeight() - m_uiSpace);
		m_lobbyPlayers[1].m_positionLobby = v2float(width - m_uiPlayer->GetWidth() - m_uiSpace - m_uiLobby->GetWidth(), height - m_uiLobby->GetHeight() - m_uiSpace);

		m_lobbyPlayers[2].m_positionPlayer = v2float(width - m_uiPlayer->GetWidth() - m_uiSpace, m_uiSpace);
		m_lobbyPlayers[2].m_positionLobby = v2float(width - m_uiPlayer->GetWidth() - m_uiSpace - m_uiLobby->GetWidth(), m_uiSpace);

		m_lobbyPlayers[3].m_positionPlayer = v2float(m_uiSpace, height - m_uiPlayer->GetHeight() - m_uiSpace);
		m_lobbyPlayers[3].m_positionLobby = v2float(m_uiSpace + m_uiPlayer->GetWidth(), height - m_uiLobby->GetHeight() - m_uiSpace);
	}
	else
	{
		m_lobbyPlayers[0].m_positionPlayer = v2float(m_uiSpace, m_uiSpace);
		m_lobbyPlayers[0].m_positionLobby = v2float(m_uiSpace, m_uiPlayer->GetHeight() - m_uiSpace * 2.0f);

		m_lobbyPlayers[1].m_positionPlayer = v2float(width - m_uiPlayer->GetWidth() - m_uiSpace, height - m_uiPlayer->GetHeight() - m_uiSpace);
		m_lobbyPlayers[1].m_positionLobby = v2float(width - m_uiLobby->GetWidth() - m_uiSpace, height - m_uiLobby->GetHeight() - m_uiPlayer->GetHeight() + m_uiSpace);

		m_lobbyPlayers[2].m_positionPlayer = v2float(width - m_uiPlayer->GetWidth() - m_uiSpace, m_uiSpace);
		m_lobbyPlayers[2].m_positionLobby = v2float(width - m_uiLobby->GetWidth() - m_uiSpace, m_uiPlayer->GetHeight() - m_uiSpace * 2.0f);

		m_lobbyPlayers[3].m_positionPlayer = v2float(m_uiSpace, height - m_uiPlayer->GetHeight() - m_uiSpace);
		m_lobbyPlayers[3].m_positionLobby = v2float(m_uiSpace, height - m_uiLobby->GetHeight() - m_uiPlayer->GetHeight() + m_uiSpace);
	}

	m_uiPressStart.SetPosition(width * 0.5f - m_uiPressStart.GetWidth() * 0.5f, height * 0.5f - m_uiPressStart.GetHeight() * 0.5f);

	// TO DO JU
	//m_uiPressB

	m_menuMain->OnResize();
	m_menuOptions->OnResize();

	ResetControllerUI();
}

void Application::ResetControllerUI()
{
	m_uiControllerMissing.SetPosition((static_cast<float>(m_pDX10_Renderer->GetWidth()) * 0.5f) - m_uiControllerMissing.GetWidth() * 0.5f, static_cast<float>(m_pDX10_Renderer->GetHeight()) - m_uiControllerMissing.GetHeight());
}

short Application::GetNumberOfConnectedControllers()
{
	short count = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (m_pContollers[i]->Connected())
		{
			count++;
		}
	}
	return count;
}
