#include "stdafx.h"
#include "UIAutoCompleteCombo.h"


CAutoCompleteComboUI::CAutoCompleteComboUI()
{
	m_pEdit      = NULL;
	m_iLeftWidth = 22;/* default size*/
	m_bBtnDrop   = FALSE;
	m_pHzToPy = new CHzToPyIner;
}


CAutoCompleteComboUI::~CAutoCompleteComboUI()
{

}

void CAutoCompleteComboUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_WINDOWINIT)	/* must get this msg */
	{
		CDuiRect rc = GetPos();
		rc.left = rc.left + 1;
		rc.right = rc.right - m_iLeftWidth;
		rc.top = rc.top + 1;
		rc.bottom = rc.bottom - 1;
		m_pEdit = new CEditUI();
		m_pEdit->SetFloat(true);
		CContainerUI *cont = (CContainerUI *)GetParent();
		cont->Add(m_pEdit);
		m_pEdit->SetPos(rc);
	}

	if (msg.pSender == m_pEdit)
	{
		OnEiditNotify(msg);
	}
	else
	{
		OnComboNotify(msg);
	}

	CDuiString str = msg.sType.GetData();

	printf("%s\n", str.GetData());
	OutputDebugString(str);
	OutputDebugString("\n");
	//printf("\n");
}

void CAutoCompleteComboUI::AddItemToCombo(LPCTSTR pText, LPCTSTR pUserData)
{
	if (m_pHzToPy == NULL) ASSERT(0);

	CListLabelElementUI* pItem = new CListLabelElementUI;
	pItem->SetText(pText);
	CDuiString TextPy = (m_pHzToPy->HzToPinYin(pText)).c_str();
	//m_itemPinyin.Insert(pText, (LPVOID)TextPy.GetData());
	itemPyMap[pText] = TextPy.GetData();
	pItem->SetUserData(pUserData);
	Add(pItem);
}

void CAutoCompleteComboUI::OnEiditNotify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_TEXTCHANGED)
	{
		CDuiString textEdit = ((CEditUI*)msg.pSender)->GetText();
		bool bSelect = false; /*默认选择 第一个匹配的选项*/

		for (int i = 0; i < GetCount(); i++)
		{
			CListLabelElementUI* pItem = (CListLabelElementUI*)GetItemAt(i);
			CDuiString text = pItem->GetText();
			CDuiString textPy = itemPyMap[text.GetData()].c_str();

			pItem->SetVisible(false);
			if (textPy.Find(textEdit) != -1)
			{
				pItem->SetVisible(true);
				if (!bSelect)
				{
					//SelectItem(i);
					bSelect = true;
				}
			}
		}
		Activate(FALSE);
		m_bBtnDrop = FALSE;
	}
	else if (msg.sType == DUI_MSGTYPE_RETURN)
	{
// 		int isel = m_pCombo->GetCurSel();
// 		CListLabelElementUI* pItem = (CListLabelElementUI*)m_pCombo->GetItemAt(isel);// CListLabelElementUI;
// 		CDuiString text = pItem->GetText();
// 		m_pEdit->SetText(text);
// 		m_pCombo->SelectItem(isel);
// 		m_pCombo->InActivate();
// 		::SetFocus(m_pm->GetPaintWindow());
	}
	else if (msg.sType == DUI_MSGTYPE_KILLFOCUS)
	{
		InActivate();
	}
}
void CAutoCompleteComboUI::OnComboNotify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_KILLFOCUS) /* 点击下拉 ,所以的item都显示出来.*/
	{
		for (int i = 0; i < GetCount(); i++)
		{
			CListLabelElementUI* pItem = (CListLabelElementUI*)GetItemAt(i);
			pItem->SetVisible(true);
		}
		m_bBtnDrop = TRUE;
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT) {
		{
			if (m_bBtnDrop || msg.pSender->IsFocused())
			{
				int isel = GetCurSel();
				CListLabelElementUI* pSelItem = static_cast<CListLabelElementUI*>(GetItemAt(isel));
				if (pSelItem != NULL)
				{
					CDuiString userdate = pSelItem->GetUserData().GetData();
					m_pEdit->SetUserData(userdate);
				}
				m_pEdit->SetText(msg.pSender->GetText());
			}
		}
	}
}

void CAutoCompleteComboUI::SetText(int iComboSel)
{
	if (iComboSel > GetCount())
	{
		iComboSel = 0;
	}
	CListLabelElementUI* pItem = (CListLabelElementUI*)GetItemAt(iComboSel);// CListLabelElementUI;
	if (pItem == NULL)
	{
		m_pEdit->SetText("");
		return;
	}
	CDuiString text = pItem->GetText();
	m_pEdit->SetText(text);
	m_pEdit->SetFocus();

	CListLabelElementUI* pSelItem = static_cast<CListLabelElementUI*>(GetItemAt(iComboSel));
	if (pSelItem != NULL)
	{
		CDuiString userdate = pSelItem->GetUserData().GetData();
		m_pEdit->SetUserData(userdate);
	}
}

CDuiString CAutoCompleteComboUI::GetEditText() 
{
	if (m_pEdit != NULL)
	{
		return m_pEdit->GetText();
	}
	return "";
}

void CAutoCompleteComboUI::SetEnabled(bool bEnable)
{
	__super::SetEnabled(bEnable);

	if (m_pEdit != NULL)
	{
		m_pEdit->SetEnabled(false);
	}
}

const CDuiString& CAutoCompleteComboUI::GetUserData()
{
	if (m_pEdit != NULL)
	{
		return m_pEdit->GetUserData();
	}
	return "";
}