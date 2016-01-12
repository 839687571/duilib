#include "StdAfx.h"
#include "UITimeSpin.h"

namespace DuiLib
{

CTimeSpinUI::CTimeSpinUI()
: m_dwTime(0)
, m_dwMinTime(0)
, m_dwMaxTime(359999)
, m_pHour(NULL)
, m_pMinute(NULL)
, m_pSecond(NULL)
, m_pSpin(NULL)
, m_nBtnWidth(20)
, m_nFocus(2)
{
    InitInnerControl();
}

CTimeSpinUI::~CTimeSpinUI()
{
	GetManager()->RemoveNotifier(this);
}
void CTimeSpinUI::InitInnerControl()
{
    m_pHour = new CEditUI;
    m_pMinute = new CEditUI;
    m_pSecond = new CEditUI;
    m_pSpin = new CSpinButtonUI;
    m_pPlaceHolder = new CContainerUI;

    CLabelUI* pSep1 = new CLabelUI;
    pSep1->SetText(_T(":"));
    pSep1->SetFixedWidth(3);
    pSep1->SetTextPadding(CDuiRect(0, 1, 0, 1));
    CLabelUI* pSep2 = new CLabelUI;
    pSep2->SetText(_T(":"));
    pSep2->SetFixedWidth(3);
    pSep2->SetTextPadding(CDuiRect(0, 1, 0, 1));
    Add(m_pHour);
    Add(pSep1);
    Add(m_pMinute);
    Add(pSep2);
    Add(m_pSecond);
    Add(m_pPlaceHolder);
    Add(m_pSpin);

    m_pHour->SetNumberOnly(true);
    m_pHour->SetMaxChar(2);
    m_pMinute->SetNumberOnly(true);
    m_pMinute->SetMaxChar(2);
    m_pSecond->SetNumberOnly(true);
    m_pSecond->SetMaxChar(2);
    m_pSpin->SetHorizontal(false);

    m_pHour->SetTextPadding(CDuiRect(1, 1, 1, 1));
    m_pMinute->SetTextPadding(CDuiRect(1, 1, 1, 1));
    m_pSecond->SetTextPadding(CDuiRect(1, 1, 1, 1));

    m_pHour->SetPadding(CDuiRect(2, 1, 0, 1));
    m_pMinute->SetPadding(CDuiRect(0, 1, 0, 1));
    m_pSecond->SetPadding(CDuiRect(0, 1, 0, 1));

    m_pHour->SetTextStyle(DT_CENTER | DT_VCENTER);
    m_pMinute->SetTextStyle(DT_CENTER | DT_VCENTER);
    m_pSecond->SetTextStyle(DT_CENTER | DT_VCENTER);

    m_pSpin->SetRangeMax(m_dwMaxTime);
}

LPCTSTR CTimeSpinUI::GetClass() const
{
    return _T("TimeSpinUI");
}

UINT CTimeSpinUI::GetControlFlags() const
{
    return UIFLAG_TABSTOP;
}

LPVOID CTimeSpinUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, DUI_CTR_TIMESPIN) == 0 ) return static_cast<CTimeSpinUI*>(this);
    return CHorizontalLayoutUI::GetInterface(pstrName);
}

void CTimeSpinUI::DoInit()
{
    __super::DoInit();
    m_pSpin->SetBuddy(this);

    GetManager()->AddNotifier(this);

    SetButtonWidth(m_nBtnWidth);
}

