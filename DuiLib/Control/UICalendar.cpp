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
namespace DuiLib {

	CCalendarWnd::CCalendarWnd() :m_pOwner(NULL)
	{
	}


	CCalendarWnd::~CCalendarWnd()
	{
	}

	void CCalendarWnd::OnPrepare()
	{
		if (m_pOwner == NULL) {
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
		else if (msg.sType == DUI_MSGTYPE_RETURN) {
			DrawCalendar(m_sysTime);
		} else if (msg.sType == DUI_MSGTYPE_KILLFOCUS) {
			if (msg.pSender->GetName() == _T("editHour")) {
				OnEditHour(msg.pSender);
				DrawCalendar(m_sysTime);
			} else if (msg.pSender->GetName() == _T("editMinute")) {
				OnEditMinute(msg.pSender);
				DrawCalendar(m_sysTime);
			}
		} else if (msg.sType == DUI_MSGTYPE_CLICK) {
			if (msg.pSender->GetName() == _T("btnPrevMonth")) {
				m_sysTime = PrevMonth(m_sysTime);
				DrawCalendar(m_sysTime);
			} else if (msg.pSender->GetName() == _T("btnNextMonth")) {
				m_sysTime = NextMonth(m_sysTime);
				DrawCalendar(m_sysTime);
			} else if (msg.pSender->GetName() == _T("btnDay")) {
				m_sysTime.wDay = atoi(msg.pSender->GetText());
				DrawCalendar(m_sysTime);
			} else if (msg.pSender->GetName() == _T("btnPrevHour")) {
				m_sysTime = PrevHour(m_sysTime);
				DrawCalendar(m_sysTime);
			} else if (msg.pSender->GetName() == _T("btnNextHour")) {
				m_sysTime = NextHour(m_sysTime);
				DrawCalendar(m_sysTime);
			} else if (msg.pSender->GetName() == _T("btnPrevMinue")) {
				m_sysTime = PrevMinute(m_sysTime);
				DrawCalendar(m_sysTime);
			} else if (msg.pSender->GetName() == _T("btnNextMinue")) {
				m_sysTime = NextMinute(m_sysTime);
				DrawCalendar(m_sysTime);
			} else if (msg.pSender->GetName() == _T("btnConfirm") || msg.pSender->GetName() == _T("btnDateTimePicker")) {
                m_pOwner->SetTime(m_sysTime,true);
			}
		}

	}

	/*
	RECT rcOwner = pOwner->GetPos();
	RECT rc = rcOwner;
	rc.top = rc.bottom;
	Create(pOwner->GetManager()->GetPaintWindow(), NULL, WS_CHILD, WS_EX_TOOLWINDOW, rc);
	*/
	void CCalendarWnd::Init(CCalendarUI *pOwner)
	{
		m_pOwner = pOwner;
		CDuiString timeStr = m_pOwner->GetText();
		m_pOwner->StringToSYSTEMTIME(timeStr.GetData(), m_sysTime);

		RECT rcOwner = pOwner->GetPos();
		RECT rc = rcOwner;
		rc.top = rc.bottom;

		POINT curPos;
		curPos.x = rc.left;
		curPos.y = rc.top;
		::ClientToScreen(pOwner->GetManager()->GetPaintWindow(), &curPos);
		rc.left = curPos.x;
		rc.top = curPos.y;


		Create(pOwner->GetManager()->GetPaintWindow(), NULL, /*WS_CHILD*/WS_POPUP, WS_EX_TOOLWINDOW, rc);
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
			m_pm.AddNotifier(this);
			return 0;
		}
         else if (uMsg == WM_KILLFOCUS) {

			POINT pt = { 0 };
			::GetCursorPos(&pt);
			::ScreenToClient(m_pm.GetPaintWindow(), &pt);
			CControlUI* pControl = m_pm.FindControl(pt);
			if (pControl == NULL) {
				printf("-----------------------post close \n");
				PostMessage(WM_CLOSE);
				return 0;
			}

		}
		LRESULT lRes = 0;
		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}
	void CCalendarWnd::OnFinalMessage(HWND /*hWnd*/)
	{
		m_pOwner->m_pClalenderWnd = NULL;
		m_pOwner->Invalidate();
		delete this;
		printf("CCalendarWnd is finnish \n");
	}
	//绘制日历

