//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	:	DX10_Shader_Sprite.h
// Description	:	This wraps all the texture shader functions and variables
// Author		:	Juran Griffith
// Mail			:	juran.griffith@mediadesignschool.com
//

// Inclusion Guards
#pragma once
#ifndef __DX10_SHADER_SPRITE_H__
#define __DX10_SHADER_SPRITE_H__

// Local Includes
#include "../DX10_Utilities.h"
#include "../DX10_Renderer.h"

class DX10_Shader_Sprite
{
	// Functions
	public:
		/*******************
		-> Default constructor.
		@author:	Juran Griffith.
		@parameter:	None.
		@return:	...
		********************/
		DX10_Shader_Sprite()
		{
			m_pDX10_Renderer = 0;
			m_fxID = 0;
			m_techID = 0;
			m_vertexLayoutID = 0;

			m_pEmvView = 0;
			m_pEmvProjection = 0;
			m_pEmvWorld = 0;
			m_pEsrvTexture = 0;

			D3DXMatrixIdentity(&m_matWorld);
		}

		/*******************
		-> Copy constructor.
		@author:	Juran Griffith.
		@parameter:	None.
		@return:	...
		********************/
		//FXTexture(const FXTexture&);

		/*******************
		-> Default destructor.
		@author:	Juran Griffith.
		@parameter:	None.
		@return:	...
		********************/
		~DX10_Shader_Sprite()
		{
			m_pEmvView = 0;
			m_pEmvProjection = 0;
			m_pEmvWorld = 0;
			m_pEsrvTexture = 0;
		}

		/*******************
		-> Intializes the object
		@author:	Juran Griffith.
		@parameter: _pD3DDevice	- The graphics device.
		@parameter: _pHWnd		- The handler to the window.
		@return:	bool		- Returns true if the initialization succeeds otherwise the error on why it failed.
		********************/
		bool Initialize(DX10_Renderer*_pDX10_Renderer, HWND* _pHWnd)
		{
			m_pDX10_Renderer = _pDX10_Renderer;
			m_pDX10_Renderer->BuildFX("Sprite.fx", "Render", &m_fxID, &m_techID);
			
			// Get pointers to the three matrices inside the shader so we can update them from this class.
			m_pEmvWorld = m_pDX10_Renderer->GetFXVariable(m_fxID, "World")->AsMatrix();
			m_pEmvView = m_pDX10_Renderer->GetFXVariable(m_fxID, "View")->AsMatrix();
			m_pEmvProjection = m_pDX10_Renderer->GetFXVariable(m_fxID, "Projection")->AsMatrix();		

			// Get the pointer to the time variable inside the shader
			m_pEvDeltaTime = m_pDX10_Renderer->GetFXVariable(m_fxID, "DeltaTime");

			// Get pointer to the texture resource inside the shader.
			m_pEsrvTexture = m_pDX10_Renderer->GetFXVariable(m_fxID, "Texture")->AsShaderResource();

			// Now setup the layout of the data that goes into the shader.
			D3D10_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 }
			};

			UINT layoutElements = sizeof(layout) / sizeof(layout[0]);

			m_pDX10_Renderer->CreateVertexLayout(layout, layoutElements, m_techID, &m_vertexLayoutID);

			// Save the screen size.
			RECT rect;
			if (GetClientRect(*_pHWnd, &rect))
			{
				// Create an orthographic projection matrix for 2D rendering.
				D3DXMatrixOrthoLH(&m_matOrtho, static_cast<float>(rect.right - rect.left), static_cast<float>(rect.bottom - rect.top), 0.1f, 100.0f);
			}

			m_matView = *m_pDX10_Renderer->GetViewMatrix();

			return true;
		}

		/*******************
		-> This updates the world, view and projection constant buffer variables for the effect to use when rendering 
		@author:	Juran Griffith.
		@parameter: _deltaTime	- The current delta time.
		@return:	void
		********************/
		void Update(float _deltaTime)
		{
			// Update the constant buffer variables on the shader
			m_pEmvView->SetMatrix((float*)&m_matView);
			m_pEmvProjection->SetMatrix((float*)&m_matOrtho);
			m_pEmvWorld->SetMatrix((float*)&m_matWorld);
			m_pEvDeltaTime->SetRawValue(&_deltaTime, 0, sizeof(float));
		}

		/*******************
		-> Draws the 2D texture
		@author:	Juran Griffith.
		@parameter: _buffID	- Buffer ID of the object
		@parameter: _texID	- Texture ID for the texture to be used
		@return:	void
		********************/
		void Render(UINT _buffID, UINT _texID)
		{
			// Bind the texture.
			m_pEsrvTexture->SetResource(m_pDX10_Renderer->GetTexture(_texID));

			// Set the input layout.
			m_pDX10_Renderer->SetInputLayout(m_vertexLayoutID);

			// Retrieve the Technique
			ID3D10EffectTechnique* pTech = m_pDX10_Renderer->GetTechnique(m_techID);

			if (pTech != NULL)
			{
				D3D10_TECHNIQUE_DESC techDesc;
				pTech->GetDesc(&techDesc);
				for (UINT p = 0; p < techDesc.Passes; ++p)
				{
					pTech->GetPassByIndex(p)->Apply(0);
					m_pDX10_Renderer->RenderBuffer(_buffID);
				}
			}
		}

	protected:
	private:

	// Variables
	public:
	protected:
	private:
		DX10_Renderer*						m_pDX10_Renderer;

		ID3D10EffectMatrixVariable*			m_pEmvView;
		ID3D10EffectMatrixVariable*			m_pEmvProjection;
		ID3D10EffectMatrixVariable*			m_pEmvWorld;
		ID3D10EffectVariable*				m_pEvDeltaTime;
		ID3D10EffectShaderResourceVariable* m_pEsrvTexture;

		UINT m_fxID;
		UINT m_techID;
		UINT m_vertexLayoutID;
		D3DXMATRIX							m_matWorld;
		D3DXMATRIX							m_matView;
		D3DXMATRIX							m_matOrtho;
};

#endif	// __DX10_SHADER_SPRITE_H__