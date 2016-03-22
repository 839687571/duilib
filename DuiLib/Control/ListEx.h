
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
		 * 构造函数
		 */
		CListUIEx();

		/**
		 * 析构函数
		 */
		virtual ~CListUIEx();

		/**
		 * 插入列,照旧,从0开始
		 * 
		 * @param	nCol					列数,需要从0开始依次插入
		 * @param	lpszColumnHeaderStr		每一列标题
		 * @param	uTextStyle				标题文字对齐方式
		 * @param	nWidth					列宽
		 * 
		 * @return	返回True为成功,False为失败
		 */
		BOOL InsertColumn(
			int nCol,
			LPCTSTR lpszColumnHeaderStr,
			UINT uTextStyle = DT_LEFT,
			int nWidth = -1
			);
		BOOL InsertColumn( int nCol, CListHeaderItemUI *pHeaderItem );
		/**
		 * 插入行,照旧,从0开始
		 * 
		 * @param	nItem					行数,需要从0开始依次插入
		 *
		 * @return	成功则返回行数,否则返回-1
		 */
		int InsertItem(int nItem, int nHeight = 20,std::string listName="");
		int InsertItem(int nItem, int nHeight, CListContainerElementUI *pListItem);

		/*删除 某一项*/
		bool RemoveItem(int nItem);
	
		void SetItemData(int nItem, int nColumn, LPCTSTR Text, LPCTSTR Name);
		void SetItemData(int nItem, int nColumn, CControlUI* pControl);

		void SetItemText(int nItem, int nColumn,LPCTSTR itemName, LPCTSTR Text);

		// 添加控件到中间位置.
		void SetItemDataCenter(int nItem, int nColumn, CControlUI* pControl,LPCTSTR inset);
		/**
		 * 根据索引获取行控件
		 * 
		 * @param	iIndex					行数,从0到最大行数
		 * 
		 * @return	成功时返回子控件地址,否则返回NULL
		 */
		CListContainerElementUI* GetListItem(int iIndex);

		/**
		 * 获取具体位置的控件
		 * 
		 * @param	iIndex					行数,从0到最大行数
		 * @param	iSubIndex					列数,从0到最大行数
		 * 
		 * @return	成功时返回子控件地址,否则返回NULL
		 */
		CHorizontalLayoutUI* GetListSubItem(int iIndex, int iSubIndex);
		BOOL SetHeaderItemData(int nColumn, CControlUI* pControl);

	private:

	};
}
#endif