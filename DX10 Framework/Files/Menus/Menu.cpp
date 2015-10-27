// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	:	Menu.cpp
// Description	:	The implementation file.
// Author(s)	:	Juran Griffith.
// Mail			:	juran.griffith@mediadesignschool.com
//

// Library Includes

// Local Includes
#include "Menu.h"

Menu::Menu()
{
	m_space = 20.0f;
	m_position.x = 0;
	m_position.y = 0;
	m_state = MENU_STATE_DEFAULT;
	m_elaspedTime = 0.0f;
	m_imageIndex = 0;
	m_exitPushed = false;
}

Menu::~Menu()
{
	// Release all sprite related resources
	ReleasePtr(m_title);

	while (!m_pContollers.empty())
	{
		m_pContollers.pop_back();
	}

	while (!m_sprites.empty())
	{
		ReleasePtr(m_sprites.back());
		m_sprites.pop_back();
	}

	while (!m_toggleButtons.empty())
	{
		ReleasePtr(m_toggleButtons.back());
		m_toggleButtons.pop_back();
	}

	while (!m_buttons.empty())
	{
		ReleasePtr(m_buttons.back());
		m_buttons.pop_back();
	}
}

bool Menu::Initialise(DX10_Renderer*_pDX10_Renderer, DX10_Shader_Sprite* _pShader, SoundManager* _pSoundManager, bool* _pKeyDown, MENU_LAYOUT _layout)
{
	if (_pDX10_Renderer && _pShader && _pSoundManager && _pKeyDown)
	{
		m_pDX10_Renderer = _pDX10_Renderer;
		m_pShader_Sprite = _pShader;
		m_pSoundManager = _pSoundManager;
		m_pKeyDown = _pKeyDown;
		m_layout = _layout;

		m_menuItem = 0;

		return true;
	}
	
	return false;
}

void Menu::Process(float _deltaTime)
{
	if (m_state == MENU_STATE_DEFAULT)
	{
		// Update the shader
		if (m_pShader_Sprite != 0)
		{
			m_pShader_Sprite->Update(_deltaTime);
		}

		// Draw Title
		if (m_title)
		{
			//m_title->Process(_deltaTime);
			if (m_elaspedTime > 60.0f)
			{
				m_title->SetState((BUTTON_STATE)++m_imageIndex);
				if (m_imageIndex == 3)
				{
					m_imageIndex = 0;
				}

				m_elaspedTime = 0.0f;
			}
			else
			{
				m_elaspedTime += _deltaTime * 100.0f;
			}
		}

		// Handle Controller Input
		for (UINT i = 0; i < m_pContollers.size(); ++i)
		{
			m_pContollers[i]->PreProcess();

			// Menu Item Navigation
			if (m_pContollers[i]->GetButtonDown(m_XButtons.DPad_Down) || m_pContollers[i]->GetStickDirectionDown(m_XStickDirections.LStick_Down) || m_pKeyDown[VK_DOWN])
			{
				m_pKeyDown[VK_DOWN] = false;
				if (m_menuItem < m_buttons.size() - 1)
				{
					m_pSoundManager->PlayMenuNavigate();
					if (m_menuItem >= 0)
					{
						m_buttons[m_menuItem]->m_pButton->SetState(BUTTON_STATE_DEFAULT);
					}

					m_menuItem++;
					if (m_menuItem >= 0)
					{
						m_buttons[m_menuItem]->m_pButton->SetState(BUTTON_STATE_HOVER);
					}
				}
			}
			else if (m_pContollers[i]->GetButtonDown(m_XButtons.DPad_Up) || m_pContollers[i]->GetStickDirectionDown(m_XStickDirections.LStick_Up) || m_pKeyDown[VK_UP])
			{
				m_pKeyDown[VK_UP] = false;
				if (m_menuItem > 0)
				{
					m_pSoundManager->PlayMenuNavigate();
					if (m_menuItem >= 0)
					{
						m_buttons[m_menuItem]->m_pButton->SetState(BUTTON_STATE_DEFAULT);
					}

					m_menuItem--;
					if (m_menuItem >= 0)
					{
						m_buttons[m_menuItem]->m_pButton->SetState(BUTTON_STATE_HOVER);
					}
				}
			}

			// Menu Item Selected
			if (m_pContollers[i]->GetButtonDown(m_XButtons.ActionButton_A) || m_pKeyDown[VK_RETURN]) // Confirm Selection
			{
				m_pKeyDown[VK_RETURN] = false;
				m_state = m_buttons[m_menuItem]->m_option;

				switch (m_state)
				{
				case MENU_STATE_FULL_SCREEN: // Fall through
				case MENU_STATE_SOUND: // Fall through
				case MENU_STATE_RUMBLE: // Fall Through
					m_pSoundManager->PlayMenuToggles();
					break;
				default:
					m_pSoundManager->PlayMenuAccept();
					break;
				}
			}
			else if (m_pContollers[i]->GetButtonDown(m_XButtons.ActionButton_B) || m_pKeyDown[VK_BACK])
			{
				m_pSoundManager->PlayMenuBack();
				m_pKeyDown[VK_BACK] = false;
				if (m_buttons[m_menuItem]->m_option == MENU_STATE_EXIT)
				{
					m_state = MENU_STATE_EXIT;
				}
				m_state = MENU_STATE_BACK;
			}

			m_pContollers[i]->PostProcess();
		}
	}
}