void CTimeSpinUI::DoEvent(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_pParent != NULL ) m_pParent->DoEvent(event);
        else CHorizontalLayoutUI::DoEvent(event);
        return;
    }

    if( event.Type == UIEVENT_SETFOCUS ) 
    {
        Invalidate();
    }
    if( event.Type == UIEVENT_KILLFOCUS ) 
    {
        Invalidate();
    }
    if( event.Type == UIEVENT_BUTTONDOWN )
    {
        if( IsEnabled() ) {
            Activate();
            m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
        }
        return;
    }
    if( event.Type == UIEVENT_BUTTONUP )
    {
        if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
            m_uButtonState &= ~ UISTATE_CAPTURED;
            Invalidate();
        }
        return;
    }
    if( event.Type == UIEVENT_MOUSEMOVE )
    {
        return;
    }
    if( event.Type == UIEVENT_SCROLLWHEEL )
    {
        bool bDownward = LOWORD(event.wParam) == SB_LINEDOWN;
        if (GetManager()->GetFocus() == m_pHour)
        {
            if (bDownward)
            {
                SetHour(GetHour() - 1);
            }
            else
            {
                SetHour(GetHour() + 1);
            }
            return;
        }
        if (GetManager()->GetFocus() == m_pMinute)
        {
            if (bDownward)
            {
                SetMinute(GetMinute() - 1);
            }
            else
            {
                SetMinute(GetMinute() + 1);
            }
            return;
        }
        if (GetManager()->GetFocus() == m_pSecond)
        {
            if (bDownward)
            {
                SetSecond(GetSecond() - 1);
            }
            else
            {
                SetSecond(GetSecond() + 1);
            }
            return;
        }
    }
    if( event.Type == UIEVENT_KEYDOWN )
    {
        switch( event.chKey ) {
        case VK_TAB:
        case VK_RETURN:
            if (event.pSender == m_pHour)
            {
                m_pMinute->SetFocus();
            }
            else if (event.pSender == m_pMinute)
            {
                m_pSecond->SetFocus();
            }
            else if (event.pSender == m_pSecond)
            {
                GetManager()->SetNextTabControl();
            }
            return;
        }
    }
    if( event.Type == UIEVENT_MOUSEENTER && event.pSender == m_pSpin )
    {
        m_pSpin->DoEvent(event);
        return;
    }
    if( event.Type == UIEVENT_MOUSELEAVE && event.pSender == m_pSpin )
    {
        m_pSpin->DoEvent(event);
        return;
    }

    CHorizontalLayoutUI::DoEvent(event);
}

void CTimeSpinUI::Notify(TNotifyUI& msg)
{
    if (msg.sType == DUI_MSGTYPE_TEXTCHANGED)
    {
        if (msg.pSender == m_pHour)
        {
            CheckTime();
        }
        else if (msg.pSender == m_pMinute)
        {
            if (GetMinute() > 59)
            {
                SetMinute(0);
            }
            CheckTime();
        }
        else if (msg.pSender == m_pSecond)
        {
            if (GetSecond() > 59)
            {
                SetSecond(0);
            }
            CheckTime();
        }
    }
    else if (msg.sType == DUI_MSGTYPE_SPINCHANGED && msg.pSender == m_pSpin)
    {
        GetManager()->SendNotify(this, DUI_MSGTYPE_TIMECHANGED);
        SetEditStatus();
    }
    else if (msg.sType == DUI_MSGTYPE_SETFOCUS)
    {
        if (msg.pSender == m_pHour)
        {
            m_pSpin->SetInc(3600);
            m_nFocus = 0;
        }
        else if (msg.pSender == m_pMinute)
        {
            m_pSpin->SetInc(60);
            m_nFocus = 1;
        }
        else if (msg.pSender == m_pSecond)
        {
            m_pSpin->SetInc(1);
            m_nFocus = 2;
        }
    }
}

void CTimeSpinUI::CheckTime()
{
    DWORD dwTime = GetCurrentTime();
    if (dwTime > m_dwMaxTime)
    {
        SetCurrentTime(m_dwMaxTime);
    }
    else if (dwTime < m_dwMinTime)
    {
        SetCurrentTime(m_dwMinTime);
    }
    else
    {
        SetCurrentTime(dwTime);
    }
}

bool CTimeSpinUI::SetCurrentTime(DWORD dwTime)
{
//     if (GetCurrentTime() == dwTime)
//     {
//         return false;
//     }

    INT nHour = dwTime / 3600;
    INT nMinute = dwTime % 3600 /60;
    INT nSecond = dwTime % 60;

    return SetCurrentTime(nHour, nMinute, nSecond);
}

bool CTimeSpinUI::SetCurrentTime(UINT nHour, UINT nMinute, UINT nSecond)
{
    DWORD dwTime = 3600 * nHour + 60 * nMinute + nSecond;
    if (dwTime > m_dwMaxTime)
    {
        return false;
    }

    if (dwTime < m_dwMinTime)
    {
        return false;
    }

    m_dwTime = dwTime;
    m_pSpin->SetSpinPosSilent(dwTime);
//     SetEditStatus();

    if (GetHour() != nHour && *m_pHour->GetText())
    {
        TCHAR szTmp[64] = {0};
        _stprintf_s(szTmp, _countof(szTmp), _T("%02d"), nHour);
        m_pHour->SetText(szTmp);
    }

    if (GetMinute() != nMinute && *m_pMinute->GetText())
    {
        TCHAR szTmp[64] = {0};
        _stprintf_s(szTmp, _countof(szTmp), _T("%02d"), nMinute);
        m_pMinute->SetText(szTmp);
    }

    if (GetSecond() != nSecond && *m_pSecond->GetText())
    {
        TCHAR szTmp[64] = {0};
        _stprintf_s(szTmp, _countof(szTmp), _T("%02d"), nSecond);
        m_pSecond->SetText(szTmp);
    }

    return true;
}

