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
#include "GUI_Button.h"

GUI_Button::GUI_Button()
{
	m_horizontal = false;
	m_boundsOffset = 0;
}

GUI_Button::~GUI_Button()
{
	//ReleasePtr(m_pSprite);
}

bool GUI_Button::Initialize(DX10_Renderer* _pDX10_Renderer, DXSprite* _pSprite, float _x, float _y, float _width, float _height)
{
	if (!_pDX10_Renderer && !_pSprite)
		return false;

	m_pDX10_Renderer = _pDX10_Renderer;
	m_pSprite = _pSprite;

	m_position.x = _x;
	m_position.y = _y;

	m_bounds.m_x = _x;
	m_bounds.m_y = _y;
	m_bounds.m_w = _width;
	m_bounds.m_h = _height;

	// Intialize the bounds
	m_bounds.m_w = m_position.x + (m_width / m_pSprite->GetSliceWidth()) - m_boundsOffset;
	m_bounds.m_h = m_position.y + (m_height / m_pSprite->GetSliceHeight()) - m_boundsOffset;

	//m_bounds.right = static_cast<long>(_x + (_width / _pBitmap->GetSliceWidth()));
	//m_bounds.bottom = static_cast<long>(_y + (_height / _pBitmap->GetSliceHeight()));

	m_width = _width;
	m_height = _height;

	/*if (!m_pBitmap->Initialize(_pD3DDevice, _pHWnd, _filename, _width, _height, _sliceWidth, _sliceHeight)) // 100 * 129, 300 * 387 //88 * 97 , 264 * 291
	return false; // 280 * 345*/

	return true;
}

void GUI_Button::GetPosition(D3DXVECTOR2& _out)	// TOIMPROVE - Maybe add the centre as the origin
{
	_out.x = m_position.x;
	_out.y = m_position.y;
}

BUTTON_STATE GUI_Button::GetState()
{
	return m_state;
}

void GUI_Button::SetPosition(float _x, float _y)
{
	m_position.x = _x;
	m_position.y = _y;

	m_bounds.m_x = m_position.x + m_boundsOffset;
	m_bounds.m_y = m_position.y + m_boundsOffset;

	// Intialize the bounds
	m_bounds.m_w = m_position.x + (m_width / m_pSprite->GetSliceWidth()) - m_boundsOffset;
	m_bounds.m_h = m_position.y + (m_height / m_pSprite->GetSliceHeight()) - m_boundsOffset;
}

void GUI_Button::SetState(BUTTON_STATE _state)
{
	m_state = _state;
}

/*void GUI_Button::SetBitmap(DXBitmap* _pBitmap)
{
	

	m_pBitmap = _pBitmap;
}*/

void GUI_Button::SetBoundsOffset(float _offset)
{
	m_boundsOffset = _offset;
}

void GUI_Button::Draw()
{
	switch (m_state)
	{
	case BUTTON_STATE::BUTTON_STATE_DEFAULT:
		m_pSprite->SetImageIndex(0);
		break;
	case BUTTON_STATE::BUTTON_STATE_HOVER:
		m_pSprite->SetImageIndex(1);
		break;
	case BUTTON_STATE::BUTTON_SELECTED:
		m_pSprite->SetImageIndex(2);
		break;
	case BUTTON_STATE::BUTTON_DEACTIVATED:
		break;
	}

	m_pSprite->SetSize(m_width, m_height);
	m_pSprite->SetPosition(m_position.x, m_position.y);
	m_pSprite->Render();
}

void GUI_Button::Process(float _deltaTime)
{
	if (m_state == BUTTON_STATE::BUTTON_DEACTIVATED)
		return;

	/*DIMOUSESTATE mouseCurrState = m_pInput->GetMouseState();

	D3DXVECTOR2 mousePosition;
	m_pInput->GetMousePosition(mousePosition);

	if (mousePosition.x >= m_bounds.m_x && mousePosition.x <= m_bounds.m_w &&
		mousePosition.y >= m_bounds.m_y && mousePosition.y <= m_bounds.m_h)
	{
		if (mouseCurrState.rgbButtons[0] && !m_pInput->IsMouseBtnLocked(0))
		{
			m_pInput->LockMouseBtn(0);
			m_state = eState::ES_SELECTED;
		}
		else
			m_state = eState::ES_HOVER;
	}
	else
		m_state = eState::ES_DEFAULT;*/
}