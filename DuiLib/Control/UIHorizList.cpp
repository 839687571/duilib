
#include "StdAfx.h"
#include "UIHorizList.h"



namespace DuiLib {
	CHorizListUI::CHorizListUI()
	{
		m_pList = new CListBodyHorzUI(this);
		m_pHeader = new CVertListHeaderUI;
		m_pHeader->SetName("listheader");


		Add(m_pHeader);
		CHorizontalLayoutUI::Add(m_pList);

		m_ListInfo.nColumns = 0;
		m_ListInfo.nFont = 1;
		m_ListInfo.uTextStyle = DT_VCENTER; // m_uTextStyle(DT_VCENTER | DT_END_ELLIPSIS)
		m_ListInfo.dwTextColor = 0xFF000000;
		m_ListInfo.dwBkColor = 0;
		m_ListInfo.bAlternateBk = false;
		m_ListInfo.dwSelectedTextColor = 0xFF000000;
		m_ListInfo.dwSelectedBkColor = 0xFFffff00;
		m_ListInfo.dwHotTextColor = 0xFF000000;
		m_ListInfo.dwHotBkColor = 0xFFff00ff;
		m_ListInfo.dwDisabledTextColor = 0xFFCCCCCC;
		m_ListInfo.dwDisabledBkColor = 0xFFFFFFFF;
		///m_ListInfo.dwSelectedBorderColor = 0xFF3A8DED;
		m_ListInfo.dwSelectedBorderColor = 0;
		m_ListInfo.dwLineColor = 0;
		m_ListInfo.bShowHtml = false;
		m_ListInfo.bMultiExpandable = false;

		::ZeroMemory(&m_ListInfo.rcTextPadding, sizeof(m_ListInfo.rcTextPadding));
		::ZeroMemory(&m_ListInfo.rcColumn, sizeof(m_ListInfo.rcColumn));
	}


	CHorizListUI::~CHorizListUI()
	{

	}
}