DWORD CTimeSpinUI::GetCurrentTime()
{
    UINT nHour = (UINT)_ttoi(m_pHour->GetText());
    UINT nMinute = (UINT)_ttoi(m_pMinute->GetText());
    UINT nSecond = (UINT)_ttoi(m_pSecond->GetText());

    return nHour * 3600 + nMinute * 60 + nSecond;
}

UINT CTimeSpinUI::GetHour()
{
    return (UINT)_ttoi(m_pHour->GetText());
}

UINT CTimeSpinUI::GetMinute()
{
    return (UINT)_ttoi(m_pMinute->GetText());
}

UINT CTimeSpinUI::GetSecond()
{
    return (UINT)_ttoi(m_pSecond->GetText());
}

CDuiString CTimeSpinUI::GetTimeString()
{
    TCHAR szTmp[64] = {0};
    _stprintf_s(szTmp, _countof(szTmp), _T("%02d:%02d:%02d"), GetHour(), GetMinute(), GetSecond());

    return szTmp;
}

void CTimeSpinUI::SetTimeString(CDuiString timeStr)
{
    UINT hour= 0,min = 0,sec = 0;
	_stscanf_s(timeStr, _T("%02d:%02d:%02d"), &hour, &min, &sec);
    SetCurrentTime(hour,min,sec);
}
bool CTimeSpinUI::SetHour(UINT nHour)
{
    UINT nOldHour = GetHour();
    UINT nOldMinute = GetMinute();
    UINT nOldSecond = GetSecond();

    DWORD dwNewTime = nHour * 3600 + nOldMinute * 60 + nOldSecond;
    if (dwNewTime > m_dwMaxTime || dwNewTime < m_dwMinTime)
    {
        return false;
    }

    if (nHour != nOldHour && *m_pHour->GetText())
    {
        TCHAR szTmp[64] = {0};
        _stprintf_s(szTmp, _countof(szTmp), _T("%02d"), nHour);
        m_pHour->SetText(szTmp);
        m_dwTime = dwNewTime;
        m_pSpin->SetSpinPosSilent(dwNewTime);
        //SetEditStatus();
    }

    return true;
}

bool CTimeSpinUI::SetMinute(UINT nMinute)
{
    UINT nOldHour = GetHour();
    UINT nOldMinute = GetMinute();
    UINT nOldSecond = GetSecond();

    DWORD dwNewTime = nOldHour * 3600 + nMinute * 60 + nOldSecond;
    if (dwNewTime > m_dwMaxTime || dwNewTime < m_dwMinTime)
    {
        return false;
    }

    if (nMinute != nOldMinute && *m_pMinute->GetText())
    {
        TCHAR szTmp[64] = {0};
        _stprintf_s(szTmp, _countof(szTmp), _T("%02d"), nMinute);
        m_pMinute->SetText(szTmp);
        m_dwTime = dwNewTime;
        m_pSpin->SetSpinPosSilent(dwNewTime);
        //SetEditStatus();
    }

    return true;
}

bool CTimeSpinUI::SetSecond(UINT nSecond)
{
    UINT nOldHour = GetHour();
    UINT nOldMinute = GetMinute();
    UINT nOldSecond = GetSecond();

    DWORD dwNewTime = nOldHour * 3600 + nOldMinute * 60 + nSecond;
    if (dwNewTime > m_dwMaxTime || dwNewTime < m_dwMinTime)
    {
        return false;
    }

    if (nSecond != nOldSecond && *m_pSecond->GetText())
    {
        TCHAR szTmp[64] = {0};
        _stprintf_s(szTmp, _countof(szTmp), _T("%02d"), nSecond);
        m_pSecond->SetText(szTmp);
        m_dwTime = dwNewTime;
        m_pSpin->SetSpinPosSilent(dwNewTime);
        //SetEditStatus();
    }

    return true;
}

void CTimeSpinUI::SetMinTime(DWORD dwMinTime)
{
    m_dwMinTime = dwMinTime;
    m_pSpin->SetRangeMin(dwMinTime);
    if ( m_dwTime< dwMinTime)
    {
        SetCurrentTime(dwMinTime);
        m_pSpin->SetSpinPosSilent(dwMinTime);
    }
}

