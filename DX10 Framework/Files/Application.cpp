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
	m_showMenu = true;

	// Save the client window sizes
	m_clientWidth = _clientWidth;
	m_clientHeight = _clientHeight;

	// Initialise boolean array for KeyDown to false
	m_pKeyDown = new bool[255];
	memset(m_pKeyDown, false, 255);

	// Create the Player One Pad controller
	m_pGamepadPlayerOne = new InputGamePad();
	VALIDATE(m_pGamepadPlayerOne->Initialise(1));

	VALIDATE(Initialise_DX10(_hInstance));

	if (m_showMenu == false)
	{
		m_pGame = new Game();
		VALIDATE(m_pGame->Initialise(m_pDX10_Renderer));
	}

	m_online = true;

	// Initialise all time keeping variables to default (zero) state
	m_pTimer = new Timer();
	m_pTimer->Reset();
	m_fps = 0;
	m_deltaTick = 0;
	m_fpsTimer = 0;
	
	return true;
}

bool Application::Initialise_DX10(HINSTANCE _hInstance)
{
	// Initialise the Renderer
	m_pDX10_Renderer = new DX10_Renderer();
	VALIDATE(m_pDX10_Renderer->Initialise(m_clientWidth, m_clientHeight, m_hWnd));

	// Initialise the Objects
	m_pCamera = new DX10_Camera_Debug();
	VALIDATE(m_pCamera->Initialise(m_pDX10_Renderer));
	m_pCamera->SetPostionVec({ 0, 0, -100.0f });
	m_pCamera->SetTargetVec({ 0, 0, 0 });
	m_pCamera->SetUpVec({ 0, 1, 0 });

	// Initialise Menu
	m_mainMenu = new Menu();
	VALIDATE(m_mainMenu->Initialize(m_pDX10_Renderer, &m_hWnd));

	m_mainMenu->AddSprite("Button/button_exit.png", 280, 345, 1, 3);
	m_mainMenu->AddButton(0, 0.5f);
	m_mainMenu->AddButton();

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

	// Delete and free the memory from the Renderer
	if (m_pDX10_Renderer != 0)
	{ 
		// DX10 pointers to release
		ReleasePtr(m_pCamera);
		
		// Gamepad input memory release
		ReleasePtr(m_pGamepadPlayerOne);

		// Menu memory release
		ReleasePtr(m_mainMenu);
		
		// Gameplay objects memory release
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

	// Limit to 60 FPS
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
		m_fpsTimer -= 1.0f;
		m_fps = 0;
	}
}

bool Application::Process(float _dt)
{
	VALIDATE(HandleInput());

	// Handle Controller Input
	m_pGamepadPlayerOne->PreProcess();

	if (m_pGamepadPlayerOne->GetButtonDown(m_XButtons.DPad_Down) || m_pGamepadPlayerOne->GetStickDirectionDown(m_XStickDirections.LStick_Down))
	{
		/*if (m_menuItem < m_buttons.size() - 1)
		{
			if (m_menuItem >= 0)
			{
				m_buttons[m_menuItem]->SetState(BUTTON_STATE_DEFAULT);
			}

			m_menuItem++;
			if (m_menuItem >= 0)
			{
				m_buttons[m_menuItem]->SetState(BUTTON_STATE_HOVER);
			}
		}*/
	}
	else if (m_pGamepadPlayerOne->GetButtonDown(m_XButtons.DPad_Up) || m_pGamepadPlayerOne->GetStickDirectionDown(m_XStickDirections.LStick_Up))
	{
		/*if (m_menuItem > 0)
		{
			if (m_menuItem >= 0)
			{
				m_buttons[m_menuItem]->SetState(BUTTON_STATE_DEFAULT);
			}

			m_menuItem--;
			if (m_menuItem >= 0)
			{
				m_buttons[m_menuItem]->SetState(BUTTON_STATE_HOVER);
			}
		}*/
	}

	m_pGamepadPlayerOne->PostProcess();

	// Processes to run when using DX10 Renderer
	if (m_pDX10_Renderer != 0)
	{		
		m_pCamera->Process();

		ProcessShaders();

		if (m_showMenu == true)
		{
			m_mainMenu->Process(_dt);
		}
		else
		{
			m_pGame->Process(_dt);
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

		if (m_showMenu == true)
		{
			m_mainMenu->Draw();
		}
		else
		{
			m_pGame->Render();
		}

		// Tell the Renderer the data input is over and present the outcome
		m_pDX10_Renderer->EndRender();	
	}
}

bool Application::HandleInput()
{
	if (m_pKeyDown[VK_ESCAPE])
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

















