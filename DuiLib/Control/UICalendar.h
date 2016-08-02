#pragma once
#include "stdafx.h"


//#include "systdate.h"

#include <ATLComTime.h>
namespace DuiLib {

	class CCalendarUI;

	class CCalendarWnd : public CWindowWnd, public INotifyUI {
	public:
		CCalendarWnd();

	private:
		~CCalendarWnd();

	public:

		LPCTSTR GetWindowClassName() const { return _T("CalendarWnd"); };

		UINT GetClassStyle() const { return CS_DBLCLKS; };

		void OnFinalMessage(HWND /*hWnd*/);
		void Init(CCalendarUI *pOwner);
		void SetWindPos(RECT &rc);
		void OnPrepare();
		void Notify(TNotifyUI& msg);
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void DrawCalendar(const SYSTEMTIME& time);
		void OnEditHour(CControlUI* pCtr);
		void OnEditMinute(CControlUI* pCtr);

		int GetDayCountInMonth(int iY, int iM) const;					// 指定月份的天数
		int GetDayCountInMonth(const SYSTEMTIME& sysTime) const
		{
			return GetDayCountInMonth(sysTime.wYear, sysTime.wMonth);
		}
		int GetDayOfWeekForMonthBegin(const SYSTEMTIME& sysTime, bool bSunStart = 0, bool bZeroStart = 0) const
		{
			SYSTEMTIME time = sysTime;
			time.wDay = 1;
			int dayOfWeek = COleDateTime(time).GetDayOfWeek();			// 1:sun, 2:Mon, 3:Tue
			dayOfWeek = (dayOfWeek == 1 && !bSunStart) ? 7 : dayOfWeek - 1;
			return dayOfWeek - bZeroStart;
		}
		SYSTEMTIME PrevMonth(const SYSTEMTIME& sysTime) const
		{
			SYSTEMTIME time = sysTime;
			time.wMonth = time.wMonth == 1 ? (time.wYear -= 1, 12) : time.wMonth - 1;
			time.wDay = GetDayCountInMonth(time.wYear, time.wMonth);
			return time;
		}
		SYSTEMTIME NextMonth(const SYSTEMTIME& sysTime) const
		{
			SYSTEMTIME time = sysTime;
			time.wMonth = time.wMonth == 12 ? (time.wYear += 1, 1) : time.wMonth + 1;
			time.wDay = GetDayCountInMonth(time.wYear, time.wMonth);
			return time;
		}
		SYSTEMTIME PrevHour(const SYSTEMTIME& sysTime) const
		{
			SYSTEMTIME time = sysTime;
			time.wHour = time.wHour == 0 ? 23 : time.wHour - 1;
			return time;
		}
		SYSTEMTIME NextHour(const SYSTEMTIME& sysTime) const
		{
			SYSTEMTIME time = sysTime;
			time.wHour = time.wHour == 23 ? 0 : time.wHour + 1;
			return time;
		}
		SYSTEMTIME PrevMinute(const SYSTEMTIME& sysTime) const
		{
			SYSTEMTIME time = sysTime;
			time.wMinute = time.wMinute == 0 ? 59 : time.wMinute - 1;
			return time;
		}
		SYSTEMTIME NextMinute(const SYSTEMTIME& sysTime) const
		{
			SYSTEMTIME time = sysTime;
			time.wMinute = time.wMinute == 59 ? 0 : time.wMinute + 1;
			return time;
		}
	private:
		CPaintManagerUI m_pm;
		SYSTEMTIME m_sysTime;

		CCalendarUI *m_pOwner;

	};


	class  UILIB_API CCalendarUI :public CHorizontalLayoutUI, INotifyUI {
	public:
		CCalendarUI();
		virtual ~CCalendarUI();

		friend class CCalendarWnd;

		LPCTSTR GetClass() const;
		void DoInit();

		CDuiString GetText() const;
		void SetShowH_M_S(bool, bool, bool);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void Notify(TNotifyUI& msg);

		void SetTime(SYSTEMTIME &time,bool notify = false);

		// format like:%Y年%m月%d日   %H:%M 2."%Y-%m-%d %H-%M-%S" "%Y-%m-%d %H:%M:%S"
		CDuiString GetTimeStr(const char *format = "%Y-%m-%d %H:%M:%S");

		void SetTimeStr(LPCTSTR str);

		SYSTEMTIME GetTime();

		bool StringToSYSTEMTIME(const char*lpszValue, SYSTEMTIME &time);

		SYSTEMTIME getYesterday(int dateType = 0/*0: 复用当前时分秒,1: 最早时间 2: 最晚时间*/);
		SYSTEMTIME getToday();
		

		void SetFont(int iFont);
		void SetText(LPCTSTR pstrText,bool notify = false);
		void SetTextColor(DWORD clrColor);
		void SetButtonNormalImage(LPCTSTR pStrImage);
		void SetButtonHotImage(LPCTSTR pStrImage);
		void SetButtonPushedImage(LPCTSTR pStrImage);
	private:

		int  m_iButtonWith;
		int  m_iButtonHeight;

		RECT  m_ButtonPadding;

		CLabelUI *m_pLable;
		CButtonUI *m_pButton;

		CCalendarWnd *m_pClalenderWnd;
	};

}