DWORD CTimeSpinUI::GetMinTime()
{
    return m_dwMinTime;
}

void CTimeSpinUI::SetMaxTime(DWORD dwMaxTime)
{
    m_dwMaxTime = dwMaxTime;
    m_pSpin->SetRangeMax(dwMaxTime);
    if (m_dwTime > dwMaxTime)
    {
        SetCurrentTime(dwMaxTime);
        m_pSpin->SetSpinPosSilent(dwMaxTime);
    }
}

DWORD CTimeSpinUI::GetMaxTime()
{
    return m_dwMaxTime;
}

void CTimeSpinUI::SetDelay(INT nDelay)
{
    m_pSpin->SetDelay(nDelay);
}

void CTimeSpinUI::SetSpeed(INT nSpeed)
{
    m_pSpin->SetSpeed(nSpeed);
}

void CTimeSpinUI::SetInc(INT nInc)
{
    m_pSpin->SetInc(nInc);
}

void CTimeSpinUI::SetButtonWidth(INT nButtonWidth)
{
    m_nBtnWidth = nButtonWidth;
    INT nWidth = GetFixedWidth();
    INT nEditWidth = (nWidth - m_nBtnWidth - 8) / 3;
    m_pHour->SetFixedWidth(nEditWidth);
    m_pMinute->SetFixedWidth(nEditWidth);
    m_pSecond->SetFixedWidth(nEditWidth);
    m_pSpin->SetFixedWidth(nButtonWidth);
    m_pPlaceHolder->SetFixedWidth(nWidth - 8 - 3 * nEditWidth - nButtonWidth);
    m_pPlaceHolder->SetPadding(CDuiRect(0, 1, 0, 1));
    m_pPlaceHolder->SetFixedHeight(1);
}

void CTimeSpinUI::SetText(LPCTSTR pstrText)
{
    SetCurrentTime(_ttoi(pstrText));

    if (!*m_pHour->GetText())
    {
        m_pHour->SetText(_T("00"));
    }
    if (!*m_pMinute->GetText())
    {
        m_pMinute->SetText(_T("00"));
    }
    if (!*m_pSecond->GetText())
    {
        m_pSecond->SetText(_T("00"));
    }
}

void CTimeSpinUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( _tcscmp(pstrName, _T("mintime")) == 0 )
    {
        SetMinTime(_ttoi(pstrValue));
    }
    else if( _tcscmp(pstrName, _T("maxtime")) == 0 )
    {
        SetMaxTime(_ttoi(pstrValue));
    }
    else if( _tcscmp(pstrName, _T("currenttime")) == 0 )
    {
        SetCurrentTime(_ttoi(pstrValue));
    }
#if 0
    else if( _tcscmp(pstrName, _T("buttonbothnormalimage")) == 0 ) m_pSpin->SetButtonBothNormalImage(pstrValue);
    else if( _tcscmp(pstrName, _T("buttonboth1hotimage")) == 0 ) m_pSpin->SetButtonBoth1HotImage(pstrValue);
    else if( _tcscmp(pstrName, _T("buttonboth1pushedimage")) == 0 ) m_pSpin->SetButtonBoth1PushedImage(pstrValue);
    else if( _tcscmp(pstrName, _T("buttonboth2hotimage")) == 0 ) m_pSpin->SetButtonBoth2HotImage(pstrValue);
    else if( _tcscmp(pstrName, _T("buttonboth2pushedimage")) == 0 ) m_pSpin->SetButtonBoth2PushedImage(pstrValue);
    else if( _tcscmp(pstrName, _T("buttonbothdisabledimage")) == 0 ) m_pSpin->SetButtonBothDisabledImage(pstrValue);
#endif
    else if( _tcscmp(pstrName, _T("button1normalimage")) == 0 ) m_pSpin->SetButton1NormalImage(pstrValue);
    else if( _tcscmp(pstrName, _T("button1hotimage")) == 0 ) m_pSpin->SetButton1HotImage(pstrValue);
    else if( _tcscmp(pstrName, _T("button1pushedimage")) == 0 ) m_pSpin->SetButton1PushedImage(pstrValue);
    else if( _tcscmp(pstrName, _T("button1disabledimage")) == 0 ) m_pSpin->SetButton1DisabledImage(pstrValue);
    else if( _tcscmp(pstrName, _T("button2normalimage")) == 0 ) m_pSpin->SetButton2NormalImage(pstrValue);
    else if( _tcscmp(pstrName, _T("button2hotimage")) == 0 ) m_pSpin->SetButton2HotImage(pstrValue);
    else if( _tcscmp(pstrName, _T("button2pushedimage")) == 0 ) m_pSpin->SetButton2PushedImage(pstrValue);
    else if( _tcscmp(pstrName, _T("button2disabledimage")) == 0 ) m_pSpin->SetButton2DisabledImage(pstrValue);
    else if( _tcscmp(pstrName, _T("delaytimes")) == 0 ) SetDelay(_ttoi(pstrValue));
    else if( _tcscmp(pstrName, _T("speed")) == 0) SetSpeed(_ttoi(pstrValue));
    else if( _tcscmp(pstrName, _T("inc")) == 0) SetInc(_ttoi(pstrValue));
