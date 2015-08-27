#include "stdafx.h"
#include "UISpinButton.h"

namespace DuiLib
{
    CSpinButtonUI::CSpinButtonUI() : m_bHorizontal(false), m_nRangeMax(0), m_nRangeMin(0), m_nScrollPos(0),
        /*m_pOwner(NULL), */m_nScrollRepeatDelay(0), m_uButton1State(0), \
        m_uButton2State(0), m_nDelayTimes(DEFAULT_DELAY_TIMES),
        m_nTime(DEFAULT_SPEED), m_nInc(DEFAULT_INC), m_pControl(NULL), m_bDisableTimer(false)
    {
        ::ZeroMemory(&m_rcButton1, sizeof(m_rcButton1));
        ::ZeroMemory(&m_rcButton2, sizeof(m_rcButton2));
//         SetHorizontal(false);
    }

    LPCTSTR CSpinButtonUI::GetClass() const
    {
        return _T("SpinButtonUI");
    }

    LPVOID CSpinButtonUI::GetInterface(LPCTSTR pstrName)
    {
        if( _tcscmp(pstrName, DUI_CTR_SPINBUTTON) == 0 ) return static_cast<CSpinButtonUI*>(this);
        return CControlUI::GetInterface(pstrName);
    }

//     CContainerUI* CSpinButtonUI::GetOwner() const
//     {
//         return m_pOwner;
//     }
// 
//     void CSpinButtonUI::SetOwner(CContainerUI* pOwner)
//     {
//         m_pOwner = pOwner;
//     }

    void CSpinButtonUI::SetVisible(bool bVisible)
    {
        if( m_bVisible == bVisible ) return;

        bool v = IsVisible();
        m_bVisible = bVisible;
        if( m_bFocused ) m_bFocused = false;
    }

    void CSpinButtonUI::SetEnabled(bool bEnable)
    {
        CControlUI::SetEnabled(bEnable);
        if( !IsEnabled() ) {
            m_uButton1State = 0;
            m_uButton2State = 0;
        }
    }

    void CSpinButtonUI::SetFocus()
    {
//         if( m_pOwner != NULL ) m_pOwner->SetFocus();
        /*else*/ CControlUI::SetFocus();
    }

    bool CSpinButtonUI::IsHorizontal()
    {
        return m_bHorizontal;
    }

    void CSpinButtonUI::SetHorizontal(bool bHorizontal)
    {
        m_bHorizontal = bHorizontal;
    }

    int CSpinButtonUI::GetRangeMax() const
    {
        return m_nRangeMax;
    }

    int CSpinButtonUI::GetRangeMin() const
    {
        return m_nRangeMin;
    }

    void CSpinButtonUI::SetRangeMin(int nMin)
    {
        m_nRangeMin = nMin;
    }

    void CSpinButtonUI::SetRangeMax(int nMax)
    {
        m_nRangeMax = nMax;
    }

    void CSpinButtonUI::SetRange(int nMin, int nMax)
    {
        m_nRangeMin = nMin;
        m_nRangeMax = nMax;
        if( m_nScrollPos > m_nRangeMax ) m_nScrollPos = m_nRangeMax;
        if( m_nScrollPos < m_nRangeMin ) m_nScrollPos = m_nRangeMin;
    }

    int CSpinButtonUI::GetSpinPos() const
    {
        return m_nScrollPos;
    }

    void CSpinButtonUI::SetSpinPos(int nPos)
    {
        if( m_nScrollPos == nPos ) return;

        m_nScrollPos = nPos;
        if( m_nScrollPos < m_nRangeMin )
        {
            m_nScrollPos = m_nRangeMin;
        }
        if( m_nScrollPos > m_nRangeMax )
        {
            m_nScrollPos = m_nRangeMax;
        }

        if (m_pControl)
        {
            TCHAR szText[MAX_PATH] = {0};
            _itot_s(m_nScrollPos, szText, _countof(szText), 10);
            m_pControl->SetText(szText);
        }

        GetManager()->SendNotify(this, DUI_MSGTYPE_SPINCHANGED);
    }

