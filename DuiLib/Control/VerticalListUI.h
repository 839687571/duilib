#pragma once


#define MIN(a,b) (a <= b ? a : b)

///#include "UIlib.h"


namespace DuiLib {

	class CListBodyHorzUI;
	class CVertListHeaderUI;
	class CHorizListUI;

	class UILIB_API CVertListHeaderUI : public CListHeaderUI {
	public:
		CVertListHeaderUI()
		{

		}

		LPCTSTR GetClass() const
		{
			return _T("VertListHeaderUI");
		}


		SIZE EstimateSize(SIZE szAvailable)
		{
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
			SIZE cXY = { m_cxyFixed.cx, 0 };
			if (cXY.cx == 0 && m_pManager != NULL) {
				for (int it = 0; it < m_items.GetSize(); it++) {
					cXY.cx = MAX(cXY.cx, static_cast<CControlUI*>(m_items[it])->EstimateSize(szAvailable).cx);
				}
				int nMin = m_pManager->GetDefaultFontInfo()->tm.tmHeight + 6;
				cXY.cx = MAX(cXY.cx, nMin);
			}

			for (int it = 0; it < m_items.GetSize(); it++) {
				cXY.cy += static_cast<CControlUI*>(m_items[it])->EstimateSize(szAvailable).cy;
			}

			return cXY;
		}
	};

	class UILIB_API CListBodyHorzUI : public CHorizontalLayoutUI {
	public:
		CListBodyHorzUI()
		{

		}
		CListBodyHorzUI(CHorizListUI* pOwner) :m_pOwner(pOwner)
		{

		}
		CHorizListUI* m_pOwner;
	};

	class UILIB_API CHorizListUI : public CHorizontalLayoutUI, IListUI {
	public:
		CHorizListUI()
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
			m_ListInfo.dwSelectedBkColor = 0xFFC1E3FF;
			m_ListInfo.dwHotTextColor = 0xFF000000;
			m_ListInfo.dwHotBkColor = 0xFFE9F5FF;
			m_ListInfo.dwDisabledTextColor = 0xFFCCCCCC;
			m_ListInfo.dwDisabledBkColor = 0xFFFFFFFF;
			m_ListInfo.dwSelectedBorderColor = 0xFF3A8DED;
			m_ListInfo.dwLineColor = 0;
			m_ListInfo.bShowHtml = false;
			m_ListInfo.bMultiExpandable = false;

			::ZeroMemory(&m_ListInfo.rcTextPadding, sizeof(m_ListInfo.rcTextPadding));
			::ZeroMemory(&m_ListInfo.rcColumn, sizeof(m_ListInfo.rcColumn));
		}


		LPCTSTR GetClass() const
		{
			return _T("HorizListUI");
		}
		UINT GetControlFlags() const
		{
			return UIFLAG_TABSTOP;
		}
		LPVOID GetInterface(LPCTSTR pstrName)
		{
			if (_tcscmp(pstrName, DUI_CTR_HORIZLIST) == 0) return static_cast<CHorizListUI*>(this);
			if (_tcscmp(pstrName, _T("IList")) == 0) return static_cast<IListUI*>(this);
			if (_tcscmp(pstrName, _T("IListOwner")) == 0) return static_cast<IListOwnerUI*>(this);
			return CHorizontalLayoutUI::GetInterface(pstrName);
		}
		CControlUI* GetItemAt(int iIndex) const
		{
			return m_pList->GetItemAt(iIndex);
		}

