#ifndef __UIBUTTONGIF_H__
#define __UIBUTTONGIF_H__

#pragma once

#include "UIButton.h"
#include "../utils/GifHandler.h"

namespace DuiLib
{
	class UILIB_API CGifButtonUI : public CButtonUI
	{
	public:	
		CGifButtonUI();
		~CGifButtonUI();

		LPCTSTR GetClass() const
		{
			return _T("GifButtonUI");
		}

		LPVOID GetInterface( LPCTSTR pstrName )
		{
			if( _tcscmp(pstrName, _T("GifButton")) == 0 ) 
				return static_cast<CGifButtonUI*>(this);

			return CControlUI::GetInterface(pstrName);
		}

		void SetNormalGifFile(LPCTSTR pstrName);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		virtual void DoEvent(TEventUI& event);
		virtual void SetVisible(bool bVisible = true);//设置控件可视性
		virtual void PaintStatusImage(HDC hDC);

	protected:
		enum
		{
			GIF_TIMER_ID = 15
		};

		bool m_isUpdateTime;
		CGifHandler* m_pGif;
		int m_nPreUpdateDelay;

	};
}// namespace DuiLib

#endif // __UIBUTTONGIF_H__