// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	:	GUI_Button.cpp
// Description	:	The implementation file.
// Author(s)	:	Juran Griffith.
// Mail			:	juran.griffith@mediadesignschool.com
//

// Library Includes

// Local Includes
#include "Menu.h"

Menu::Menu():
	m_scale(1.0f)
{
	m_position.x = 0;
	m_position.y = 0;
}

Menu::~Menu()
{
	// Release all sprite related resources
	ReleasePtr(m_pShader_Sprite);

	while (!m_sprites.empty())
	{
		ReleasePtr(m_sprites.back());
		m_sprites.pop_back();
	}

	while (!m_buttons.empty())
	{
		ReleasePtr(m_buttons.back());
		m_buttons.pop_back();
	}
}

bool Menu::Initialize(DX10_Renderer*_pDX10_Renderer, HWND* _pHWnd)
{
	m_pDX10_Renderer = _pDX10_Renderer;
	m_pHWnd = _pHWnd;

	m_menuItem = -1;

	m_pShader_Sprite = new DX10_Shader_Sprite();
	VALIDATE(m_pShader_Sprite->Initialize(m_pDX10_Renderer, m_pHWnd));

	return true;
}

void Menu::Process(float _deltaTime)
{
	// Update the shader
	if (m_pShader_Sprite != 0)
	{
		m_pShader_Sprite->Update(_deltaTime);
	}

	// Process all the buttons
	for (auto button = m_buttons.begin(); button != m_buttons.end(); button++)
	{
		(*button)->Process(_deltaTime);
	}
}

void Menu::Draw()
{
	// Turn the z buffer off
	m_pDX10_Renderer->TurnZBufferOff();

	// Draw all the buttons
	for (auto button = m_buttons.begin(); button != m_buttons.end(); button++)
	{
		(*button)->Draw();
	}

	// Turn the z buffer on
	m_pDX10_Renderer->TurnZBufferOn();
}

bool Menu::AddSprite(std::string _filename, UINT _imageWidth, UINT _imageHeight, UINT _sliceWidth, UINT _sliceHeight)
{
	//	TO IMPROVE Juran
	//  - Dynamically find the width and height of image files (may need to check which type of image it is too)
	//	- Check for duplicate sprites
	m_sprites.push_back(new DXSprite());
	VALIDATE(m_sprites.back()->Initialize(m_pHWnd, m_pDX10_Renderer, m_pShader_Sprite, _filename, _imageWidth, _imageHeight, _sliceWidth, _sliceHeight));

	return true;
}

bool Menu::AddButton(UINT _spriteIndex, float _scale)
{
	if (_spriteIndex < m_sprites.size())
	{
		float offsety = 0.0f;
		if (m_buttons.size() > 0) // TO IMPROVE Juran - More options on how to place the buttons
		{
			offsety = m_buttons[m_buttons.size() - 1]->GetHeight();
		}

		m_buttons.push_back(new GUI_Button());
		VALIDATE(m_buttons.back()->Initialize(m_pDX10_Renderer, m_sprites[_spriteIndex],
			m_position.x,
			m_position.y + offsety * m_sprites.size() * _scale,
			m_sprites[_spriteIndex]->GetWidth() * _scale,
			m_sprites[_spriteIndex]->GetHeight() * _scale));
	}
	
	return true;
}

BUTTON_STATE Menu::GetButtonState(UINT _index)
{
	if (_index < m_buttons.size())
		return m_buttons[_index]->GetState();
	return BUTTON_STATE_DEACTIVATED;
}

UINT Menu::GetCurrentMenuItem()
{
	return m_menuItem;
}

void Menu::SetPosition(v2float _position)
{
	m_position = _position;
}

void Menu::SetScale(float _scale)
{
	m_scale = _scale;
}