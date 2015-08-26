#include "stdafx.h"
#include "UIAutoCompleteCombo.h"

/* 内部使用的类 汉字的拼音首字母
bug: 部分汉字 无法获取到拼音.
*/

class CHzToPyIner
{
public:
	CHzToPyIner(void)
	{
		notFondMap[0xDBDA] = 'z';/* 圳*/
		notFondMap[53671] = 'x'; /* 学*/
		notFondMap[55792] = 'y'; /* 兖*/
	}
	~CHzToPyIner(void)
	{}
	char convert(wchar_t n)
	{
		if (In(0xB0A1, 0xB0C4, n))   return   'a';
		if (In(0XB0C5, 0XB2C0, n))   return   'b';
		if (In(0xB2C1, 0xB4ED, n))   return   'c';
		if (In(0xB4EE, 0xB6E9, n))   return   'd';
		if (In(0xB6EA, 0xB7A1, n))   return   'e';
		if (In(0xB7A2, 0xB8c0, n))   return   'f';
		if (In(0xB8C1, 0xB9FD, n))   return   'g';
		if (In(0xB9FE, 0xBBF6, n))   return   'h';
		if (In(0xBBF7, 0xBFA5, n))   return   'j';
		if (In(0xBFA6, 0xC0AB, n))   return   'k';
		if (In(0xC0AC, 0xC2E7, n))   return   'l';
		if (In(0xC2E8, 0xC4C2, n))   return   'm';
		if (In(0xC4C3, 0xC5B5, n))   return   'n';
		if (In(0xC5B6, 0xC5BD, n))   return   'o';
		if (In(0xC5BE, 0xC6D9, n))   return   'p';
		if (In(0xC6DA, 0xC8BA, n))   return   'q';
		if (In(0xC8BB, 0xC8F5, n))   return   'r';
		if (In(0xC8F6, 0xCBF0, n))   return   's';
		if (In(0xCBFA, 0xCDD9, n))   return   't';
		if (In(0xCDDA, 0xCEF3, n))   return   'w';
		if (In(0xCEF4, 0xD188, n))   return   'x';
		if (In(0xD1B9, 0xD4D0, n))   return   'y';
		if (In(0xD4D1, 0xD7F9, n))   return   'z';
		return   IsInNotFondList(n);
	}
	bool In(wchar_t start, wchar_t end, wchar_t code)
	{
		if (code >= start   &&   code <= end) {
			return   true;
		}
		return   false;
	}
	char IsInNotFondList(wchar_t n)
	{
		std::map<wchar_t, char >::iterator iter;
		for (iter = notFondMap.begin(); iter != notFondMap.end(); iter++)
		{
			if (iter->first == n)
				return iter->second;
		}
		return 'x';
	}
	std::string HzToPinYin(std::string str)
	{

		//std::string   sChinese   =   “张三丰”;   //   输入的字符串
		char *psText;
#ifdef UNICODE

		DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, str.GetBuffer(0), -1, NULL, 0, NULL, FALSE);

		psText = new char[dwNum];
		if (!psText)
			delete[]psText;
		WideCharToMultiByte(CP_OEMCP, NULL, str.GetBuffer(0), -1, psText, dwNum, NULL, FALSE);
		ASSERT(0); /* 单字节情况下判断*/
#else
		//psText = str.c_str();
#endif
		std::string   sChinese = str;

		char   chr[3];
		wchar_t   wchr = 0;
		int leno = sChinese.length();
		int len = sChinese.length() /*/ 2*/;
		int bufLen = len * 2;
		char*   buff = new   char[bufLen];
		memset(buff, 0x00, bufLen);
		for (int i = 0, j = 0; j < (sChinese.length());)
		{
			char cChar = sChinese[j];
			if ((cChar >= 0 && cChar < 255) && isalnum(cChar)) /*为英文或者数字 0-9 a-z A-Z*/
			{
				buff[i] = sChinese[j];
				i++;
				j++;
				continue;
			}
			memset(chr, 0x00, sizeof(chr));
			chr[0] = sChinese[j++];
			chr[1] = sChinese[j++];
			chr[2] = '\0';

			//  单个字符的编码   如：’我’   =   0xced2
			//if (stricmp(chr, "圳") == 0) {
			//	buff[i] = 'z';
			//}
			//else {
			wchr = (chr[0] & 0xff) << 8;
			wchr |= (chr[1] & 0xff);
			buff[i] = convert(wchr);
			//}
			i++;
		}
		PinYin = buff;

		std::string strPy = buff;
		std::string outStr = "";
		for (size_t i = 0; i < strPy.length(); i++)
		{
			outStr += tolower(strPy[i]);
		}

		return  outStr.c_str();
	}
	std::string PinYin;
	std::map<wchar_t, char > notFondMap;
};

CAutoCompleteComboUI::CAutoCompleteComboUI()
{
	m_pEdit      = NULL;
	m_iLeftWidth = 22;/* default size*/
	m_pHzToPy = new CHzToPyIner;
	OnNotify += MakeDelegate(this, &CAutoCompleteComboUI::OnComboNotify);
	OnInit += MakeDelegate(this, &CAutoCompleteComboUI::OnInitControl);
}