	void CCalendarWnd::DrawCalendar(const SYSTEMTIME& time)
	{
		CControlUI* pCtrl = 0;
		if (pCtrl = m_pm.FindControl(_T("lblYearMonth")))
			pCtrl->SetText(COleDateTime(time).Format(_T("%Y年%m月")));

		if (pCtrl = m_pm.FindControl(_T("lblDateTime")))
			pCtrl->SetText(COleDateTime(time).Format(_T("%Y年%m月%d日   %H:%M")));

		int iDayOfCurrMonth = 0;
		int iDayOfNextMonth = 0;
		char cDay[3] = { 0 };

		int iStartDay = GetDayOfWeekForMonthBegin(time);								//本月开始
		int iDayOfPrevMonth = GetDayCountInMonth(PrevMonth(time)) - iStartDay;			//上月开始
		int nDayCount = GetDayCountInMonth(time);										//本月天数

		CContainerUI* pContainer = static_cast<CContainerUI*>(m_pm.FindControl(_T("ContainerUI3")));
		for (int i = 0; i < 42; i++) {
			pCtrl = m_pm.FindSubControlByClass(pContainer, _T("ButtonUI"), i);
			pCtrl->SetEnabled(false);
			pCtrl->SetBkColor(pCtrl->GetParent()->GetBkColor());

			if (i < iStartDay) {
				iDayOfPrevMonth++;
				sprintf(cDay, "%d", iDayOfPrevMonth);
				pCtrl->SetText(cDay);
				pCtrl->SetEnabled(false);
			} else if (i > iStartDay - 1 && iDayOfCurrMonth < nDayCount) {
				iDayOfCurrMonth++;
				sprintf(cDay, "%d", iDayOfCurrMonth);
				pCtrl->SetText(cDay);
				pCtrl->SetEnabled(true);
			} else {
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
			//闰年可以被4或者400整除 但是不能被100整除
			if (iY % 4 == 0 && iY % 100 != 0 || iY % 400 == 0)
				iTotalDay = 29;
			else
				iTotalDay = 28;
		}

		else if (iM == 4 || iM == 6 || iM == 9 || iM == 11)
			iTotalDay = 30;
		return iTotalDay;

	}



	CCalendarUI::CCalendarUI()
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

     CCalendarUI::~CCalendarUI()
     {
            GetManager()->RemoveNotifier(this);
     }
	LPCTSTR CCalendarUI::GetClass() const
	{
		return _T("CalenderUI");
	}
	void CCalendarUI::DoInit()
	{
		__super::DoInit();

		printf("this  = %d name=%s\n", this, GetName().GetData());
		GetManager()->AddNotifier(this);/* need fix */

		m_pButton->SetFixedHeight(m_iButtonHeight);
		m_pButton->SetFixedWidth(m_iButtonWith);
		m_pButton->SetPadding(m_ButtonPadding);
		SYSTEMTIME tm;
		::GetLocalTime(&tm);
		SetTime(tm);
	}

	CDuiString CCalendarUI::GetText() const
	{
		return m_pLable->GetText();
	}
	void CCalendarUI::SetShowH_M_S(bool, bool, bool)
	{

	}
	void CCalendarUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcscmp(pstrName, _T("font")) == 0) SetFont(_ttoi(pstrValue));
		else if (_tcscmp(pstrName, _T("text")) == 0) SetText(pstrValue);