		int GetItemIndex(CControlUI* pControl) const
		{
			if (pControl->GetInterface(_T("ListHeader")) != NULL) return CHorizontalLayoutUI::GetItemIndex(pControl);
			if (_tcsstr(pControl->GetClass(), _T("ListHeaderItemUI")) != NULL) return m_pHeader->GetItemIndex(pControl);

			return m_pList->GetItemIndex(pControl);
		}
		bool SetItemIndex(CControlUI* pControl, int iIndex)
		{
			if (pControl->GetInterface(_T("ListHeader")) != NULL) return CHorizontalLayoutUI::SetItemIndex(pControl, iIndex);
			// We also need to recognize header sub-items
			if (_tcsstr(pControl->GetClass(), _T("ListHeaderItemUI")) != NULL) return m_pHeader->SetItemIndex(pControl, iIndex);

			int iOrginIndex = m_pList->GetItemIndex(pControl);
			if (iOrginIndex == -1) return false;
			if (iOrginIndex == iIndex) return true;

			IListItemUI* pSelectedListItem = NULL;
			if (m_iCurSel >= 0) pSelectedListItem =
				static_cast<IListItemUI*>(GetItemAt(m_iCurSel)->GetInterface(_T("ListItem")));
			if (!m_pList->SetItemIndex(pControl, iIndex)) return false;
			int iMinIndex = min(iOrginIndex, iIndex);
			int iMaxIndex = max(iOrginIndex, iIndex);
			for (int i = iMinIndex; i < iMaxIndex + 1; ++i) {
				CControlUI* p = m_pList->GetItemAt(i);
				IListItemUI* pListItem = static_cast<IListItemUI*>(p->GetInterface(_T("ListItem")));
				if (pListItem != NULL) {
					pListItem->SetIndex(i);
				}
			}
			if (m_iCurSel >= 0 && pSelectedListItem != NULL) m_iCurSel = pSelectedListItem->GetIndex();
			return true;
		}
		bool Add(CControlUI* pControl)
		{
			if (pControl->GetInterface(_T("ListHeader")) != NULL) {
				if (m_pHeader != pControl && m_pHeader->GetCount() == 0) {
					CHorizontalLayoutUI::Remove(m_pHeader);
					m_pHeader = static_cast<CVertListHeaderUI*>(pControl);
				}
				m_ListInfo.nColumns = MIN(m_pHeader->GetCount(), UILIST_MAX_COLUMNS);
				return CHorizontalLayoutUI::AddAt(pControl, 0);
			}
			// We also need to recognize header sub-items
			if (_tcsstr(pControl->GetClass(), _T("ListHeaderItemUI")) != NULL) {
				bool ret = m_pHeader->Add(pControl);
				m_ListInfo.nColumns = MIN(m_pHeader->GetCount(), UILIST_MAX_COLUMNS);
				return ret;
			}
			IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
			if (pListItem != NULL) {
				pListItem->SetOwner(this);
				pListItem->SetIndex(GetCount());
			}
			return m_pList->Add(pControl);
		}
		bool AddAt(CControlUI* pControl, int iIndex)
		{
			// Override the AddAt() method so we can add items specifically to
			// the intended widgets. Headers and are assumed to be
			// answer the correct interface so we can add multiple list headers.
			if (pControl->GetInterface(_T("ListHeader")) != NULL) {
				if (m_pHeader != pControl && m_pHeader->GetCount() == 0) {
					CHorizontalLayoutUI::Remove(m_pHeader);
					m_pHeader = static_cast<CVertListHeaderUI*>(pControl);
				}
				m_ListInfo.nColumns = MIN(m_pHeader->GetCount(), UILIST_MAX_COLUMNS);
				return CHorizontalLayoutUI::AddAt(pControl, 0);
			}
			// We also need to recognize header sub-items
			if (_tcsstr(pControl->GetClass(), _T("ListHeaderItemUI")) != NULL) {
				bool ret = m_pHeader->AddAt(pControl, iIndex);
				m_ListInfo.nColumns = MIN(m_pHeader->GetCount(), UILIST_MAX_COLUMNS);
				return ret;
			}

			// The list items should know about us
			IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
			if (pListItem != NULL) {
				pListItem->SetOwner(this);
				pListItem->SetIndex(iIndex);
			}
			if (!m_pList->AddAt(pControl, iIndex)) return false;

			for (int i = iIndex + 1; i < m_pList->GetCount(); ++i) {
				CControlUI* p = m_pList->GetItemAt(i);
				pListItem = static_cast<IListItemUI*>(p->GetInterface(_T("ListItem")));
				if (pListItem != NULL) {
					pListItem->SetIndex(i);
				}
			}
			if (m_iCurSel >= iIndex) m_iCurSel += 1;
			return true;
		}
		bool Remove(CControlUI* pControl)
		{
			if (pControl->GetInterface(_T("ListHeader")) != NULL) return CHorizontalLayoutUI::Remove(pControl);
			// We also need to recognize header sub-items
			if (_tcsstr(pControl->GetClass(), _T("ListHeaderItemUI")) != NULL) return m_pHeader->Remove(pControl);

			int iIndex = m_pList->GetItemIndex(pControl);
			if (iIndex == -1) return false;

			if (!m_pList->RemoveAt(iIndex)) return false;

			for (int i = iIndex; i < m_pList->GetCount(); ++i) {
				CControlUI* p = m_pList->GetItemAt(i);
				IListItemUI* pListItem = static_cast<IListItemUI*>(p->GetInterface(_T("ListItem")));
				if (pListItem != NULL) {
					pListItem->SetIndex(i);
				}
			}

			if (iIndex == m_iCurSel && m_iCurSel >= 0) {
				int iSel = m_iCurSel, iSelActivate = m_iCurSelActivate;
				m_iCurSel = -1;
				m_iCurSelActivate = -1;
				SelectItem(FindSelectable(iSel, false));
				SelectItemActivate(FindSelectable(iSelActivate, false));
			} else if (iIndex < m_iCurSel) {
				m_iCurSel -= 1;
				m_iCurSelActivate = -1;
			}
			return true;
		}
		bool RemoveAt(int iIndex)
		{
			if (!m_pList->RemoveAt(iIndex)) return false;

			for (int i = iIndex; i < m_pList->GetCount(); ++i) {
				CControlUI* p = m_pList->GetItemAt(i);
				IListItemUI* pListItem = static_cast<IListItemUI*>(p->GetInterface(_T("ListItem")));
				if (pListItem != NULL) pListItem->SetIndex(i);
			}

			if (iIndex == m_iCurSel && m_iCurSel >= 0) {
				int iSel = m_iCurSel, iSelActivate = m_iCurSelActivate;
				m_iCurSel = -1;
				m_iCurSelActivate = -1;
				SelectItem(FindSelectable(iSel, false));
				SelectItemActivate(FindSelectable(iSelActivate, false));
			} else if (iIndex < m_iCurSel) {
				m_iCurSel -= 1;
				m_iCurSelActivate = -1;
			}

			return true;
		}

