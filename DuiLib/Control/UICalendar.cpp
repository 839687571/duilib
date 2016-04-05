#include "stdafx.h"
#include "UICalendar.h"

//////////////////////////////////////////////////////////////////////////
// Format a time string: 
//		https://msdn.microsoft.com/zh-cn/library/fe06s4ak.aspx
// SYSTEMTIME structure:
//		https://msdn.microsoft.com/library/windows/desktop/ms724950
// COleDateTime Class:
//		https://msdn.microsoft.com/zh-cn/library/38wh24td.aspx
// COleDateTimeSpan Class:
//		https://msdn.microsoft.com/zh-cn/library/xb7yw6f3.aspx
//////////////////////////////////////////////////////////////////////////


CCalendarWnd::CCalendarWnd() :m_pOwner(NULL)
{
}


CCalendarWnd::~CCalendarWnd()
{
}

void CCalendarWnd::OnPrepare()
{
    if(m_pOwner == NULL)
    {
        ::GetLocalTime(&m_sysTime);
		
    }
	DrawCalendar(m_sysTime);
}

void CCalendarWnd::OnEditHour(CControlUI* pCtrl)
{
	int h = atoi(pCtrl->GetText());
	m_sysTime.wHour = (0 <= h && h <= 23) ? h : m_sysTime.wHour;
}

void CCalendarWnd::OnEditMinute(CControlUI* pCtrl)
{
	int m = atoi(pCtrl->GetText());
	m_sysTime.wMinute = (0 <= m && m <= 59) ? m : m_sysTime.wMinute;
}

void CCalendarWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
		OnPrepare();
	else if (msg.sType == DUI_MSGTYPE_RETURN)
	{
		DrawCalendar(m_sysTime);
	} 
	else if (msg.sType == DUI_MSGTYPE_KILLFOCUS)
	{
		if (msg.pSender->GetName() == _T("editHour")) {
			OnEditHour(msg.pSender);
			DrawCalendar(m_sysTime);
		}
		else if (msg.pSender->GetName() == _T("editMinute")) {
			OnEditMinute(msg.pSender);
			DrawCalendar(m_sysTime);
		}
	}
	else if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == _T("btnPrevMonth")) {
			m_sysTime = PrevMonth(m_sysTime);
			DrawCalendar(m_sysTime);
		}
		else if (msg.pSender->GetName() == _T("btnNextMonth")) {
			m_sysTime = NextMonth(m_sysTime);
			DrawCalendar(m_sysTime);
		}
		else if (msg.pSender->GetName() == _T("btnDay")) {
			m_sysTime.wDay = atoi(msg.pSender->GetText());
			DrawCalendar(m_sysTime);
		}
		else if (msg.pSender->GetName() == _T("btnPrevHour")) {
			m_sysTime = PrevHour(m_sysTime);
			DrawCalendar(m_sysTime);
		}
		else if (msg.pSender->GetName() == _T("btnNextHour")) {
			m_sysTime = NextHour(m_sysTime);
			DrawCalendar(m_sysTime);
		}
		else if (msg.pSender->GetName() == _T("btnPrevMinue")) {
			m_sysTime = PrevMinute(m_sysTime);
			DrawCalendar(m_sysTime);
		}
		else if (msg.pSender->GetName() == _T("btnNextMinue")) {
			m_sysTime = NextMinute(m_sysTime);
			DrawCalendar(m_sysTime);
		}
		else if (msg.pSender->GetName() == _T("btnConfirm") || msg.pSender->GetName() == _T("btnDateTimePicker") ) {
			CControlUI* pCtrl = 0;
			m_pOwner->SetTime(m_sysTime);
			PostMessage(WM_CLOSE);
            
		}
	}
}