void Menu::Draw()
{
	if (m_state == MENU_STATE_DEFAULT)
	{
		// Turn the z buffer off
		//m_pDX10_Renderer->TurnZBufferOff();

		// Draw Title
		if (m_title)
		{
			m_title->Draw();
		}

		// Draw all the buttons
		for (auto button = m_buttons.begin(); button != m_buttons.end(); button++)
		{
			/*if (m_sprites.size() == 1 && m_buttons.size() > 1)
			{
				//m_sprites[0]->SetSize((*button)->m_pButton->GetWidth(), (*button)->m_pButton->GetHeight());
				m_sprites[0]->SetPosition((*button)->m_pButton->GetPosition().x, (*button)->m_pButton->GetPosition().y);
			}
			else
			{
				(*button)->m_pButton->Draw();
			}*/
			(*button)->m_pButton->Draw();
		}

		for (auto button = m_toggleButtons.begin(); button != m_toggleButtons.end(); button++)
		{
			m_sprites[0]->SetSize(m_toggleButtons[0]->GetWidth(), m_toggleButtons[0]->GetHeight());
			m_sprites[0]->SetPosition((*button)->GetPosition().x, (*button)->GetPosition().y);
			(*button)->Draw();
		}

		// Turn the z buffer on
		//m_pDX10_Renderer->TurnZBufferOn();
	}
}

bool Menu::AddSprite(std::string _filename, UINT _imageWidth, UINT _imageHeight, UINT _sliceWidth, UINT _sliceHeight)
{
	//	TO IMPROVE Juran
	//  - Dynamically find the width and height of image files (may need to check which type of image it is too)
	//	- Check for duplicate sprites
	m_sprites.push_back(new DXSprite());
	VALIDATE(m_sprites.back()->Initialise(m_pDX10_Renderer, m_pShader_Sprite, _filename, _imageWidth, _imageHeight, _sliceWidth, _sliceHeight));

	return true;
}