		void RemoveAll()
		{
			m_iCurSel = -1;
			m_iCurSelActivate = -1;
			m_iExpandedItem = -1;
			m_pList->RemoveAll();
		}
		void SetPos(RECT rc, bool bNeedInvalidate)
		{
			CHorizontalLayoutUI::SetPos(rc);
			if (m_pHeader == NULL) return;
			// Determine general list information and the size of header columns
			m_ListInfo.nColumns = MIN(m_pHeader->GetCount(), UILIST_MAX_COLUMNS);
			// The header/columns may or may not be visible at runtime. In either case
			// we should determine the correct dimensions...

			if (!m_pHeader->IsVisible()) {
				for (int it = 0; it < m_pHeader->GetCount(); it++) {
					static_cast<CControlUI*>(m_pHeader->GetItemAt(it))->SetInternVisible(true);
				}
				m_pHeader->SetPos(CDuiRect(rc.left, 0, rc.right, 0));
			}
			int iOffset = m_pList->GetScrollPos().cx;
			for (int i = 0; i < m_ListInfo.nColumns; i++) {
				CControlUI* pControl = static_cast<CControlUI*>(m_pHeader->GetItemAt(i));
				if (!pControl->IsVisible()) continue;
				if (pControl->IsFloat()) continue;

				RECT rcPos = pControl->GetPos();
				if (iOffset > 0) {
					rcPos.left -= iOffset;
					rcPos.right -= iOffset;
					pControl->SetPos(rcPos, false);
				}
				m_ListInfo.rcColumn[i] = pControl->GetPos();
			}
			if (!m_pHeader->IsVisible()) {
				for (int it = 0; it < m_pHeader->GetCount(); it++) {
					static_cast<CControlUI*>(m_pHeader->GetItemAt(it))->SetInternVisible(false);
				}
			}

		}