void CCalendarWnd::Init(CCalenderUI *pOwner)
{
	m_pOwner = pOwner;
	CDuiString timeStr = m_pOwner->GetText();
	m_pOwner->StringToSYSTEMTIME(timeStr.GetData(), m_sysTime);
	//DrawCalendar(m_sysTime);
	
	RECT rcOwner = pOwner->GetPos();
	RECT rc = rcOwner;
	rc.top = rc.bottom;		
	Create(pOwner->GetManager()->GetPaintWindow(), NULL, WS_CHILD, WS_EX_TOOLWINDOW, rc);
	ShowWindow(TRUE);
	SetFocus(m_hWnd);

}
void  CCalendarWnd::SetWindPos(RECT &rc)
{
	RECT rect;

	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, w, h, SWP_NOZORDER | SWP_NOACTIVATE);

}
LRESULT CCalendarWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE) {
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		m_pm.Init(m_hWnd);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(_T("Calendar.xml"), (UINT)0, NULL, &m_pm);
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);
		//m_pm.AddPreMessageFilter(this);
		m_pm.AddNotifier(this);
		return 0;
	}

	else if (uMsg == WM_DESTROY) {
		//::PostQuitMessage(0L);
	} else if (uMsg == WM_ERASEBKGND) {
		//return 1;
	}else if (uMsg == WM_CLOSE)
	{
		printf("close \n");
	}else if (uMsg == WM_KILLFOCUS)
	{
		POINT pt = { 0 };
		::GetCursorPos(&pt);
		::ScreenToClient(m_pm.GetPaintWindow(), &pt);
		CControlUI* pControl = m_pm.FindControl(pt);
		if (pControl == NULL)
		{
			PostMessage(WM_CLOSE);
		}
	}else if (uMsg == WM_LBUTTONUP)
	{
		printf("btn up \n");
	}
	LRESULT lRes = 0;
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}
void CCalendarWnd::OnFinalMessage(HWND /*hWnd*/)
{
	m_pOwner->m_pClalenderWnd = NULL;
	delete this;
	printf("finnal \n");
}
//��������

void CCalendarWnd::DrawCalendar(const SYSTEMTIME& time)
{
	CControlUI* pCtrl = 0;
	if (pCtrl = m_pm.FindControl(_T("lblYearMonth")))
		pCtrl->SetText(COleDateTime(time).Format(_T("%Y��%m��")));

	if (pCtrl = m_pm.FindControl(_T("lblDateTime")))
		pCtrl->SetText(COleDateTime(time).Format(_T("%Y��%m��%d��   %H:%M")));

	int iDayOfCurrMonth = 0;
	int iDayOfNextMonth = 0;
	char cDay[3] = { 0 };

	int iStartDay = GetDayOfWeekForMonthBegin(time);								//���¿�ʼ
	int iDayOfPrevMonth = GetDayCountInMonth(PrevMonth(time)) - iStartDay;			//���¿�ʼ
	int nDayCount = GetDayCountInMonth(time);										//��������

	CContainerUI* pContainer = static_cast<CContainerUI*>(m_pm.FindControl(_T("ContainerUI3")));
	for (int i = 0; i < 42; i++) {
		pCtrl = m_pm.FindSubControlByClass(pContainer, _T("ButtonUI"), i);
		pCtrl->SetEnabled(false);
		pCtrl->SetBkColor(pCtrl->GetParent()->GetBkColor());

		if (i < iStartDay) 
		{
			iDayOfPrevMonth++;
			sprintf(cDay, "%d", iDayOfPrevMonth);
			pCtrl->SetText(cDay);
			pCtrl->SetEnabled(false);
		} 
		else if (i > iStartDay - 1 && iDayOfCurrMonth < nDayCount) 
		{
			iDayOfCurrMonth++;
			sprintf(cDay, "%d", iDayOfCurrMonth);
			pCtrl->SetText(cDay);
			pCtrl->SetEnabled(true);
		} 
		else 
		{
			iDayOfNextMonth++;
			sprintf(cDay, "%d", iDayOfNextMonth);
			pCtrl->SetText(cDay);
			pCtrl->SetEnabled(false);
		}
	}

	pCtrl = m_pm.FindSubControlByClass(pContainer, _T("ButtonUI"), iStartDay + time.wDay - 1);
	pCtrl->SetBkColor(0xFF3C3E4A);

	if (pCtrl = m_pm.FindControl(_T("editHour"))) {
		pCtrl->SetText(COleDateTime(time).Format(_T("%H")));
	}
	if (pCtrl = m_pm.FindControl(_T("editMinute"))) {
		pCtrl->SetText(COleDateTime(time).Format(_T("%M")));
	}
}

int CCalendarWnd::GetDayCountInMonth(int iY, int iM) const
{
	int iTotalDay = 31;

	if (iM == 2) {
		//������Ա�4����400���� ���ǲ��ܱ�100����
		if (iY % 4 == 0 && iY % 100 != 0 || iY % 400 == 0)
			iTotalDay = 29;
		else
			iTotalDay = 28;
	}

	else if (iM == 4 || iM == 6 || iM == 9 || iM == 11)
		iTotalDay = 30;
	return iTotalDay;

}



