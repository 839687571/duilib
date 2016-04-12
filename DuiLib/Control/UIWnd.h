#pragma  once

namespace DuiLib {


	// 将HWND显示到CControlUI上面
	class UILIB_API CWndUI : public CControlUI {
	public:
		CWndUI() : m_hWnd(NULL) {}

		virtual void SetVisible(bool bVisible = true)
		{
			__super::SetVisible(bVisible);
			::ShowWindow(m_hWnd, bVisible);
		}

		virtual void SetInternVisible(bool bVisible = true)
		{
			__super::SetInternVisible(bVisible);
			::ShowWindow(m_hWnd, bVisible);
		}

		/*virtual void SetPos(RECT rc)
		{
		__super::SetPos(rc);
		int w = rc.right - rc.left;
		int h = rc.bottom - rc.top;
		::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, w, h, SWP_NOZORDER | SWP_NOACTIVATE);
		}*/

		virtual void SetPos(RECT rc, bool bNeedInvalidate = true)
		{
			RECT rect;
			__super::SetPos(rc, bNeedInvalidate);
			rect = m_rcItem;
			int w = rc.right - rc.left;
			int h = rc.bottom - rc.top;
			DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
			if (dwStyle & WS_CHILD) {
				if (IsFloat()) {
					::SetWindowPos(m_hWnd, NULL, rect.left, rect.top, w, h, SWP_NOZORDER | SWP_NOACTIVATE);
				} else {
					::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, w, h, SWP_NOZORDER | SWP_NOACTIVATE);
				}
			}
		}

		void  SetPosDirect(RECT rcPos)
		{
			::SetWindowPos(m_hWnd, NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left,
				rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);
		}

		BOOL Attach(HWND hWndNew)
		{
			if (!::IsWindow(hWndNew)) {
				return FALSE;
			}

			m_hWnd = hWndNew;
			return TRUE;
		}

		HWND Detach()
		{
			HWND hWnd = m_hWnd;
			m_hWnd = NULL;
			return hWnd;
		}

		HWND GetHWND()
		{
			return m_hWnd;
		}

	protected:
		HWND m_hWnd;
	};
}