#if 0
    else if( _tcscmp(pstrName, _T("bknormalimage")) == 0 ) m_pSpin->SetBkNormalImage(pstrValue);
    else if( _tcscmp(pstrName, _T("bkhotimage")) == 0 ) m_pSpin->SetBkHotImage(pstrValue);
    else if( _tcscmp(pstrName, _T("bkpushedimage")) == 0 ) m_pSpin->SetBkPushedImage(pstrValue);
    else if( _tcscmp(pstrName, _T("bkdisabledimage")) == 0 ) m_pSpin->SetBkDisabledImage(pstrValue);
#endif
    else if( _tcscmp(pstrName, _T("textpadding")) == 0 )
    {
        RECT rcTextPadding = { 0 };
        LPTSTR pstr = NULL;
        rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
        rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
        rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
        rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
        SetTextPadding(rcTextPadding);
    }
    else if( _tcscmp(pstrName, _T("font")) == 0 )
    {
        SetFont(_ttoi(pstrValue));
    }
    else if( _tcscmp(pstrName, _T("textcolor")) == 0 ) {
        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        SetTextColor(clrColor);
    }
    else if( _tcscmp(pstrName, _T("btnwidth")) == 0) SetButtonWidth(_ttoi(pstrValue));
	else if (!_tcscmp(pstrName, _T("imageleft")))  {
		m_diLeft.sDrawString = pstrValue;
	}
	else if (!_tcscmp(pstrName, _T("imagemid"))) m_diMid.sDrawString = pstrValue;
	else if (!_tcscmp(pstrName, _T("imageright"))) m_diRight.sDrawString = pstrValue;
    else CHorizontalLayoutUI::SetAttribute(pstrName, pstrValue);
}

void CTimeSpinUI::SetFont(int iFont)
{
    m_pHour->SetFont(iFont);
    m_pMinute->SetFont(iFont);
    m_pSecond->SetFont(iFont);
}
void CTimeSpinUI::SetTextPadding(RECT& rcTextPadding)
{
    m_pHour->SetTextPadding(rcTextPadding);
    m_pMinute->SetTextPadding(rcTextPadding);
    m_pSecond->SetTextPadding(rcTextPadding);
}

void CTimeSpinUI::SetTextColor(DWORD clrColor)
{
    m_pHour->SetTextColor(clrColor);
    m_pMinute->SetTextColor(clrColor);
    m_pSecond->SetTextColor(clrColor);
}
void CTimeSpinUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
	__super::DoPaint(hDC, rcPaint);
	if (!m_diLeft.sDrawString.IsEmpty()) {
		m_pHour->DrawImage(hDC, m_diLeft);
	}

	if (!m_diMid.sDrawString.IsEmpty()) {
		m_pHour->DrawImage(hDC, m_diMid);
	}

	if (!m_diRight.sDrawString.IsEmpty()) {
		m_pHour->DrawImage(hDC, m_diRight);
	}
}
void CTimeSpinUI::PaintBkImage(HDC hDC)
{
    __super::PaintBkImage(hDC);
}

void CTimeSpinUI::SetEditStatus()
{
    if (m_nFocus == 0)
    {
        if (m_pHour != GetManager()->GetFocus())
        {
            m_pHour->SetFocus();
            m_pHour->SetSelAll();
        }
    }
    else if (m_nFocus == 1)
    {
        if (m_pMinute != GetManager()->GetFocus())
        {
            m_pMinute->SetFocus();
            m_pMinute->SetSelAll();
        }
    }
    else if (m_nFocus == 2)
    {
        if (m_pSecond != GetManager()->GetFocus())
        {
            m_pSecond->SetFocus();
            m_pSecond->SetSelAll();
        }
    }
}

void CTimeSpinUI::DisableTimer(bool bDisable /* = true */)
{
    m_pSpin->DisableTimer(bDisable);
}

}