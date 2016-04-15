
#pragma once


#include "uicombo.h"
#include <map>
#include <string>

using namespace  DuiLib;

class CHzToPyIner;
class UILIB_API CAutoCompleteComboUI :
	public CComboUI
{

public:
	CAutoCompleteComboUI();
	~CAutoCompleteComboUI();

	void SetLeftWidth(unsigned int iWidth){ m_iLeftWidth = iWidth; }
	void AddItemToCombo(LPCTSTR pText, LPCTSTR pUserData);

	void AddFixCountItem(int count);
	void SetItemData(LPCTSTR pText, LPCTSTR pUserData, int index, bool visible = true);

	bool OnEiditNotify(void* pMsg);

	bool OnEiditNotifyEx(void* pMsg);
	
	bool OnComboNotify(void* pMsg);
	bool OnInitControl(void* pMsg);


	void SetPos(RECT rc,  bool bNeedInvalidate);

	CDuiString GetText() const;
	CDuiString GetEditText();
	void SetEditText(LPCTSTR pstrValue);

	void SetOutPinying(bool bOutPy);


	LPCTSTR GetComboSelectItemData();
	LPCTSTR GetComboSelectItemText();

	void SelectComboItemByUserData(LPCTSTR userData);

	/* ���µ�combui ����*/
	/*********************************************************/
	void SetEnabled(bool bEnable = true);
	virtual const CDuiString& GetUserData(); /* selected item userdata */
	virtual UINT GetControlFlags() const;
	virtual void SetFocus();
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	/*********************************************************/
	void SetTipValue(LPCTSTR pStrTipValue);
	LPCTSTR GetTipValue();
	void SetTipValueColor(LPCTSTR pStrColor);
	LPCTSTR GetTipValueColor();
	void SetTextColor(LPCTSTR pStrValue);
	DWORD GetTextColor();
private:
	void SetText(int iComboSel);
private:
	CEditUI *m_pEdit;
	unsigned int m_iLeftWidth;/* ���������Ԥ���Ĵ�С*/
	CDuiString  m_strBkcolorValue;
	int         m_iFont;

	// tip
	CDuiString m_sTipValue;
	CDuiString m_sSrcTipValue;
	CDuiString m_sTipValueColor;
	DWORD m_dwTextColor;
	
	/*
	����������
	*/
	CStdStringPtrMap  m_itemPinyin;/*pinyin*/

	std::map<std::string, std::string> itemPyMap;/*����ĸ*/
	CHzToPyIner              *m_pHzToPy;
	bool					m_bInited;
	bool                    m_OutPy; /*�ⲿ����ƴ��*/
};