    void CSpinButtonUI::SetSpinPosSilent(int nPos)
    {
        if( m_nScrollPos == nPos ) return;

        m_nScrollPos = nPos;
        if( m_nScrollPos < m_nRangeMin )
        {
            m_nScrollPos = m_nRangeMin;
        }
        if( m_nScrollPos > m_nRangeMax )
        {
            m_nScrollPos = m_nRangeMax;
        }

        if (m_pControl)
        {
            TCHAR szText[MAX_PATH] = {0};
            _itot_s(m_nScrollPos, szText, _countof(szText), 10);
            m_pControl->SetText(szText);
        }
    }


    LPCTSTR CSpinButtonUI::GetButton1NormalImage()
    {
		return m_diBtn1Normal.sDrawString;
    }

    void CSpinButtonUI::SetButton1NormalImage(LPCTSTR pStrImage)
    {
		if (m_diBtn1Normal.sDrawString == pStrImage
			&& m_diBtn1Normal.pImageInfo != NULL) {
			return;
		}
		m_diBtn1Normal.Clear();
		m_diBtn1Normal.sDrawString = pStrImage;
        Invalidate();
    }

    LPCTSTR CSpinButtonUI::GetButton1HotImage()
    {
		return m_diBtn1Hot.sDrawString;
    }

    void CSpinButtonUI::SetButton1HotImage(LPCTSTR pStrImage)
    {
		if (m_diBtn1Hot.sDrawString == pStrImage
			&& m_diBtn1Hot.pImageInfo != NULL) {
			return;
		}
		m_diBtn1Hot.Clear();
		m_diBtn1Hot.sDrawString = pStrImage;
        Invalidate();
    }

    LPCTSTR CSpinButtonUI::GetButton1PushedImage()
    {
		return m_diBtn1Pushed.sDrawString;
    }

    void CSpinButtonUI::SetButton1PushedImage(LPCTSTR pStrImage)
    {
		if (m_diBtn1Pushed.sDrawString == pStrImage
			&& m_diBtn1Pushed.pImageInfo != NULL) {
			return;
		}
		m_diBtn1Pushed.Clear();
		m_diBtn1Pushed.sDrawString = pStrImage;
		Invalidate();
    }

    LPCTSTR CSpinButtonUI::GetButton1DisabledImage()
    {
		return m_diBtn1Disabled.sDrawString;
    }

    void CSpinButtonUI::SetButton1DisabledImage(LPCTSTR pStrImage)
    {
		if (m_diBtn1Disabled.sDrawString == pStrImage
			&& m_diBtn1Disabled.pImageInfo != NULL) {
			return;
		}
		m_diBtn1Disabled.Clear();
		m_diBtn1Disabled.sDrawString = pStrImage;
        Invalidate();
    }

    LPCTSTR CSpinButtonUI::GetButton2NormalImage()
    {
		return  m_diBtn2Normal.sDrawString;;
    }

    void CSpinButtonUI::SetButton2NormalImage(LPCTSTR pStrImage)
    {
		if (m_diBtn2Normal.sDrawString == pStrImage
			&& m_diBtn2Normal.pImageInfo != NULL) {
			return;
		}
		m_diBtn2Normal.Clear();
		m_diBtn2Normal.sDrawString = pStrImage;
        Invalidate();
    }

    LPCTSTR CSpinButtonUI::GetButton2HotImage()
    {
		return m_diBtn2Hot.sDrawString;
    }

    void CSpinButtonUI::SetButton2HotImage(LPCTSTR pStrImage)
    {
		if (m_diBtn2Hot.sDrawString == pStrImage
			&& m_diBtn2Hot.pImageInfo != NULL) {
			return;
		}
		m_diBtn2Hot.Clear();
		m_diBtn2Hot.sDrawString = pStrImage;
        Invalidate();
    }

    LPCTSTR CSpinButtonUI::GetButton2PushedImage()
    {
		return m_diBtn2Pushed.sDrawString;
    }

    void CSpinButtonUI::SetButton2PushedImage(LPCTSTR pStrImage)
    {
		if (m_diBtn2Pushed.sDrawString == pStrImage
			&& m_diBtn2Pushed.pImageInfo != NULL) {
			return;
		}
		m_diBtn2Pushed.Clear();
		m_diBtn2Pushed.sDrawString = pStrImage;
        Invalidate();
    }

    LPCTSTR CSpinButtonUI::GetButton2DisabledImage()
    {
		return m_diBtn2Disabled.sDrawString;
    }