bool Menu::AddButton(MENU_STATE _option, UINT _spriteIndex, float _scale, UINT _col, UINT _row)
{
	if (_spriteIndex < m_sprites.size())
	{
		float offsety = 0.0f;
		if (m_buttons.size() > 0)
		{
			offsety = m_buttons[m_buttons.size() - 1]->m_pButton->GetHeight();
		}

		m_buttons.push_back(new TButton());
		GUI_Button* temp = new GUI_Button();
		switch (m_layout)
		{
		case MENU_LAYOUT_CENTRE:
		{
			float m_screenWidth = static_cast<float>(m_pDX10_Renderer->GetWidth());
			float m_screenHeight = static_cast<float>(m_pDX10_Renderer->GetHeight());

			/*RECT rect;
			if (GetClientRect(m_pShader_Sprite->GetHWnd(), &rect))
			{
				m_screenWidth = static_cast<float>(rect.right - rect.left);
				m_screenHeight = static_cast<float>(rect.bottom - rect.top);
			}*/

			m_screenWidth = m_screenWidth * 0.5f - (m_sprites[_spriteIndex]->GetWidth() * 0.25f);
			m_screenHeight = m_screenHeight * 0.5f;

			VALIDATE(temp->Initialise(m_pDX10_Renderer, m_sprites[_spriteIndex],
				m_position.x + m_screenWidth,
				m_position.y + m_screenHeight + (m_space + offsety) * (m_buttons.size() - 1),
				m_sprites[_spriteIndex]->GetWidth() * _scale,
				m_sprites[_spriteIndex]->GetHeight() * _scale));
		}
			
			break;
		case MENU_LAYOUT_CUSTOM:
			VALIDATE(temp->Initialise(m_pDX10_Renderer, m_sprites[_spriteIndex],
				m_position.x,
				m_position.y,
				m_sprites[_spriteIndex]->GetWidth() * _scale,
				m_sprites[_spriteIndex]->GetHeight() * _scale));
			break;
		}
		
		m_buttons.back()->SetButton(temp, _option, _spriteIndex, _scale, _col, _row);

		if (m_buttons.size() == 1)
		{
			m_buttons.back()->m_pButton->SetState(BUTTON_STATE_HOVER);
		}
	}
	
	return true;
}

bool Menu::AddToggleButton(TButton* _button, UINT _spriteIndex, bool _toggled, float _scale)
{
	if (_spriteIndex < m_sprites.size())
	{
		m_toggleButtons.push_back(new GUI_Button());
		m_toggleButtons.back()->SetState((BUTTON_STATE)_toggled);

		VALIDATE(m_toggleButtons.back()->Initialise(m_pDX10_Renderer, m_sprites[_spriteIndex],
			_button->m_pButton->GetPosition().x + _button->m_pButton->GetWidth() + m_space,
			_button->m_pButton->GetPosition().y,
			m_sprites[_spriteIndex]->GetWidth() * _scale,
			m_sprites[_spriteIndex]->GetHeight() * _scale));

		m_toggleButtons.back()->SetScale(_scale);
	}
	return true;
}

bool Menu::AddTitle(UINT _spriteIndex, float _scale, v2float _position)
{
	if (!m_title)
	{
		m_title = new GUI_Button();
		switch (m_layout)
		{
		case MENU_LAYOUT_CENTRE:
		{
			float m_screenWidth = static_cast<float>(m_pDX10_Renderer->GetWidth());
			float m_screenHeight = static_cast<float>(m_pDX10_Renderer->GetHeight());

			/*RECT rect;
			if (GetClientRect(m_pShader_Sprite->GetHWnd(), &rect))
			{
				m_screenWidth = static_cast<float>(rect.right - rect.left);
				m_screenHeight = static_cast<float>(rect.bottom - rect.top);
			}*/

			m_screenWidth = m_screenWidth * 0.5f - (m_sprites[_spriteIndex]->GetWidth() * 0.25f);
			m_screenHeight = m_screenHeight * 0.125f; // TO DO - Juran (work on a better system)

			if (m_position.x + m_screenWidth + m_sprites[_spriteIndex]->GetWidth() * _scale > static_cast<float>(m_pDX10_Renderer->GetWidth()))
			{
				m_screenWidth = static_cast<float>(m_pDX10_Renderer->GetWidth()) * 0.125f * 0.125f;
			}

			VALIDATE(m_title->Initialise(m_pDX10_Renderer, m_sprites[_spriteIndex],
				m_position.x + m_screenWidth,
				m_position.y + m_screenHeight,
				m_sprites[_spriteIndex]->GetWidth() * _scale,
				m_sprites[_spriteIndex]->GetHeight() * _scale));
		}
			
			break;
		case MENU_LAYOUT_CUSTOM:
			VALIDATE(m_title->Initialise(m_pDX10_Renderer, m_sprites[_spriteIndex],
				m_position.x + _position.x,
				m_position.y + _position.y,
				m_sprites[_spriteIndex]->GetWidth() * _scale,
				m_sprites[_spriteIndex]->GetHeight() * _scale));
			break;
		}
		m_title->SetScale(_scale);
	}

	return true;
}

