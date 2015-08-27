#include "StdAfx.h"
#include "UIFadeButton.h"

namespace DuiLib {

	CFadeButtonUI::CFadeButtonUI() : 
		CAnimationUI( this ), 
		m_bMouseHove( FALSE ), 
		m_bMouseLeave( FALSE ),
		m_hasFadeEffect( TRUE )
	{
	}

	CFadeButtonUI::~CFadeButtonUI()
	{
		StopAnimation();
// 		CControlUI::~CControlUI();
	}

	LPCTSTR CFadeButtonUI::GetClass() const
	{
		return _T("FadeButtonUI");
	}

	LPVOID CFadeButtonUI::GetInterface(LPCTSTR pstrName)
	{
		if( !_tcscmp(pstrName, _T("FadeButton")) ) return static_cast<CFadeButtonUI*>(this);
		return __super::GetInterface(pstrName);
	}

	void CFadeButtonUI::DoEvent(TEventUI& event)
	{
		if (!m_hasFadeEffect)
		{
			__super::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_MOUSEENTER && !IsAnimationRunning( FADE_IN_ID ) )
		{
			m_bFadeAlpha = 0;
			m_bMouseHove = TRUE;
			StopAnimation( FADE_OUT_ID );
			StartAnimation( FADE_ELLAPSE, FADE_FRAME_COUNT, FADE_IN_ID );
			Invalidate();
// 			return;
		}
		if( event.Type == UIEVENT_MOUSELEAVE && !IsAnimationRunning( FADE_OUT_ID ) )
		{
			m_bFadeAlpha = 0;
			m_bMouseLeave = TRUE;
			StopAnimation(FADE_IN_ID);
			StartAnimation(FADE_ELLAPSE, FADE_FRAME_COUNT, FADE_OUT_ID);
			Invalidate();
// 			return;
		}
		if( event.Type == UIEVENT_TIMER ) 
		{
			OnTimer(  event.wParam );
		}
		__super::DoEvent( event );
	}

	void CFadeButtonUI::SetFadeEffect(BOOL hasFadeEffect)
	{
		m_hasFadeEffect = hasFadeEffect;
	}

	void CFadeButtonUI::OnTimer( int nTimerID )
	{
		OnAnimationElapse( nTimerID );
	}

	void CFadeButtonUI::PaintStatusImage(HDC hDC)
	{
		if (!m_hasFadeEffect) 
		{
			__super::PaintStatusImage(hDC);
			return;
		}

		if ((m_uButtonState & UISTATE_DISABLED) != 0) {
			if (!m_diDisabled.sDrawString.IsEmpty()) {
				if (!DrawImage(hDC, m_diDisabled))m_diDisabled.sDrawString.Empty();
				return;
			}
		} else if ((m_uButtonState & UISTATE_PUSHED) != 0) {
			if (!m_diPushed.sDrawString.IsEmpty()) {
				if (!DrawImage(hDC, m_diPushed)) m_diPushed.sDrawString.Empty();
				return;
			}
		} else if ((m_uButtonState & UISTATE_FOCUSED) != 0) {
			if (!m_diFocused.sDrawString.IsEmpty()) {
				if (!DrawImage(hDC, m_diFocused)) m_diFocused.sDrawString.Empty();
				return;
			}
		}

		if (!(m_diNormal.sDrawString.IsEmpty())) {
			if (IsAnimationRunning(FADE_IN_ID) || IsAnimationRunning(FADE_OUT_ID))
			{
				if (m_bMouseHove) {
					m_bMouseHove = FALSE;
					m_diLastImage.Clear();
					m_diLastImage.sDrawString = m_diHot.sDrawString;
					if (!DrawImage(hDC, m_diNormal))m_diNormal.sDrawString.Empty();
					return;
				}

				if (m_bMouseLeave) {
					m_bMouseLeave = FALSE;
                    m_diLastImage.Clear();
					m_diLastImage.sDrawString = m_diNormal.sDrawString;
					if (!DrawImage(hDC, m_diHot))
						m_diHot.sDrawString.Empty();
					return;
				}
				m_diOldImage.Clear();
				m_diNewImage.Clear();
				m_diOldImage.sDrawString = m_diNormal.sDrawString;
				m_diNewImage.sDrawString = m_diHot.sDrawString;
				if (IsAnimationRunning(FADE_OUT_ID)) {
					m_diOldImage.sDrawString = m_diHot.sDrawString;
					m_diNewImage.sDrawString = m_diNormal.sDrawString;
				}
				m_diOldImage.sDrawString = UpdateFadeAttribute(m_diOldImage.sDrawString, 255 - m_bFadeAlpha);
				m_diNewImage.sDrawString = UpdateFadeAttribute(m_diNewImage.sDrawString, m_bFadeAlpha);
				if (!DrawImage(hDC, m_diOldImage)) {
					m_diOldImage.sDrawString.Empty();
					OutputDebugString("failed \n");
				}

				if (!DrawImage(hDC, m_diNewImage)) {
					OutputDebugString("fail2d \n");
					m_diNewImage.sDrawString.Empty();
				}

				//Sleep(100);

// 				CDuiString outStr;
// 				outStr.Format("cur state %d %s\n", m_uButtonState, m_diNewImage.sDrawString);
// 				::OutputDebugString(outStr.GetData());
				return;


			} else {
				if (m_diLastImage.sDrawString.IsEmpty()) {
					m_diLastImage.sDrawString = m_diNormal.sDrawString;
				}
				if (!DrawImage(hDC, m_diLastImage))
					m_diLastImage.sDrawString.Empty();
				return;
			}
		}
	}


	void CFadeButtonUI::OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID)
	{
		m_bFadeAlpha = (int)((nCurFrame / (double)nTotalFrame) * 255 + 0.5);
		if(m_bFadeAlpha == 0) 
			m_bFadeAlpha = 10;
		Invalidate();
	}

	CDuiString CFadeButtonUI::UpdateFadeAttribute(const CDuiString& imageAttribute, int fadeAlpha)
	{
		CDuiString updateImageAttribute = imageAttribute;
		if (updateImageAttribute.Find(_T("file='")) == -1)
		{
			updateImageAttribute = _T("file='");
			updateImageAttribute += imageAttribute;
			updateImageAttribute += _T("' ");
		}

		if (int startFadePos = imageAttribute.Find(_T("fade='")) != -1)
		{
			int endFadePos = imageAttribute.Find(_T("'"), startFadePos + 6);
			ASSERT(endFadePos == -1);
			updateImageAttribute = imageAttribute.Mid(startFadePos, endFadePos - startFadePos);
		}

		updateImageAttribute += _T(" fade='%d' ");
		updateImageAttribute.Format(updateImageAttribute.GetData(), fadeAlpha);

		return updateImageAttribute;
	}

} // namespace DuiLib