CAutoCompleteComboUI::~CAutoCompleteComboUI()
{
	delete m_pHzToPy;
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

bool CAutoCompleteComboUI::OnEiditNotify(void* pMsg)
{
	TNotifyUI* pNotify = (TNotifyUI*)pMsg;
	if (pNotify->sType == DUI_MSGTYPE_TEXTCHANGED) {
		CDuiString textEdit = ((CEditUI*)pNotify->pSender)->GetText();
		bool bSelect = false; /*默认选择 第一个匹配的选项*/

		for (int i = 0; i < GetCount(); i++) {
			CListLabelElementUI* pItem = (CListLabelElementUI*)GetItemAt(i);
			CDuiString text = pItem->GetText();
			CDuiString textPy = itemPyMap[text.GetData()].c_str();

			pItem->SetVisible(false);
			if (textPy.Find(textEdit) != -1) {
				pItem->SetVisible(true);
				if (!bSelect) {
					//SelectItem(i);
					bSelect = true;
				}
			}
		}
		if (GetCount()>0){
			Activate(FALSE);
		}
	}
// 		int isel = m_pCombo->GetCurSel();
// 		CListLabelElementUI* pItem = (CListLabelElementUI*)m_pCombo->GetItemAt(isel);// CListLabelElementUI;
// 		CDuiString text = pItem->GetText();
// 		m_pEdit->SetText(text);
// 		m_pCombo->SelectItem(isel);
// 		m_pCombo->InActivate();
// 		::SetFocus(m_pm->GetPaintWindow());
	else if (pNotify->sType == DUI_MSGTYPE_KILLFOCUS) {
		InActivate();
	}
	return true;
}

bool CAutoCompleteComboUI::OnInitControl(void* pMsg)
{
	m_pEdit = new CEditUI();
	m_pEdit->SetFloat(true);
	CContainerUI *cont = (CContainerUI *)GetParent();
	cont->Add(m_pEdit);
	
	m_pEdit->OnNotify += MakeDelegate(this, &CAutoCompleteComboUI::OnEiditNotify);
	return true;
}

bool CAutoCompleteComboUI::OnComboNotify(void* pMsg)
{
	TNotifyUI* pNotify = (TNotifyUI*)pMsg;
	if (pNotify->sType == DUI_MSGTYPE_KILLFOCUS) /* 点击下拉 ,所以的item都显示出来.*/
	{
		for (int i = 0; i < GetCount(); i++)
		{
			CListLabelElementUI* pItem = (CListLabelElementUI*)GetItemAt(i);
			pItem->SetVisible(true);
		}
	}
	else if (pNotify->sType == DUI_MSGTYPE_ITEMSELECT) {
		int isel = GetCurSel();
		CListLabelElementUI* pSelItem = static_cast<CListLabelElementUI*>(GetItemAt(isel));
		if (pSelItem != NULL)
		{
			CDuiString userdate = pSelItem->GetUserData().GetData();
			m_pEdit->SetUserData(userdate);
		}
		m_pEdit->SetText(pNotify->pSender->GetText());
	}
	return true;
}

void CAutoCompleteComboUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	CComboUI::SetPos(rc, bNeedInvalidate);
	if (m_pEdit) {
		rc.left += 1;
		rc.right -= m_iLeftWidth;
		rc.top += 1;
		rc.bottom -= 1;
		m_pEdit->SetPos(rc, bNeedInvalidate);
	}
}

void CAutoCompleteComboUI::SetText(int iComboSel)
{
	if (iComboSel > GetCount()) {
		iComboSel = 0;
	}
	CListLabelElementUI* pItem = (CListLabelElementUI*)GetItemAt(iComboSel);// CListLabelElementUI;
	if (pItem == NULL) {
		m_pEdit->SetText("");
		return;
	}
	CDuiString text = pItem->GetText();
	m_pEdit->SetText(text);
	m_pEdit->SetFocus();

	CListLabelElementUI* pSelItem = static_cast<CListLabelElementUI*>(GetItemAt(iComboSel));
	if (pSelItem != NULL) {
		CDuiString userdate = pSelItem->GetUserData().GetData();
		m_pEdit->SetUserData(userdate);
	}
}

CDuiString CAutoCompleteComboUI::GetEditText() 
{
	if (m_pEdit != NULL) {
		return m_pEdit->GetText();
	}
	return "";
}

void CAutoCompleteComboUI::SetEnabled(bool bEnable)
{
	__super::SetEnabled(bEnable);

	if (m_pEdit != NULL) {
		m_pEdit->SetEnabled(false);
	}
} 
UINT CAutoCompleteComboUI::GetControlFlags() const
{
	//return UIFLAG_TABSTOP;
	return 0;
}
void CAutoCompleteComboUI::SetFocus()
{
	if (m_pEdit != NULL) {
		m_pEdit->SetFocus();
	}
}
const CDuiString& CAutoCompleteComboUI::GetUserData()
{
	if (m_pEdit != NULL) {
		return m_pEdit->GetUserData();
	}
	return "";
}