BUTTON_STATE Menu::GetButtonState(UINT _index)
{
	if (_index < m_buttons.size())
	{
		return m_buttons[_index]->m_pButton->GetState();
	}
	return BUTTON_STATE_DEACTIVATED;
}

UINT Menu::GetCurrentMenuItem()
{
	return m_menuItem;
}

MENU_STATE Menu::GetMenuState()
{
	return m_state;
}

TButton* Menu::GetButton(UINT _index)
{
	if (_index < m_buttons.size())
	{
		return m_buttons[_index];
	}
	return NULL;
}

void Menu::SetPosition(v2float _position)
{
	m_position = _position;
}

void Menu::SetButtonPosition(UINT _index, v2float _position)
{
	m_buttons[_index]->m_pButton->SetPosition(_position);
}

void Menu::ToggleButton(UINT _index)
{
	if (m_toggleButtons[_index]->GetState() == BUTTON_STATE_DEFAULT)
		m_toggleButtons[_index]->SetState(BUTTON_STATE_HOVER);
	else
		m_toggleButtons[_index]->SetState(BUTTON_STATE_DEFAULT);
}

void Menu::Reset()
{
	m_state = MENU_STATE_DEFAULT;
}

/*void Menu::SetController(InputGamePad* _pGamepad)
{
	m_pGamepad = _pGamepad;
}*/

void Menu::AddController(InputGamePad* _controller)
{
	if (m_pContollers.size() != 4)
	{ 
		for (UINT i = 0; i < m_pContollers.size(); ++i)
		{
			if (m_pContollers[i] == _controller)
			{
				return;
			}
		}
		m_pContollers.push_back(_controller);
	}
}

void Menu::OnResize()
{
	if (m_title)
	{
		switch (m_layout)
		{
			case MENU_LAYOUT_CENTRE:
			{
				float m_screenWidth = static_cast<float>(m_pDX10_Renderer->GetWidth());
				float m_screenHeight = static_cast<float>(m_pDX10_Renderer->GetHeight());

				m_screenWidth = m_screenWidth * 0.5f - (m_title->GetSprite()->GetWidth() * 0.5f);
				m_screenHeight = m_screenHeight * 0.125f; // TO DO - Juran (work on a better system)

				if (m_position.x + m_screenWidth + m_title->GetSprite()->GetWidth() * m_title->GetScale() > static_cast<float>(m_pDX10_Renderer->GetWidth()))
				{
					m_screenWidth = static_cast<float>(m_pDX10_Renderer->GetWidth()) * 0.125f * 0.125f;
				}

				m_title->SetPosition(v2float(m_position.x + m_screenWidth, m_position.y + m_screenHeight));
			}
			break;
			case MENU_LAYOUT_CUSTOM:
				
			break;
		}
	}

	for (UINT i = 0; i < m_buttons.size(); ++i)
	{
		float offsety = m_buttons[i]->m_pButton->GetHeight();

		switch (m_layout)
		{
			case MENU_LAYOUT_CENTRE:
			{
				float m_screenWidth = static_cast<float>(m_pDX10_Renderer->GetWidth());
				float m_screenHeight = static_cast<float>(m_pDX10_Renderer->GetHeight());

				m_screenWidth = m_screenWidth * 0.5f - (m_buttons[i]->m_pButton->GetSprite()->GetWidth() * 0.5f);
				m_screenHeight = m_screenHeight * 0.5f;

				m_buttons[i]->m_pButton->SetPosition(v2float(m_position.x + m_screenWidth, m_position.y + m_screenHeight + (m_space + offsety) * i));
			}
			break;
			case MENU_LAYOUT_CUSTOM:
				
			break;
		}

	}

	for (UINT i = 0; i < m_toggleButtons.size(); ++i)
	{
		// TO DO - Juran (this assumes the toggle buttons are index the same as the normal buttons)
		m_toggleButtons[i]->SetPosition(v2float(m_buttons[i]->m_pButton->GetPosition().x + m_buttons[i]->m_pButton->GetWidth() + m_space, m_buttons[i]->m_pButton->GetPosition().y));
	}
}