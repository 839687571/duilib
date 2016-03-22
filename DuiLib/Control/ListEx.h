
#pragma once
#ifndef LISTEX_H
#define LISTEX_H



/*#include "../duilib.h"*/
#include <map>

#include "UIlib.h"
namespace DuiLib {

	class UILIB_API CListUIEx : public CListUI
	{
	public:
		/**
		 * ���캯��
		 */
		CListUIEx();

		/**
		 * ��������
		 */
		virtual ~CListUIEx();

		/**
		 * ������,�վ�,��0��ʼ
		 * 
		 * @param	nCol					����,��Ҫ��0��ʼ���β���
		 * @param	lpszColumnHeaderStr		ÿһ�б���
		 * @param	uTextStyle				�������ֶ��뷽ʽ
		 * @param	nWidth					�п�
		 * 
		 * @return	����TrueΪ�ɹ�,FalseΪʧ��
		 */
		BOOL InsertColumn(
			int nCol,
			LPCTSTR lpszColumnHeaderStr,
			UINT uTextStyle = DT_LEFT,
			int nWidth = -1
			);
		BOOL InsertColumn( int nCol, CListHeaderItemUI *pHeaderItem );
		/**
		 * ������,�վ�,��0��ʼ
		 * 
		 * @param	nItem					����,��Ҫ��0��ʼ���β���
		 *
		 * @return	�ɹ��򷵻�����,���򷵻�-1
		 */
		int InsertItem(int nItem, int nHeight = 20,std::string listName="");
		int InsertItem(int nItem, int nHeight, CListContainerElementUI *pListItem);

		/*ɾ�� ĳһ��*/
		bool RemoveItem(int nItem);
	
		void SetItemData(int nItem, int nColumn, LPCTSTR Text, LPCTSTR Name);
		void SetItemData(int nItem, int nColumn, CControlUI* pControl);

		void SetItemText(int nItem, int nColumn,LPCTSTR itemName, LPCTSTR Text);

		// ��ӿؼ����м�λ��.
		void SetItemDataCenter(int nItem, int nColumn, CControlUI* pControl,LPCTSTR inset);
		/**
		 * ����������ȡ�пؼ�
		 * 
		 * @param	iIndex					����,��0���������
		 * 
		 * @return	�ɹ�ʱ�����ӿؼ���ַ,���򷵻�NULL
		 */
		CListContainerElementUI* GetListItem(int iIndex);

		/**
		 * ��ȡ����λ�õĿؼ�
		 * 
		 * @param	iIndex					����,��0���������
		 * @param	iSubIndex					����,��0���������
		 * 
		 * @return	�ɹ�ʱ�����ӿؼ���ַ,���򷵻�NULL
		 */
		CHorizontalLayoutUI* GetListSubItem(int iIndex, int iSubIndex);
		BOOL SetHeaderItemData(int nColumn, CControlUI* pControl);

	private:

	};
}
#endif