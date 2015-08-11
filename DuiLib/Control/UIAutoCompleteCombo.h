#pragma once


#include "uicombo.h"
#include <map>
#include <string>

using namespace  DuiLib;

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
		if (code >= start   &&   code <= end)
		{
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

class UILIB_API CAutoCompleteComboUI :
	public CComboUI
{

public:
	CAutoCompleteComboUI();
	~CAutoCompleteComboUI();

	virtual void Notify(TNotifyUI& msg);

	void SetLeftWidth(unsigned int iWidth){ m_iLeftWidth = iWidth; }
	void AddItemToCombo(LPCTSTR pText, LPCTSTR pUserData);

	void OnEiditNotify(TNotifyUI& msg);
	void OnComboNotify(TNotifyUI& msg);




	/* 重新的combui 方法*/
	/*********************************************************/
	CDuiString GetText() const;
	void SetEnabled(bool bEnable = true);
	virtual const CDuiString& GetUserData(); /* selected item userdata */
	/*********************************************************/
private:
	void SetText(int iComboSel);
private:
	CEditUI *m_pEdit;
	unsigned int m_iLeftWidth;/* 下来框左边预留的大小*/

	BOOL     m_bBtnDrop; /* 是否点击下拉框*/
	/*
	搜索的条件
	*/
	//CStdStringPtrMap  m_itemPinyin;/*pinyin*/

	std::map<std::string, std::string> itemPyMap;/*首字母*/
	CHzToPyIner           *m_pHzToPy;
};