    void CSpinButtonUI::SetButton2DisabledImage(LPCTSTR pStrImage)
    {
		if (m_diBtn2Disabled.sDrawString == pStrImage
			&& m_diBtn2Disabled.pImageInfo != NULL) {
			return;
		}
		m_diBtn2Disabled.Clear();
		m_diBtn2Disabled.sDrawString = pStrImage;
        Invalidate();
    }
#if 0
    LPCTSTR CSpinButtonUI::GetButtonBothNormalImage()
    {
		//return m_diBtn2Disabled.sDrawString;
    }

    void CSpinButtonUI::SetButtonBothNormalImage(LPCTSTR pStrImage)
    {
        m_sButtonBothNormalImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CSpinButtonUI::GetButtonBoth1HotImage()
    {
        return m_sButtonBoth1HotImage;
    }

    void CSpinButtonUI::SetButtonBoth1HotImage(LPCTSTR pStrImage)
    {
        m_sButtonBoth1HotImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CSpinButtonUI::GetButtonBoth1PushedImage()
    {
        return m_sButtonBoth1PushedImage;
    }

    void CSpinButtonUI::SetButtonBoth1PushedImage(LPCTSTR pStrImage)
    {
        m_sButtonBoth1PushedImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CSpinButtonUI::GetButtonBoth2HotImage()
    {
        return m_sButtonBoth2HotImage;
    }

    void CSpinButtonUI::SetButtonBoth2HotImage(LPCTSTR pStrImage)
    {
        m_sButtonBoth2HotImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CSpinButtonUI::GetButtonBoth2PushedImage()
    {
        return m_sButtonBoth2PushedImage;
    }

    void CSpinButtonUI::SetButtonBoth2PushedImage(LPCTSTR pStrImage)
    {
        m_sButtonBoth2PushedImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CSpinButtonUI::GetButtonBothDisabledImage()
    {
        return m_sButtonBothDisabledImage;
    }

    void CSpinButtonUI::SetButtonBothDisabledImage(LPCTSTR pStrImage)
    {
        m_sButtonBothDisabledImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CSpinButtonUI::GetBkNormalImage()
    {
        return m_sBkNormalImage;
    }

    void CSpinButtonUI::SetBkNormalImage(LPCTSTR pStrImage)
    {
        m_sBkNormalImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CSpinButtonUI::GetBkHotImage()
    {
        return m_sBkHotImage;
    }

    void CSpinButtonUI::SetBkHotImage(LPCTSTR pStrImage)
    {
        m_sBkHotImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CSpinButtonUI::GetBkPushedImage()
    {
        return m_sBkPushedImage;
    }

    void CSpinButtonUI::SetBkPushedImage(LPCTSTR pStrImage)
    {
        m_sBkPushedImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CSpinButtonUI::GetBkDisabledImage()
    {
        return m_sBkDisabledImage;
    }

    void CSpinButtonUI::SetBkDisabledImage(LPCTSTR pStrImage)
    {
        m_sBkDisabledImage = pStrImage;
        Invalidate();
    }
#endif
    int CSpinButtonUI::GetSpeed()
    {
        return m_nTime;
    }

    void CSpinButtonUI::SetSpeed(int nMillisec)
    {
        m_nTime = nMillisec;
    }

    int CSpinButtonUI::GetInc()
    {
        return m_nInc;
    }

    void CSpinButtonUI::SetInc(int nInc)
    {
        m_nInc = nInc;
    }

    void CSpinButtonUI::SetBuddy(CControlUI* pControl)
    {
        m_pControl = pControl;
        if (m_pControl)
        {
            TCHAR szText[MAX_PATH] = {0};
            _itot_s(m_nScrollPos, szText, _countof(szText), 10);
            m_pControl->SetText(szText);
        }
    }

    CControlUI* CSpinButtonUI::GetBuddy()
    {
        return m_pControl;
    }

    void CSpinButtonUI::SetDelay(int nTimes)
    {
        m_nDelayTimes = nTimes;
    }

    int CSpinButtonUI::GetDelay()
    {
        return m_nDelayTimes;
    }

    void CSpinButtonUI::SetPos(RECT rc,bool bNeedInvalidate)
    {
        CControlUI::SetPos(rc,bNeedInvalidate);

        if( m_bHorizontal ) {
            m_rcButton1 = m_rcItem;
            m_rcButton2 = m_rcItem;
            m_rcButton1.right = (m_rcItem.left + m_rcItem.right) / 2;
            m_rcButton2.left = (m_rcItem.left + m_rcItem.right) / 2;
        }
        else {
            m_rcButton1 = m_rcItem;
            m_rcButton2 = m_rcItem;
            m_rcButton1.bottom = (m_rcItem.top + m_rcItem.bottom) / 2;
            m_rcButton2.top = (m_rcItem.top + m_rcItem.bottom) / 2;
        }

//         if( m_pOwner != NULL ) m_pOwner->NeedUpdate(); else NeedParentUpdate();
    }

    void CSpinButtonUI::DoEvent(TEventUI& event)
    {
        if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        CControlUI::DoEvent(event);
            return;
        }

        if( event.Type == UIEVENT_SETFOCUS ) 
        {
            return;
        }
        if( event.Type == UIEVENT_KILLFOCUS ) 
        {
            if (!m_bDisableTimer)
            {
                m_pManager->KillTimer(this, DEFAULT_TIMERID);
            }
            return;
        }
        if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK )
        {
            if( !IsEnabled() ) return;

            m_nScrollRepeatDelay = 0;
            if (!m_bDisableTimer)
            {
                m_pManager->SetTimer(this, DEFAULT_TIMERID, (UINT)m_nTime);
            }

            if( ::PtInRect(&m_rcButton1, event.ptMouse) ) {
                m_uButton1State |= UISTATE_PUSHED;
                m_uButton2State &= ~UISTATE_PUSHED;
                m_uButton2State &= ~UISTATE_HOT;
                SetSpinPos(m_nScrollPos + m_nInc);
                Invalidate();
            }
            else if( ::PtInRect(&m_rcButton2, event.ptMouse) ) {
                m_uButton2State |= UISTATE_PUSHED;
                m_uButton1State &= ~UISTATE_PUSHED;
                m_uButton1State &= ~UISTATE_HOT;
                SetSpinPos(m_nScrollPos - m_nInc);
                Invalidate();
            }
//             if( m_pManager != NULL && m_pOwner == NULL ) m_pManager->SendNotify(this, DUI_MSGTYPE_SCROLL);
            return;
        }
        if( event.Type == UIEVENT_BUTTONUP )
        {
            m_nScrollRepeatDelay = 0;
            if (!m_bDisableTimer)
            {
                m_pManager->KillTimer(this, DEFAULT_TIMERID);
            }

            if( (m_uButton1State & UISTATE_PUSHED) != 0 ) {
                m_uButton1State &= ~UISTATE_PUSHED;
                m_uButton2State &= ~UISTATE_HOT;
                m_uButton2State &= ~UISTATE_PUSHED;
                Invalidate();
            }
            else if( (m_uButton2State & UISTATE_PUSHED) != 0 ) {
                m_uButton2State &= ~UISTATE_PUSHED;
                m_uButton1State &= ~UISTATE_HOT;
                m_uButton1State &= ~UISTATE_PUSHED;
                Invalidate();
            }
            return;
        }
        if( event.Type == UIEVENT_MOUSEMOVE )
        {
            if( IsEnabled() ) {
                if (PtInRect(&m_rcButton1, event.ptMouse))
                {
                    m_uButton1State |= UISTATE_HOT;
                    m_uButton2State &= ~UISTATE_HOT;
                }
                else if (PtInRect(&m_rcButton2, event.ptMouse))
                {
                    m_uButton2State |= UISTATE_HOT;
                    m_uButton1State &= ~UISTATE_HOT;
                }
                Invalidate();
            }
            return;
        }
        if( event.Type == UIEVENT_CONTEXTMENU )
        {
            return;
        }
        if( event.Type == UIEVENT_TIMER && event.wParam == DEFAULT_TIMERID )
        {
//             if (::GetFocus() != GetManager()->GetPaintWindow())
//             {
//                 GetManager()->KillTimer(this);
//                 return;
//             }
            ++m_nScrollRepeatDelay;
            if( (m_uButton1State & UISTATE_PUSHED) != 0 ) {
                if( m_nScrollRepeatDelay <= 5 ) return;
                SetSpinPos(m_nScrollPos + m_nInc);
            }
            else if( (m_uButton2State & UISTATE_PUSHED) != 0 ) {
                if( m_nScrollRepeatDelay <= 5 ) return;
                SetSpinPos(m_nScrollPos - m_nInc);
            }
//             if( m_pManager != NULL && m_pOwner == NULL ) m_pManager->SendNotify(this, DUI_MSGTYPE_SCROLL);
            return;
        }
        if( event.Type == UIEVENT_MOUSEENTER )
        {
            if( IsEnabled() ) {
                if (PtInRect(&m_rcButton1, event.ptMouse))
                {
                    m_uButton1State |= UISTATE_HOT;
                    m_uButton2State &= ~UISTATE_HOT;
                }
                else if (PtInRect(&m_rcButton2, event.ptMouse))
                {
                    m_uButton2State |= UISTATE_HOT;
                    m_uButton1State &= ~UISTATE_HOT;
                }
                Invalidate();
            }
            return;
        }
        if( event.Type == UIEVENT_MOUSELEAVE )
        {
            if( IsEnabled() ) {
                m_uButton1State &= ~UISTATE_HOT;
                m_uButton2State &= ~UISTATE_HOT;
                Invalidate();
            }
            return;
        }

        /*if( m_pOwner != NULL ) m_pOwner->DoEvent(event); else */CControlUI::DoEvent(event);
    }

    void CSpinButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if( _tcscmp(pstrName, _T("button1normalimage")) == 0 ) SetButton1NormalImage(pstrValue);
        else if( _tcscmp(pstrName, _T("button1hotimage")) == 0 ) SetButton1HotImage(pstrValue);
        else if( _tcscmp(pstrName, _T("button1pushedimage")) == 0 ) SetButton1PushedImage(pstrValue);
        else if( _tcscmp(pstrName, _T("button1disabledimage")) == 0 ) SetButton1DisabledImage(pstrValue);
        else if( _tcscmp(pstrName, _T("button2normalimage")) == 0 ) SetButton2NormalImage(pstrValue);
        else if( _tcscmp(pstrName, _T("button2hotimage")) == 0 ) SetButton2HotImage(pstrValue);
        else if( _tcscmp(pstrName, _T("button2pushedimage")) == 0 ) SetButton2PushedImage(pstrValue);
        else if( _tcscmp(pstrName, _T("button2disabledimage")) == 0 ) SetButton2DisabledImage(pstrValue);
#if 0
        else if( _tcscmp(pstrName, _T("buttonbothnormalimage")) == 0 ) SetButtonBothNormalImage(pstrValue);
        else if( _tcscmp(pstrName, _T("buttonboth1hotimage")) == 0 ) SetButtonBoth1HotImage(pstrValue);
        else if( _tcscmp(pstrName, _T("buttonboth1pushedimage")) == 0 ) SetButtonBoth1PushedImage(pstrValue);
        else if( _tcscmp(pstrName, _T("buttonboth2hotimage")) == 0 ) SetButtonBoth2HotImage(pstrValue);
        else if( _tcscmp(pstrName, _T("buttonboth2pushedimage")) == 0 ) SetButtonBoth2PushedImage(pstrValue);
        else if( _tcscmp(pstrName, _T("buttonbothdisabledimage")) == 0 ) SetButtonBothDisabledImage(pstrValue);
        else if( _tcscmp(pstrName, _T("bknormalimage")) == 0 ) SetBkNormalImage(pstrValue);
        else if( _tcscmp(pstrName, _T("bkhotimage")) == 0 ) SetBkHotImage(pstrValue);
        else if( _tcscmp(pstrName, _T("bkpushedimage")) == 0 ) SetBkPushedImage(pstrValue);
        else if( _tcscmp(pstrName, _T("bkdisabledimage")) == 0 ) SetBkDisabledImage(pstrValue);
#endif
        else if( _tcscmp(pstrName, _T("hor")) == 0 ) SetHorizontal(_tcscmp(pstrValue, _T("true")) == 0);
        else if( _tcscmp(pstrName, _T("rangemin")) == 0 ) SetRangeMin(_ttoi(pstrValue));
        else if( _tcscmp(pstrName, _T("rangemax")) == 0 ) SetRangeMax(_ttoi(pstrValue));
        else if( _tcscmp(pstrName, _T("value")) == 0 ) SetSpinPos(_ttoi(pstrValue));
        else if( _tcscmp(pstrName, _T("delaytimes")) == 0 ) SetDelay(_ttoi(pstrValue));
        else if( _tcscmp(pstrName, _T("speed")) == 0) SetSpeed(_ttoi(pstrValue));
        else if( _tcscmp(pstrName, _T("inc")) == 0) SetInc(_ttoi(pstrValue));
        else CControlUI::SetAttribute(pstrName, pstrValue);
    }

    void CSpinButtonUI::DoPaint(HDC hDC, const RECT& rcPaint)
    {
        if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;
        PaintBk(hDC);
       // if (m_sButtonBothNormalImage.IsEmpty())
        {
            PaintButton1(hDC);
            PaintButton2(hDC);
        }
//         else
//         {
//             PaintBothButton(hDC);
//         }
    }

    void CSpinButtonUI::PaintBk(HDC hDC)
    {
        //if( !m_sBkNormalImage.IsEmpty() ) {
         //   if( !DrawImage(hDC, (LPCTSTR)m_sBkNormalImage) ) m_sBkNormalImage.Empty();
         //   else return;
       // }
    }

    void CSpinButtonUI::PaintBothButton(HDC hDC)
    {
#if 0
        if( !IsEnabled() )
        {
            m_uButton1State |= UISTATE_DISABLED;
            m_uButton2State |= UISTATE_DISABLED;
        }
        else
        {
            m_uButton1State &= ~ UISTATE_DISABLED;
            m_uButton2State &= ~ UISTATE_DISABLED;
        }

        //         m_sImageModify.Empty();
        //         m_sImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), m_rcButton1.left - m_rcItem.left, \
        //             m_rcButton1.top - m_rcItem.top, m_rcButton1.right - m_rcItem.left, m_rcButton1.bottom - m_rcItem.top);
        // 
        if( (m_uButton1State & UISTATE_DISABLED) != 0 ) {
            if( !m_sButtonBothDisabledImage.IsEmpty() ) {
                if( !DrawImage(hDC, (LPCTSTR)m_sButtonBothDisabledImage, (LPCTSTR)m_sImageModify) ) m_sButtonBothDisabledImage.Empty();
                else return;
            }
        }
        else if( (m_uButton1State & UISTATE_PUSHED) != 0 ) {
            if( !m_sButtonBoth1PushedImage.IsEmpty() ) {
                if( !DrawImage(hDC, (LPCTSTR)m_sButtonBoth1PushedImage, (LPCTSTR)m_sImageModify) ) m_sButtonBoth1PushedImage.Empty();
                else return;
            }
        }
        else if( (m_uButton2State & UISTATE_PUSHED) != 0 ) {
            if( !m_sButtonBoth2PushedImage.IsEmpty() ) {
                if( !DrawImage(hDC, (LPCTSTR)m_sButtonBoth2PushedImage, (LPCTSTR)m_sImageModify) ) m_sButtonBoth2PushedImage.Empty();
                else return;
            }
        }
        else if( (m_uButton1State & UISTATE_HOT) != 0 ) {
            if( !m_sButtonBoth1HotImage.IsEmpty() ) {
                if( !DrawImage(hDC, (LPCTSTR)m_sButtonBoth1HotImage, (LPCTSTR)m_sImageModify) ) m_sButtonBoth1HotImage.Empty();
                else return;
            }
        }
        else if( (m_uButton2State & UISTATE_HOT) != 0 ) {
            if( !m_sButtonBoth2HotImage.IsEmpty() ) {
                if( !DrawImage(hDC, (LPCTSTR)m_sButtonBoth2HotImage, (LPCTSTR)m_sImageModify) ) m_sButtonBoth2HotImage.Empty();
                else return;
            }
        }

        if( !m_sButtonBothNormalImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sButtonBothNormalImage, (LPCTSTR)m_sImageModify) ) m_sButtonBothNormalImage.Empty();
            else return;
        }
#endif
        DWORD dwBorderColor = 0xFF85E4FF;
        int nBorderSize = 2;
        CRenderEngine::DrawRect(hDC, m_rcButton1, nBorderSize, dwBorderColor);
    }

    void CSpinButtonUI::PaintButton1(HDC hDC)
    {
        if( !IsEnabled() ) m_uButton1State |= UISTATE_DISABLED;
        else m_uButton1State &= ~ UISTATE_DISABLED;

		RECT rc = { 0 };
		rc.left = m_rcButton1.left - m_rcItem.left;
		rc.top = m_rcButton1.top - m_rcItem.top;
		rc.right = m_rcButton1.right - m_rcItem.left;
		rc.bottom = m_rcButton1.bottom - m_rcItem.top;

		if ((m_uButton1State & UISTATE_DISABLED) != 0) {
			if (!m_diBtn1Disabled.sDrawString.IsEmpty()) {
				m_diBtn1Disabled.rcDestOffset = rc;
				DrawImage(hDC, m_diBtn1Disabled); return;
			}
		} else if ((m_uButton1State & UISTATE_PUSHED) != 0) {
			if (!m_diBtn1Pushed.sDrawString.IsEmpty()) {
				m_diBtn1Pushed.rcDestOffset = rc;
				DrawImage(hDC, m_diBtn1Pushed); return;
			}
		} else if ((m_uButton1State & UISTATE_HOT) != 0) {
			if (!m_diBtn1Hot.sDrawString.IsEmpty()) {
				m_diBtn1Hot.rcDestOffset = rc;
				DrawImage(hDC, m_diBtn1Hot); return;
			}
		}

		if (!m_diBtn1Normal.sDrawString.IsEmpty()) {
			if (!m_diBtn1Normal.sDrawString.IsEmpty()) {
				m_diBtn1Normal.rcDestOffset = rc;
				DrawImage(hDC, m_diBtn1Normal);
			}
		}

        DWORD dwBorderColor = 0xFF85E4FF;
        int nBorderSize = 2;
        CRenderEngine::DrawRect(hDC, m_rcButton1, nBorderSize, dwBorderColor);

    }

    void CSpinButtonUI::PaintButton2(HDC hDC)
    {
        if( !IsEnabled() ) m_uButton2State |= UISTATE_DISABLED;
        else m_uButton2State &= ~ UISTATE_DISABLED;

		RECT rc = { 0 };
		rc.left = m_rcButton2.left - m_rcItem.left;
		rc.top = m_rcButton2.top - m_rcItem.top;
		rc.right = m_rcButton2.right - m_rcItem.left;
		rc.bottom = m_rcButton2.bottom - m_rcItem.top;

        if( (m_uButton2State & UISTATE_DISABLED) != 0 ) {
            if( !m_diBtn2Disabled.sDrawString.IsEmpty() ) {
				m_diBtn2Disabled.rcDestOffset = rc;
				DrawImage(hDC, m_diBtn2Disabled); return;
            }
        }
        else if( (m_uButton2State & UISTATE_PUSHED) != 0 ) {
			if (!m_diBtn2Pushed.sDrawString.IsEmpty()) {
				m_diBtn2Pushed.rcDestOffset = rc;
				DrawImage(hDC, m_diBtn2Pushed); return;
			}
        }
        else if( (m_uButton2State & UISTATE_HOT) != 0 ) {
			if (!m_diBtn2Hot.sDrawString.IsEmpty()) {
				m_diBtn2Hot.rcDestOffset = rc;
				DrawImage(hDC, m_diBtn2Hot); return;
			}
        }

		if (!m_diBtn2Normal.sDrawString.IsEmpty()) {
			if (!m_diBtn2Normal.sDrawString.IsEmpty()) {
				m_diBtn2Normal.rcDestOffset = rc;
				DrawImage(hDC, m_diBtn2Normal); 
			}
        }

        DWORD dwBorderColor = 0xFF85E4FF;
        int nBorderSize = 2;
        CRenderEngine::DrawRect(hDC, m_rcButton2, nBorderSize, dwBorderColor);
    }

    void CSpinButtonUI::ToLoseFocus()
    {
        if (::GetFocus() != m_pManager->GetPaintWindow()
            && ::GetParent(::GetFocus()) != m_pManager->GetPaintWindow()
            && !m_bDisableTimer)
        {
            m_pManager->KillTimer(this, DEFAULT_TIMERID);
        }
    }

    void CSpinButtonUI::DisableTimer(bool bDisable /* = true */)
    {
        m_bDisableTimer = bDisable;
    }
}