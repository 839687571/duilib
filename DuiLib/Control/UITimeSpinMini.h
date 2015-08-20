#ifndef __UITIMESPINMINI_H__
#define __UITIMESPINMINI_H__

#pragma once

namespace DuiLib
{

class UILIB_API CTimeSpinMiniUI : public CTimeSpinUI
{
public:
    CTimeSpinMiniUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void DoEvent(TEventUI& event);
    void SetText(LPCTSTR pstrText);

    void Notify(TNotifyUI& msg);

    virtual bool SetCurrentTime(DWORD dwTime);
    virtual bool SetCurrentTime(UINT nHour, UINT nMinute, UINT nSecond);
    virtual bool SetCurrentTime(UINT nHour, UINT nMinute, UINT nSecond, UINT nMicroSecond);
    virtual DWORD GetCurrentTime();
    virtual UINT GetMicroSecond();
    virtual bool SetHour(UINT nHour);
    virtual bool SetMinute(UINT nMinute);
    virtual bool SetSecond(UINT nSecond);
    virtual bool SetMicroSecond(UINT nMicroSecond);

	virtual CDuiString GetTimeString();
    virtual void InitInnerControl();
    virtual void SetButtonWidth(INT nButtonWidth);
    virtual void SetFont(int iFont);
    virtual void SetTextPadding(RECT& rcTextPadding);
    virtual void SetTextColor(DWORD clrColor);

protected:
    virtual void SetEditStatus();

    CEditUI* m_pMicroSecond;
};

}
#endif  // __UITIMESPINMINI_H__