		else if (_tcscmp(pstrName, _T("buttonnormalimage")) == 0) SetButtonNormalImage(pstrValue);
		else if (_tcscmp(pstrName, _T("buttonhotimage")) == 0) SetButtonHotImage(pstrValue);
		else if (_tcscmp(pstrName, _T("buttonpushedimage")) == 0) SetButtonPushedImage(pstrValue);
		else if (_tcscmp(pstrName, _T("textcolor")) == 0) {
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetTextColor(clrColor);
		} else CHorizontalLayoutUI::SetAttribute(pstrName, pstrValue);

	}
	void CCalendarUI::Notify(TNotifyUI& msg)
	{
		if (m_pButton == msg.pSender && msg.sType == DUI_MSGTYPE_CLICK) {
			if (m_pClalenderWnd == NULL) {
				m_pClalenderWnd = new CCalendarWnd;
				m_pClalenderWnd->Init(this);

			}
			return;
		}
	}

	void CCalendarUI::SetTime(SYSTEMTIME &time,bool notify)
	{
		if(m_pClalenderWnd){
			//FIX ME: can not get destroy msg will popup a window
			// and get destroy msg after popup window destroyed
			m_pClalenderWnd->ShowWindow(FALSE);
            m_pClalenderWnd->Close();
            m_pClalenderWnd = NULL;
	    }
		SetText(COleDateTime(time).Format(_T("%Y年%m月%d日   %H:%M")),notify);
		printf("----------SetTime return ;\n");
	}

	SYSTEMTIME CCalendarUI::GetTime()
	{
		SYSTEMTIME tm;
		StringToSYSTEMTIME(m_pLable->GetText().GetData(), tm);
		return tm;
	}

	CDuiString CCalendarUI::GetTimeStr(const char *format)
	{
		SYSTEMTIME tm;
		StringToSYSTEMTIME(m_pLable->GetText().GetData(), tm);
		CString p3 = COleDateTime(tm).Format("%Y-%m-%d %H:%M:%S");

		return p3.GetBuffer();

	}
	void CCalendarUI::SetTimeStr(LPCTSTR str)
	{   
        SYSTEMTIME tm;
		StringToSYSTEMTIME(str, tm);
		SetTime(tm,false);
	}
	bool  CCalendarUI::StringToSYSTEMTIME(const char*lpszValue, SYSTEMTIME &time)
	{
		if (!lpszValue) {
			return FALSE;
		}

		COleDateTime tT;
		tT.ParseDateTime(lpszValue);
		tT.GetAsSystemTime(time);

		return TRUE;
	}

	SYSTEMTIME CCalendarUI::getYesterday(int dateType)
	{
		SYSTEMTIME st;
		time_t   yest;
		tm *tm1;
		yest = time(NULL) - (time_t)(60 * 60 * 24);
		tm1 = gmtime(&yest);
		st.wYear = tm1->tm_year + 1900;
		st.wMonth = tm1->tm_mon + 1;
		st.wDay = tm1->tm_mday;
		switch (dateType) {
		case 0:
		{
				  st.wHour = tm1->tm_hour + 8;
				  if (st.wHour >= 24){
			       // st.wDay = tm1->tm_mday+1;
                	st.wHour = st.wHour-23;
				  }

				  st.wMinute = tm1->tm_min;
				  st.wSecond = tm1->tm_sec;
				  break;
		}
		case 1:
		{
				  st.wHour = 0;
				  st.wMinute = 0;
				  st.wSecond = 0;
				  break;
		}

		case 2:
		{
				  st.wHour = 23;
				  st.wMinute = 59;
				  st.wSecond = 59;
				  break;
		}
			
		default:
		break;
		}

		return st;
	}

	SYSTEMTIME CCalendarUI::getToday()
    {
    	SYSTEMTIME st;
    	time_t   today;
    	tm *tm1;
    	today = time(NULL) ;
    	tm1 = gmtime(&today);
    	st.wYear = tm1->tm_year + 1900;
    	st.wMonth = tm1->tm_mon + 1;
    	st.wDay = tm1->tm_mday;
    	st.wHour = tm1->tm_hour+ 8; 
    	st.wMinute = tm1->tm_min;
    	st.wSecond = tm1->tm_sec;
    	return st;
    }
	void CCalendarUI::SetFont(int iFont)
	{
		m_pLable->SetFont(iFont);
	}
	void CCalendarUI::SetText(LPCTSTR pstrText,bool notify)
	{
		CDuiString oldText = m_pLable->GetText();
		m_pLable->SetText(pstrText);
		if (notify && oldText != pstrText) {
			GetManager()->SendNotify(this, DUI_MSGTYPE_TEXTCHANGED,0,0,true);
		}
	}
	void CCalendarUI::SetTextColor(DWORD clrColor)
	{
		m_pLable->SetTextColor(clrColor);
	}
	void CCalendarUI::SetButtonNormalImage(LPCTSTR pStrImage)
	{
		m_pButton->SetNormalImage(pStrImage);
	}
	void CCalendarUI::SetButtonHotImage(LPCTSTR pStrImage)
	{
		m_pButton->SetHotImage(pStrImage);
	}
	void CCalendarUI::SetButtonPushedImage(LPCTSTR pStrImage)
	{
		m_pButton->SetPushedImage(pStrImage);
	}
}