CCalenderUI::CCalenderUI()
{
	m_pLable = new CLabelUI;
	m_pButton = new CButtonUI;

	m_pClalenderWnd = NULL;

	m_iButtonWith = 18;
	m_iButtonHeight = 18; 
	m_ButtonPadding = { 0, 6, 6, 0 };

	m_pLable->SetTextPadding({ 5, 0, 0, 0 });
	Add(m_pLable);
	Add(m_pButton);
}

// CCalenderUI::~CCalenderUI()
// {
// }
LPCTSTR CCalenderUI::GetClass() const
{
	return _T("CalenderUI");
}
void CCalenderUI::DoInit()
{
	__super::DoInit();

	GetManager()->AddNotifier(this);
	m_pButton->SetFixedHeight(m_iButtonHeight);
	m_pButton->SetFixedWidth(m_iButtonWith);
	m_pButton->SetPadding(m_ButtonPadding);
	SYSTEMTIME tm;
	::GetLocalTime(&tm);
	SetTime(tm);
}

CDuiString CCalenderUI::GetText() const
{
	return m_pLable->GetText();
}
void CCalenderUI::SetShowH_M_S(bool, bool, bool)
{

}
void CCalenderUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcscmp(pstrName, _T("font")) == 0) SetFont(_ttoi(pstrValue));
	else if (_tcscmp(pstrName, _T("text")) == 0) SetText(pstrValue);
	//else if (_tcscmp(pstrName, _T("width")) == 0) SetFixedWidth(_ttoi(pstrValue));
	//else if (_tcscmp(pstrName, _T("height")) == 0) SetFixedHeight(_ttoi(pstrValue));

	else if (_tcscmp(pstrName, _T("buttonnormalimage")) == 0) SetButtonNormalImage(pstrValue);
	else if (_tcscmp(pstrName, _T("buttonhotimage")) == 0) SetButtonHotImage(pstrValue);
	else if (_tcscmp(pstrName, _T("buttonpushedimage")) == 0) SetButtonPushedImage(pstrValue);
	else if (_tcscmp(pstrName, _T("textcolor")) == 0) {
		if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetTextColor(clrColor);
	}
	else CHorizontalLayoutUI::SetAttribute(pstrName, pstrValue);
		
} 
void CCalenderUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK) {
		if (m_pClalenderWnd == NULL)
		{
			m_pClalenderWnd = new CCalendarWnd;
			m_pClalenderWnd->Init(this); 

		}
		return;
	} else if (msg.sType == DUI_MSGTYPE_KILLFOCUS) {
		printf("kill focus \n");
		return;
	}
}

void CCalenderUI::SetTime(SYSTEMTIME &time)
{
	m_pLable->SetText(COleDateTime(time).Format(_T("%Y��%m��%d��   %H:%M")));
}

LPCTSTR CCalenderUI::GetTimeStr(const char *format)
{
	SYSTEMTIME tm;
	StringToSYSTEMTIME(m_pLable->GetText().GetData(), tm);
	return COleDateTime(tm).Format(format);
}
bool  CCalenderUI::StringToSYSTEMTIME(const char*lpszValue, SYSTEMTIME &time)
{
	if (!lpszValue) {
		return FALSE;
	}

	COleDateTime tT;
	tT.ParseDateTime(lpszValue);
	tT.GetAsSystemTime(time);

	return TRUE;
}
void CCalenderUI::SetFont(int iFont)
{
	m_pLable->SetFont(iFont);
}
void CCalenderUI::SetText(LPCTSTR pstrText)
{
	m_pLable->SetText(pstrText);
}
void CCalenderUI::SetTextColor(DWORD clrColor)
{
	m_pLable->SetTextColor(clrColor);
}
void CCalenderUI::SetButtonNormalImage(LPCTSTR pStrImage)
{
	m_pButton->SetNormalImage(pStrImage);
}
void CCalenderUI::SetButtonHotImage(LPCTSTR pStrImage)
{
	m_pButton->SetHotImage(pStrImage);
}
void CCalenderUI::SetButtonPushedImage(LPCTSTR pStrImage)
{
	m_pButton->SetPushedImage(pStrImage);
}