		void Move(SIZE szOffset, bool bNeedInvalidate)
		{
			CHorizontalLayoutUI::Move(szOffset, bNeedInvalidate);
			if (m_pHeader == NULL) return;
			if (!m_pHeader->IsVisible()) m_pHeader->Move(szOffset, false);
		}

		void DoEvent(TEventUI& event)
		{
			if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND) {
				if (m_pParent != NULL) m_pParent->DoEvent(event);
				else CHorizontalLayoutUI::DoEvent(event);
				return;
			}

			if (event.Type == UIEVENT_SETFOCUS) {
				m_bFocused = true;
				return;
			}
			if (event.Type == UIEVENT_KILLFOCUS) {
				m_bFocused = false;
				return;
			}

			switch (event.Type) {
			case UIEVENT_KEYDOWN:
				switch (event.chKey) {
				case VK_LEFT:
					SelectItem(FindSelectable(m_iCurSel - 1, false), true);
					return;
				case VK_RIGHT:
					SelectItem(FindSelectable(m_iCurSel + 1, true), true);
					return;
				case VK_PRIOR:
					PageUp();
					return;
				case VK_NEXT:
					PageDown();
					return;
				case VK_HOME:
					SelectItem(FindSelectable(0, false), true);
					return;
				case VK_END:
					SelectItem(FindSelectable(GetCount() - 1, true), true);
					return;
				case VK_RETURN:
					if (m_iCurSel != -1) GetItemAt(m_iCurSel)->Activate();
					return;
				}
				break;
			case UIEVENT_SCROLLWHEEL:
			{
				switch (LOWORD(event.wParam)) {
				case SB_LINEUP:
					//if (m_bScrollSelect) SelectItem(FindSelectable(m_iCurSel - 1, false), true);
					//else LineUp();
					return;
				case SB_LINEDOWN:
					//if (m_bScrollSelect) SelectItem(FindSelectable(m_iCurSel + 1, true), true);
					//else LineDown();
					return;
				}
			}
			break;
			}
			CHorizontalLayoutUI::DoEvent(event);
		}
		int GetCount() const
		{
			return m_pList->GetCount();
		}
		virtual CContainerUI* GetHeader() const
		{
			return (CContainerUI* )m_pHeader;
		}
		virtual CContainerUI* GetList() const
		{

			return (CContainerUI*)m_pList;
		}
		virtual IListCallbackUI* GetTextCallback() const
		{
			return NULL;
		}
		virtual void SetTextCallback(IListCallbackUI* pCallback)
		{

		}
		virtual bool ExpandItem(int iIndex, bool bExpand = true)
		{
			return false;
		}
		virtual int GetExpandedItem() const
		{

			return 0;
		}



		bool GetScrollSelect()
		{
			return m_bScrollSelect;
		}

		void SetScrollSelect(bool bScrollSelect)
		{
			m_bScrollSelect = bScrollSelect;
		}
		int GetCurSel() const
		{
			return m_iCurSel;
		}

		int GetCurSelActivate() const
		{
			return m_iCurSelActivate;
		}
		bool SelectItem(int iIndex, bool bTakeFocus = true)
		{
			if (iIndex == m_iCurSel) return true;

			int iOldSel = m_iCurSel;
			// We should first unselect the currently selected item
			if (m_iCurSel >= 0) {
				CControlUI* pControl = GetItemAt(m_iCurSel);
				if (pControl != NULL) {
					IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
					if (pListItem != NULL) pListItem->Select(false);
				}

				m_iCurSel = -1;
			}
			if (iIndex < 0) return false;

			CControlUI* pControl = GetItemAt(iIndex);
			if (pControl == NULL) return false;
			//if( !pControl->IsVisible() ) return false;
			//if( !pControl->IsEnabled() ) return false;

			IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
			if (pListItem == NULL) return false;
			///
			if (!pListItem->Select(true)) {
				m_iCurSel = -1;
				return false;
			}
			m_iCurSel = iIndex;
			EnsureVisible(m_iCurSel);
			if (bTakeFocus) pControl->SetFocus();
			if (m_pManager != NULL) {
				m_pManager->SendNotify(this, DUI_MSGTYPE_ITEMSELECT, m_iCurSel, iOldSel);
			}

			return true;
		}



		void EnsureVisible(int iIndex)
		{
			if (m_iCurSel < 0) return;
			RECT rcItem = m_pList->GetItemAt(iIndex)->GetPos();
			RECT rcList = m_pList->GetPos();
			RECT rcListInset = m_pList->GetInset();

			rcList.left += rcListInset.left;
			rcList.top += rcListInset.top;
			rcList.right -= rcListInset.right;
			rcList.bottom -= rcListInset.bottom;

			CScrollBarUI* pHorizontalScrollBar = m_pList->GetHorizontalScrollBar();
			if (pHorizontalScrollBar && pHorizontalScrollBar->IsVisible()) rcList.bottom -= pHorizontalScrollBar->GetFixedHeight();

			int iPos = m_pList->GetScrollPos().cx;
			if (rcItem.left >= rcList.left && rcItem.right < rcList.right) return;

			int dx = 0;
			if (rcItem.left < rcList.left) dx = rcItem.left - rcList.left;
			if (rcItem.right > rcList.right) dx = rcItem.right - rcList.right;

			Scroll(dx, 0);
		}

		bool SelectItemActivate(int iIndex)
		{
			if (!SelectItem(iIndex, true)) {
				return false;
			}

			m_iCurSelActivate = iIndex;
			return true;
		}

		TListInfoUI* GetListInfo()
		{
			return &m_ListInfo;
		}

		int GetChildPadding() const
		{
			return m_pList->GetChildPadding();
		}

		void SetChildPadding(int iPadding)
		{
			m_pList->SetChildPadding(iPadding);
		}

		void SetItemFont(int index)
		{
			m_ListInfo.nFont = index;
			NeedUpdate();
		}

		void SetItemTextStyle(UINT uStyle)
		{
			m_ListInfo.uTextStyle = uStyle;
			NeedUpdate();
		}

		void SetItemTextPadding(RECT rc)
		{
			m_ListInfo.rcTextPadding = rc;
			NeedUpdate();
		}

		RECT GetItemTextPadding() const
		{
			return m_ListInfo.rcTextPadding;
		}

		void SetItemTextColor(DWORD dwTextColor)
		{
			m_ListInfo.dwTextColor = dwTextColor;
			Invalidate();
		}

		void SetItemBkColor(DWORD dwBkColor)
		{
			m_ListInfo.dwBkColor = dwBkColor;
			Invalidate();
		}

		void SetItemBkImage(LPCTSTR pStrImage)
		{
			if (m_ListInfo.diBk.sDrawString == pStrImage && m_ListInfo.diBk.pImageInfo != NULL) return;
			m_ListInfo.diBk.Clear();
			m_ListInfo.diBk.sDrawString = pStrImage;
			Invalidate();
		}

		bool IsAlternateBk() const
		{
			return m_ListInfo.bAlternateBk;
		}

		void SetAlternateBk(bool bAlternateBk)
		{
			m_ListInfo.bAlternateBk = bAlternateBk;
			Invalidate();
		}

		DWORD GetItemTextColor() const
		{
			return m_ListInfo.dwTextColor;
		}

		DWORD GetItemBkColor() const
		{
			return m_ListInfo.dwBkColor;
		}

		LPCTSTR GetItemBkImage() const
		{
			return m_ListInfo.diBk.sDrawString;
		}

		void SetSelectedItemTextColor(DWORD dwTextColor)
		{
			m_ListInfo.dwSelectedTextColor = dwTextColor;
			Invalidate();
		}

		void SetSelectedItemBkColor(DWORD dwBkColor)
		{
			m_ListInfo.dwSelectedBkColor = dwBkColor;
			Invalidate();
		}

		void SetSelectedItemImage(LPCTSTR pStrImage)
		{
			if (m_ListInfo.diSelected.sDrawString == pStrImage && m_ListInfo.diSelected.pImageInfo != NULL) return;
			m_ListInfo.diSelected.Clear();
			m_ListInfo.diSelected.sDrawString = pStrImage;
			Invalidate();
		}

		DWORD GetSelectedItemTextColor() const
		{
			return m_ListInfo.dwSelectedTextColor;
		}

		DWORD GetSelectedItemBkColor() const
		{
			return m_ListInfo.dwSelectedBkColor;
		}

		LPCTSTR GetSelectedItemImage() const
		{
			return m_ListInfo.diSelected.sDrawString;
		}

		void SetHotItemTextColor(DWORD dwTextColor)
		{
			m_ListInfo.dwHotTextColor = dwTextColor;
			Invalidate();
		}

		void SetHotItemBkColor(DWORD dwBkColor)
		{
			m_ListInfo.dwHotBkColor = dwBkColor;
			Invalidate();
		}

		void SetHotItemImage(LPCTSTR pStrImage)
		{
			if (m_ListInfo.diHot.sDrawString == pStrImage && m_ListInfo.diHot.pImageInfo != NULL) return;
			m_ListInfo.diHot.Clear();
			m_ListInfo.diHot.sDrawString = pStrImage;
			Invalidate();
		}

		DWORD GetHotItemTextColor() const
		{
			return m_ListInfo.dwHotTextColor;
		}
		DWORD GetHotItemBkColor() const
		{
			return m_ListInfo.dwHotBkColor;
		}

		LPCTSTR GetHotItemImage() const
		{
			return m_ListInfo.diHot.sDrawString;
		}

		void Scroll(int dx, int dy)
		{
			if (dx == 0 && dy == 0) return;
			SIZE sz = m_pList->GetScrollPos();
			m_pList->SetScrollPos(CDuiSize(sz.cx + dx, sz.cy + dy));
		}
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
		{
			if (_tcscmp(pstrName, _T("header")) == 0) GetHeader()->SetVisible(_tcscmp(pstrValue, _T("hidden")) != 0);
			else if (_tcscmp(pstrName, _T("headerbkimage")) == 0) GetHeader()->SetBkImage(pstrValue);
			else if (_tcscmp(pstrName, _T("scrollselect")) == 0) SetScrollSelect(_tcscmp(pstrValue, _T("true")) == 0);
			///else if (_tcscmp(pstrName, _T("multiexpanding")) == 0) SetMultiExpanding(_tcscmp(pstrValue, _T("true")) == 0);
			else if (_tcscmp(pstrName, _T("itemfont")) == 0) m_ListInfo.nFont = _ttoi(pstrValue);
			else if (_tcscmp(pstrName, _T("itemalign")) == 0) {
				if (_tcsstr(pstrValue, _T("left")) != NULL) {
					m_ListInfo.uTextStyle &= ~(DT_CENTER | DT_RIGHT);
					m_ListInfo.uTextStyle |= DT_LEFT;
				}
				if (_tcsstr(pstrValue, _T("center")) != NULL) {
					m_ListInfo.uTextStyle &= ~(DT_LEFT | DT_RIGHT);
					m_ListInfo.uTextStyle |= DT_CENTER;
				}
				if (_tcsstr(pstrValue, _T("right")) != NULL) {
					m_ListInfo.uTextStyle &= ~(DT_LEFT | DT_CENTER);
					m_ListInfo.uTextStyle |= DT_RIGHT;
				}
			} else if (_tcscmp(pstrName, _T("itemendellipsis")) == 0) {
				if (_tcscmp(pstrValue, _T("true")) == 0) m_ListInfo.uTextStyle |= DT_END_ELLIPSIS;
				else m_ListInfo.uTextStyle &= ~DT_END_ELLIPSIS;
			} else if (_tcscmp(pstrName, _T("itemtextpadding")) == 0) {
				RECT rcTextPadding = { 0 };
				LPTSTR pstr = NULL;
				rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
				rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
				rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
				rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
				SetItemTextPadding(rcTextPadding);
			} else if (_tcscmp(pstrName, _T("itemtextcolor")) == 0) {
				if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
				LPTSTR pstr = NULL;
				DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
				SetItemTextColor(clrColor);
			} else if (_tcscmp(pstrName, _T("itembkcolor")) == 0) {
				if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
				LPTSTR pstr = NULL;
				DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
				SetItemBkColor(clrColor);
			} else if (_tcscmp(pstrName, _T("itembkimage")) == 0) SetItemBkImage(pstrValue);
			else if (_tcscmp(pstrName, _T("itemaltbk")) == 0) SetAlternateBk(_tcscmp(pstrValue, _T("true")) == 0);
			else if (_tcscmp(pstrName, _T("itemselectedtextcolor")) == 0) {
				if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
				LPTSTR pstr = NULL;
				DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
				SetSelectedItemTextColor(clrColor);
			} else if (_tcscmp(pstrName, _T("itemselectedbkcolor")) == 0) {
				if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
				LPTSTR pstr = NULL;
				DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
				SetSelectedItemBkColor(clrColor);
			} else if (_tcscmp(pstrName, _T("itemselectedimage")) == 0) SetSelectedItemImage(pstrValue);
			else if (_tcscmp(pstrName, _T("itemhottextcolor")) == 0) {
				if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
				LPTSTR pstr = NULL;
				DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
				SetHotItemTextColor(clrColor);
			} else if (_tcscmp(pstrName, _T("itemhotbkcolor")) == 0) {
				if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
				LPTSTR pstr = NULL;
				DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
				SetHotItemBkColor(clrColor);
			} else if (_tcscmp(pstrName, _T("itemhotimage")) == 0) SetHotItemImage(pstrValue);
			else if (_tcscmp(pstrName, _T("itemdisabledtextcolor")) == 0) {
				if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
				LPTSTR pstr = NULL;
				DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
				///SetDisabledItemTextColor(clrColor);
			} else if (_tcscmp(pstrName, _T("itemdisabledbkcolor")) == 0) {
				if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
				LPTSTR pstr = NULL;
				DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
				///SetDisabledItemBkColor(clrColor);
			}
			///else if (_tcscmp(pstrName, _T("itemdisabledimage")) == 0) SetDisabledItemImage(pstrValue);
			else if (_tcscmp(pstrName, _T("itemlinecolor")) == 0) {
				if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
				LPTSTR pstr = NULL;
				DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
				///SetItemLineColor(clrColor);
			}
			///else if (_tcscmp(pstrName, _T("itemshowhtml")) == 0) SetItemShowHtml(_tcscmp(pstrValue, _T("true")) == 0);
			else CHorizontalLayoutUI::SetAttribute(pstrName, pstrValue);
		}
		SIZE GetScrollPos() const
		{
			return m_pList->GetScrollPos();
		}

		SIZE GetScrollRange() const
		{
			return m_pList->GetScrollRange();
		}

		void SetScrollPos(SIZE szPos)
		{
			m_pList->SetScrollPos(szPos);
		}
		void LineUp()
		{
			m_pList->LineUp();
		}

		void LineDown()
		{
			m_pList->LineDown();
		}

		void PageUp()
		{
			m_pList->PageUp();
		}

		void PageDown()
		{
			m_pList->PageDown();
		}

		void HomeUp()
		{
			m_pList->HomeUp();
		}

		void EndDown()
		{
			m_pList->EndDown();
		}

		void LineLeft()
		{
			m_pList->LineLeft();
		}

		void LineRight()
		{
			m_pList->LineRight();
		}

		void PageLeft()
		{
			m_pList->PageLeft();
		}

		void PageRight()
		{
			m_pList->PageRight();
		}

		void HomeLeft()
		{
			m_pList->HomeLeft();
		}

		void EndRight()
		{
			m_pList->EndRight();
		}

		void EnableScrollBar(bool bEnableVertical, bool bEnableHorizontal)
		{
			m_pList->EnableScrollBar(bEnableVertical, bEnableHorizontal);
		}

		CScrollBarUI* GetVerticalScrollBar() const
		{
			return m_pList->GetVerticalScrollBar();
		}

		CScrollBarUI* GetHorizontalScrollBar() const
		{
			return m_pList->GetHorizontalScrollBar();
		}


		bool m_bScrollSelect;
		int m_iCurSel;
		int m_iCurSelActivate;
		int m_iExpandedItem;

		CListBodyHorzUI   *m_pList;
		CVertListHeaderUI     * m_pHeader;
		TListInfoUI m_ListInfo;

	};


};