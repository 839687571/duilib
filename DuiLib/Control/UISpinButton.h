#ifndef __UISPINBUTTON_H__
#define __UISPINBUTTON_H__

#pragma once

namespace DuiLib
{
    class UILIB_API CSpinButtonUI : public CControlUI
    {
    public:
        CSpinButtonUI();

        LPCTSTR GetClass() const;
        LPVOID GetInterface(LPCTSTR pstrName);

        void SetVisible(bool bVisible = true);
        void SetEnabled(bool bEnable = true);
        void SetFocus();

        bool IsHorizontal();
        void SetHorizontal(bool bHorizontal = true);
        int GetRangeMin() const;
        int GetRangeMax() const;
        void SetRangeMax(int nMax);
        void SetRangeMin(int nMin);
        void SetRange(int nMin, int nMax);
        int GetSpinPos() const;
        void SetSpinPos(int nPos);
        void SetSpinPosSilent(int nPos);
        void SetSpeed(int nMillisec);
        int GetSpeed();
        void SetInc(int nInc);
        int GetInc();
        void SetBuddy(CControlUI* pControl);
        CControlUI* GetBuddy();
        void SetDelay(int nTimes);
        int GetDelay();
        void SetPos(RECT rc,bool bNeedInvalidate = true );
        void ToLoseFocus();
        void DisableTimer(bool bDisable = true);

        LPCTSTR GetButton1NormalImage();
        void SetButton1NormalImage(LPCTSTR pStrImage);
        LPCTSTR GetButton1HotImage();
        void SetButton1HotImage(LPCTSTR pStrImage);
        LPCTSTR GetButton1PushedImage();
        void SetButton1PushedImage(LPCTSTR pStrImage);
        LPCTSTR GetButton1DisabledImage();
        void SetButton1DisabledImage(LPCTSTR pStrImage);

        LPCTSTR GetButton2NormalImage();
        void SetButton2NormalImage(LPCTSTR pStrImage);
        LPCTSTR GetButton2HotImage();
        void SetButton2HotImage(LPCTSTR pStrImage);
        LPCTSTR GetButton2PushedImage();
        void SetButton2PushedImage(LPCTSTR pStrImage);
        LPCTSTR GetButton2DisabledImage();
        void SetButton2DisabledImage(LPCTSTR pStrImage);

//         LPCTSTR GetButtonBothNormalImage();
//         void SetButtonBothNormalImage(LPCTSTR pStrImage);
//         LPCTSTR GetButtonBoth1HotImage();
//         void SetButtonBoth1HotImage(LPCTSTR pStrImage);
//         LPCTSTR GetButtonBoth1PushedImage();
//         void SetButtonBoth1PushedImage(LPCTSTR pStrImage);
//         LPCTSTR GetButtonBoth2HotImage();
//         void SetButtonBoth2HotImage(LPCTSTR pStrImage);
//         LPCTSTR GetButtonBoth2PushedImage();
//         void SetButtonBoth2PushedImage(LPCTSTR pStrImage);
//         LPCTSTR GetButtonBothDisabledImage();
//         void SetButtonBothDisabledImage(LPCTSTR pStrImage);

//         LPCTSTR GetBkNormalImage();
//         void SetBkNormalImage(LPCTSTR pStrImage);
//         LPCTSTR GetBkHotImage();
//         void SetBkHotImage(LPCTSTR pStrImage);
//         LPCTSTR GetBkPushedImage();
//         void SetBkPushedImage(LPCTSTR pStrImage);
//         LPCTSTR GetBkDisabledImage();
//         void SetBkDisabledImage(LPCTSTR pStrImage);

        void DoEvent(TEventUI& event);
        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

        void DoPaint(HDC hDC, const RECT& rcPaint);

        void PaintBk(HDC hDC);
        void PaintBothButton(HDC hDC);
        void PaintButton1(HDC hDC);
        void PaintButton2(HDC hDC);

    protected:

        enum
        { 
            DEFAULT_TIMERID = 11,
            DEFAULT_INC = 1,
            DEFAULT_SPEED = 50,
            DEFAULT_DELAY_TIMES = 5,
        };

        int m_nTime;
        int m_nInc;
        CControlUI* m_pControl;

        bool m_bHorizontal;
        int m_nRangeMax;
        int m_nRangeMin;
        int m_nScrollPos;
        int m_nScrollRepeatDelay;
        int m_nDelayTimes;

		//对于该控件 设置btn1 btn2 的各种属性即可满足需要.
		// 另有需要 可在扩展.
       /*  CDuiString m_sBkNormalImage;
         CDuiString m_sBkHotImage;
         CDuiString m_sBkPushedImage;
         CDuiString m_sBkDisabledImage;
		CDuiString m_sButtonBothNormalImage;
		CDuiString m_sButtonBoth1HotImage;
		CDuiString m_sButtonBoth1PushedImage;
		CDuiString m_sButtonBothDisabledImage;
		CDuiString m_sButtonBoth2HotImage;
		CDuiString m_sButtonBoth2PushedImage;
	    CDuiString m_sImageModify;*/

        RECT m_rcButton1;
        UINT m_uButton1State;

		TDrawInfo  m_diBtn1Normal;
		TDrawInfo  m_diBtn1Hot;
		TDrawInfo  m_diBtn1Pushed;
		TDrawInfo  m_diBtn1Disabled;

        RECT m_rcButton2;
        UINT m_uButton2State;

		TDrawInfo  m_diBtn2Normal;
		TDrawInfo  m_diBtn2Hot;
		TDrawInfo  m_diBtn2Pushed;
		TDrawInfo  m_diBtn2Disabled;

        bool m_bDisableTimer;
    };
}

#endif // __UISPINBUTTON_H__