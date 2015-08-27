#include "StdAfx.h"
#include "UITimeSpinMini.h"

namespace DuiLib
{

CTimeSpinMiniUI::CTimeSpinMiniUI()
{
    m_dwMaxTime = 359999999;
    m_nFocus = 3;
    InitInnerControl();
}

void CTimeSpinMiniUI::InitInnerControl()
{
    m_pMicroSecond = new CEditUI;
    CLabelUI* pSep3 = new CLabelUI;
    pSep3->SetText(_T("."));
    pSep3->SetFixedWidth(3);
    pSep3->SetTextPadding(CDuiRect(0, 1, 0, 1));
    AddAt(pSep3, 5);
    AddAt(m_pMicroSecond, 6);

    m_pMicroSecond->SetNumberOnly(true);
    m_pMicroSecond->SetMaxChar(3);
    m_pMicroSecond->SetTextPadding(CDuiRect(1, 1, 1, 1));
    m_pMicroSecond->SetTextStyle(DT_CENTER | DT_VCENTER);

    m_pHour->SetPadding(CDuiRect(2, 1, 0, 1));
    m_pMinute->SetPadding(CDuiRect(0, 1, 0, 1));
    m_pSecond->SetPadding(CDuiRect(0, 1, 0, 1));
    m_pMicroSecond->SetPadding(CDuiRect(0, 1, 0, 1));

    m_pSpin->SetRangeMax(m_dwMaxTime);
    m_pSpin->SetInc(100);
}

LPCTSTR CTimeSpinMiniUI::GetClass() const
{
    return _T("TimeSpinMiniUI");
}

LPVOID CTimeSpinMiniUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, DUI_CTR_TIMESPINMINI) == 0 ) return static_cast<CTimeSpinMiniUI*>(this);
    return CTimeSpinUI::GetInterface(pstrName);
}

void CTimeSpinMiniUI::DoEvent(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_pParent != NULL ) m_pParent->DoEvent(event);
        else CTimeSpinUI::DoEvent(event);
        return;
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
                m_pMicroSecond->SetFocus();
            }
            else if (event.pSender == m_pMicroSecond)
            {
                GetManager()->SetNextTabControl();
            }
            return;
        }
    }
    if( event.Type == UIEVENT_SCROLLWHEEL )
    {
        bool bDownward = LOWORD(event.wParam) == SB_LINEDOWN;
        if (GetManager()->GetFocus() == m_pMicroSecond)
        {
            if (bDownward)
            {
                SetMicroSecond(GetMicroSecond() - m_pSpin->GetInc());
            }
            else
            {
                SetMicroSecond(GetMicroSecond() + m_pSpin->GetInc());
            }
            return;
        }
    }

    __super::DoEvent(event);
}

void CTimeSpinMiniUI::Notify(TNotifyUI& msg)
{
    if (msg.sType == DUI_MSGTYPE_TEXTCHANGED)
    {
        if (msg.pSender == m_pMicroSecond)
        {
            if (GetMicroSecond() > 999)
            {
                SetMicroSecond(0);
            }
            CheckTime();
            return;
        }
    }
    else if (msg.sType == DUI_MSGTYPE_SETFOCUS)
    {
        if (msg.pSender == m_pHour)
        {
            m_pSpin->SetInc(3600000);
            m_nFocus = 0;
            return;
        }
        else if (msg.pSender == m_pMinute)
        {
            m_pSpin->SetInc(60000);
            m_nFocus = 1;
            return;
        }
        else if (msg.pSender == m_pSecond)
        {
            m_pSpin->SetInc(1000);
            m_nFocus = 2;
            return;
        }
        else if (msg.pSender == m_pMicroSecond)
        {
            m_pSpin->SetInc(100);
            m_nFocus = 3;
            return;
        }
    }
    __super::Notify(msg);
}

bool CTimeSpinMiniUI::SetCurrentTime(DWORD dwTime)
{
    INT nHour = dwTime / 3600000;
    INT nMinute = dwTime % 3600000 / 60000;
    INT nSecond = dwTime % 60000 / 1000;
    INT nMicroSecond = dwTime % 1000;

    return SetCurrentTime(nHour, nMinute, nSecond, nMicroSecond);
}

bool CTimeSpinMiniUI::SetCurrentTime(UINT nHour, UINT nMinute, UINT nSecond)
{
    return SetCurrentTime(nHour, nMinute, nSecond, 0);
}

bool CTimeSpinMiniUI::SetCurrentTime(UINT nHour, UINT nMinute, UINT nSecond, UINT nMicroSecond)
{
    DWORD dwTime = 3600000 * nHour + 60000 * nMinute + 1000 * nSecond + nMicroSecond;
    if (dwTime > m_dwMaxTime)
    {
        return false;
    }

    if (dwTime < m_dwMinTime)
    {
        return false;
    }

    m_dwTime = dwTime;
    m_pSpin->SetSpinPos(dwTime);
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

    if (GetMicroSecond() != nMicroSecond && *m_pMicroSecond->GetText())
    {
        TCHAR szTmp[64] = {0};
        _stprintf_s(szTmp, _countof(szTmp), _T("%03d"), nMicroSecond);
        m_pMicroSecond->SetText(szTmp);
    }

    return true;
}

