
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

	bool OnEiditNotify(void* pMsg);
	bool OnComboNotify(void* pMsg);
	bool OnInitControl(void* pMsg);


	void SetPos(RECT rc,  bool bNeedInvalidate);

	CDuiString GetText() const;
	CDuiString GetEditText();



	/* ���µ�combui ����*/
	/*********************************************************/
	void SetEnabled(bool bEnable = true);
	virtual const CDuiString& GetUserData(); /* selected item userdata */
	virtual UINT GetControlFlags() const;
	virtual void SetFocus();
	/*********************************************************/
private:
	void SetText(int iComboSel);
private:
	CEditUI *m_pEdit;
	unsigned int m_iLeftWidth;/* ���������Ԥ���Ĵ�С*/

	/*
	����������
	*/
	//CStdStringPtrMap  m_itemPinyin;/*pinyin*/

	std::map<std::string, std::string> itemPyMap;/*����ĸ*/
	CHzToPyIner           *m_pHzToPy;
	bool					m_bInited;
};