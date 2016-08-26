#ifndef __UISLIDER_H__
#define __UISLIDER_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CSliderUI : public CProgressUI
	{
	public:
		CSliderUI();

		LPCTSTR GetClass() const;
		UINT GetControlFlags() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		int GetChangeStep();
		void SetChangeStep(int step);
		void SetThumbSize(SIZE szXY);
		RECT GetThumbRect() const;
		LPCTSTR GetThumbImage() const;
		void SetThumbImage(LPCTSTR pStrImage);
		LPCTSTR GetThumbHotImage() const;
		void SetThumbHotImage(LPCTSTR pStrImage);
		LPCTSTR GetThumbPushedImage() const;
		void SetThumbPushedImage(LPCTSTR pStrImage);
		void  SetThumbDisabledImage(LPCTSTR pStrImage);


		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void PaintStatusImage(HDC hDC);

	protected:
		SIZE m_szThumb;
		int m_nStep;

		TDrawInfo m_diThumb;
		TDrawInfo m_diThumbHot;
		TDrawInfo m_diThumbPushed;
		TDrawInfo m_diThumbDisabled;
	};
}

#endif // __UISLIDER_H__