DWORD CTimeSpinMiniUI::GetCurrentTime()
{
    UINT nHour = (UINT)_ttoi(m_pHour->GetText());
    UINT nMinute = (UINT)_ttoi(m_pMinute->GetText());
    UINT nSecond = (UINT)_ttoi(m_pSecond->GetText());
    UINT nMicroSecond = (UINT)_ttoi(m_pMicroSecond->GetText());

    return nHour * 3600000 + nMinute * 60000 + nSecond * 1000 + nMicroSecond;
}

UINT CTimeSpinMiniUI::GetMicroSecond()
{
    return (UINT)_ttoi(m_pMicroSecond->GetText());
}

CDuiString CTimeSpinMiniUI::GetTimeString()
{
    TCHAR szTmp[64] = {0};
    _stprintf_s(szTmp, _countof(szTmp), _T("%02d:%02d:%02d.%03d"), GetHour(), GetMinute(), GetSecond(), GetMicroSecond());

    return szTmp;
}

bool CTimeSpinMiniUI::SetHour(UINT nHour)
{
    UINT nOldHour = GetHour();
    UINT nOldMinute = GetMinute();
    UINT nOldSecond = GetSecond();
    UINT nOldMicroSecond = GetMicroSecond();

    DWORD dwNewTime = nHour * 3600000 + nOldMinute * 60000 + nOldSecond * 1000 + nOldMicroSecond;
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
        m_pSpin->SetSpinPos(dwNewTime);
        SetEditStatus();
    }

    return true;
}

bool CTimeSpinMiniUI::SetMinute(UINT nMinute)
{
    UINT nOldHour = GetHour();
    UINT nOldMinute = GetMinute();
    UINT nOldSecond = GetSecond();
    UINT nOldMicroSecond = GetMicroSecond();

    DWORD dwNewTime = nOldHour * 3600000 + nMinute * 60000 + nOldSecond * 1000 + nOldMicroSecond;
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
        m_pSpin->SetSpinPos(dwNewTime);
        SetEditStatus();
    }

    return true;
}

bool CTimeSpinMiniUI::SetSecond(UINT nSecond)
{
    UINT nOldHour = GetHour();
    UINT nOldMinute = GetMinute();
    UINT nOldSecond = GetSecond();
    UINT nOldMicroSecond = GetMicroSecond();

    DWORD dwNewTime = nOldHour * 3600000 + nOldMinute * 60000 + nSecond * 1000 + nOldMicroSecond;
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
        m_pSpin->SetSpinPos(dwNewTime);
        SetEditStatus();
    }

    return true;
}

bool CTimeSpinMiniUI::SetMicroSecond(UINT nMicroSecond)
{
    UINT nOldHour = GetHour();
    UINT nOldMinute = GetMinute();
    UINT nOldSecond = GetSecond();
    UINT nOldMicroSecond = GetMicroSecond();

    DWORD dwNewTime = nOldHour * 3600000 + nOldMinute * 60000 + nOldSecond * 1000 + nMicroSecond;
    if (dwNewTime > m_dwMaxTime || dwNewTime < m_dwMinTime)
    {
        return false;
    }

    if (nMicroSecond != nOldMicroSecond && *m_pMicroSecond->GetText())
    {
        TCHAR szTmp[64] = {0};
        _stprintf_s(szTmp, _countof(szTmp), _T("%03d"), nMicroSecond);
        m_pMicroSecond->SetText(szTmp);
        m_dwTime = dwNewTime;
        m_pSpin->SetSpinPos(dwNewTime);
        SetEditStatus();
    }

    return true;
}

void CTimeSpinMiniUI::SetButtonWidth(INT nButtonWidth)
{
    m_nBtnWidth = nButtonWidth;
    INT nWidth = GetFixedWidth();
    INT nEditWidth = (nWidth - m_nBtnWidth - 11 + 4) / 9;
    m_pHour->SetFixedWidth(nEditWidth * 2);
    m_pMinute->SetFixedWidth(nEditWidth * 2);
    m_pSecond->SetFixedWidth(nEditWidth * 2);
    m_pMicroSecond->SetFixedWidth(nWidth - 11 - 6 * nEditWidth - nButtonWidth);
    m_pPlaceHolder->SetFixedWidth(0);
    m_pPlaceHolder->SetPadding(CDuiRect(0, 1, 0, 1));
    m_pPlaceHolder->SetFixedHeight(1);
    m_pSpin->SetFixedWidth(nButtonWidth);
}

void CTimeSpinMiniUI::SetText(LPCTSTR pstrText)
{
    __super::SetText(pstrText);
    if (!*m_pMicroSecond->GetText())
    {
        m_pMicroSecond->SetText(_T("000"));
    }
}

void CTimeSpinMiniUI::SetFont(int iFont)
{
    __super::SetFont(iFont);
    m_pMicroSecond->SetFont(iFont);
}

void CTimeSpinMiniUI::SetTextPadding(RECT& rcTextPadding)
{
    __super::SetTextPadding(rcTextPadding);
    m_pMicroSecond->SetTextPadding(rcTextPadding);
}

void CTimeSpinMiniUI::SetTextColor(DWORD clrColor)
{
    __super::SetTextColor(clrColor);
    m_pMicroSecond->SetTextColor(clrColor);
}

void CTimeSpinMiniUI::SetEditStatus()
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
    else if (m_nFocus == 3)
    {
        if (m_pMicroSecond != GetManager()->GetFocus())
        {
            m_pMicroSecond->SetFocus();
            m_pMicroSecond->SetSelAll();
        }
    }
}

}