#pragma once

namespace DuiLib
{
	class UILIB_API DialogImplBase
		: public CWindowWnd
		, public CNotifyPump
		, public INotifyUI
		, public IMessageFilterUI
		, public IDialogBuilderCallback
	{
	public:
		DialogImplBase() {}
		
		virtual ~DialogImplBase(){};
		virtual void InitWindow(){};
		virtual void OnFinalMessage( HWND hWnd );
		virtual void Notify(TNotifyUI& msg);

		////DUI_DECLARE_MESSAGE_MAP()
		virtual void OnClick(TNotifyUI& msg);

	protected:
		virtual CDuiString GetSkinFolder()=0;
		virtual CDuiString GetSkinFile()= 0;
		virtual LPCTSTR GetWindowClassName(void) const = 0 ;

		CPaintManagerUI m_PaintManager;
		static LPBYTE m_lpResourceZIPBuffer;

	public:
		virtual UINT GetClassStyle() const;

		virtual CControlUI* CreateControl(LPCTSTR pstrClass);
		virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);

#if defined(WIN32) && !defined(UNDER_CE)
		virtual LRESULT OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
#endif
		virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LONG GetStyle();
        virtual BOOL IsInStaticControl(CControlUI *pControl); // ????????:???????,??????,??????????,???TRUE,????FALSE

	};


// HOW TO USE 

/*
	CXmlDialog *testckb = new CXmlDialog("dialog/dialgo.xml");;
	testckb->CreateWnd(m_PaintManager.GetPaintWindow());
	testckb->ShowModal();

	WITH caption 0,cannot drag it.

*/
	class CXmlDialog : public DialogImplBase
	{
	public:
	    explicit CXmlDialog(LPCTSTR pszXMLName) 
	        : m_strXMLName(pszXMLName){}

		
	public:

		virtual LPCTSTR GetWindowClassName() const
	    {
	        return _T("UIXmlDialog");
	    }

	    virtual CDuiString GetSkinFile()
	    {
	        return m_strXMLName;
	    }

	    virtual CDuiString GetSkinFolder()
	    {
	        return _T("");
	    }

		void CreateWnd(HWND hParent)
		{
			Create(hParent, _T(""), GetClassStyle(), 0, 0, 0, 0, 0, NULL);
			CenterWindow();
		}

	 	CDuiString m_strXMLName;

		};
}

