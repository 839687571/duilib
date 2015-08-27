#ifndef __UITIMESPIN_H__
#define __UITIMESPIN_H__

#pragma once

namespace DuiLib
{

class UILIB_API CTimeSpinUI : public CHorizontalLayoutUI, public INotifyUI
{
public:
    CTimeSpinUI();
	~CTimeSpinUI();
    LPCTSTR GetClass() const;
    UINT GetControlFlags() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    void DoInit();

    void DoEvent(TEventUI& event);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    void SetText(LPCTSTR pstrText);

    void Notify(TNotifyUI& msg);

    virtual bool SetCurrentTime(DWORD dwTime);
    virtual bool SetCurrentTime(UINT nHour, UINT nMinute, UINT nSecond);
    virtual DWORD GetCurrentTime();
    virtual UINT GetHour();
    virtual UINT GetMinute();
    virtual UINT GetSecond();
    virtual bool SetHour(UINT nHour);
    virtual bool SetMinute(UINT nMinute);
    virtual bool SetSecond(UINT nSecond);

    virtual void SetMinTime(DWORD dwMinTime);
    virtual void SetMaxTime(DWORD dwMaxTime);
    virtual DWORD GetMinTime();
    virtual DWORD GetMaxTime();
    void SetDelay(INT nDelay);
    void SetSpeed(INT nSpeed);
    void SetInc(INT nInc);
    void DisableTimer(bool bDisable = true);
    virtual void SetButtonWidth(INT nWidth);

	virtual CDuiString GetTimeString();
    virtual void InitInnerControl();

    virtual void SetFont(int iFont);
    virtual void SetTextPadding(RECT& rcTextPadding);
    virtual void SetTextColor(DWORD clrColor);
    virtual void PaintBkImage(HDC hDC);
	virtual void DoPaint(HDC hDC, const RECT& rcPaint);
	DWORD GetTime() { return m_dwTime; }
protected:
    virtual void CheckTime();
    virtual void SetEditStatus();

//     CDuiString m_strLeft;
//     CDuiString m_strMid;
//     CDuiString m_strRight;

	TDrawInfo m_diLeft;
	TDrawInfo m_diMid;
	TDrawInfo m_diRight;

    CEditUI* m_pHour;
    CEditUI* m_pMinute;
    CEditUI* m_pSecond;
    CSpinButtonUI* m_pSpin;
    CContainerUI* m_pPlaceHolder;

    DWORD m_dwTime;
    DWORD m_dwMinTime;
    DWORD m_dwMaxTime;

    INT m_nBtnWidth;
    INT m_nFocus;
};

}
#endif  // __UITIMESPIN_H__