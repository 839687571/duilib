#include "stdafx.h"
#include "UIGifButton.h"

namespace DuiLib
{
	CGifButtonUI::CGifButtonUI()
		:m_pGif(NULL)
		,m_nPreUpdateDelay(0)
		,m_isUpdateTime(false)
	{
	}

	CGifButtonUI::~CGifButtonUI()
	{
		m_pManager->KillTimer(this, GIF_TIMER_ID);
		
		if (m_pGif)
		{
			delete m_pGif;
			m_pGif = NULL;
		}
	}

	void CGifButtonUI::SetVisible( bool bVisible /*= true*/ )
	{
		if(bVisible == false)
			m_pManager->KillTimer(this, GIF_TIMER_ID);

		CButtonUI::SetVisible(bVisible);
	}

	void CGifButtonUI::PaintStatusImage( HDC hDC )
	{
		if(m_pGif)
		{
			TImageInfo* pImageInfo = NULL;
			if (m_isUpdateTime)
			{
				m_isUpdateTime = false;
				pImageInfo = m_pGif->GetNextFrameInfo();
			}
			else
			{
				pImageInfo = m_pGif->GetCurrentFrameInfo();
			}
			if (pImageInfo)
			{
				RECT rcBmpPart = {0};
				RECT rcCorners = {0};
				rcBmpPart.right = pImageInfo->nX;
				rcBmpPart.bottom = pImageInfo->nY;
				CRenderEngine::DrawImage(hDC,pImageInfo->hBitmap,m_rcItem, m_rcPaint,rcBmpPart,rcCorners,pImageInfo->alphaChannel,255);
				if (m_nPreUpdateDelay != pImageInfo->delay)
				{
					m_pManager->KillTimer(this, GIF_TIMER_ID);
					m_pManager->SetTimer(this, GIF_TIMER_ID, pImageInfo->delay);
					m_nPreUpdateDelay = pImageInfo->delay;
				}
			}
		}
		//没有gif图片,则与普通按钮一样
		else
		{
             CButtonUI::PaintStatusImage(hDC);
		}
	}

	void CGifButtonUI::DoEvent( TEventUI& event )
	{
		if( event.Type == UIEVENT_TIMER && event.wParam == GIF_TIMER_ID )
		{
			m_isUpdateTime = true;
			Invalidate();
			return;
		}

		CButtonUI::DoEvent(event);
	}

	void CGifButtonUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
	{
		if( _tcscmp(pstrName, _T("NormalGifFile")) == 0 )
		{
			SetNormalGifFile(pstrValue);
		} else {

		CButtonUI::SetAttribute(pstrName,pstrValue);
		}
	}

	void CGifButtonUI::SetNormalGifFile( LPCTSTR pstrName )
	{
		if(pstrName == NULL) return;
		
		if (m_pGif)
		{
			m_pManager->KillTimer(this, GIF_TIMER_ID);
			m_nPreUpdateDelay = 0;
			delete m_pGif;
			m_pGif = NULL;
		}
		
		m_pGif = CRenderEngine::LoadGif(STRINGorID(pstrName),0, 0);
		
		Invalidate();
	}
}