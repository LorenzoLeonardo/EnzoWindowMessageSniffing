
// EnzoWindowMessageSniffingDlg.h : header file
//

#pragma once
#define WM_ENZO_MESSAGE (WM_USER + 1)

typedef BOOL(WINAPI* LPInstallHook)(HWND);
typedef VOID(WINAPI* LPUninstallHook)();
// CEnzoWindowMessageSniffingDlg dialog
class CEnzoWindowMessageSniffingDlg : public CDialogEx
{
// Construction
public:
	CEnzoWindowMessageSniffingDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENZOWINDOWMESSAGESNIFFING_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	LPInstallHook installProc;
	LPUninstallHook uninstallProc;

// Implementation
protected:
	bool m_shiftPressed;
	bool m_capsLockOn;
	bool m_controlPressed;

	HICON m_hIcon;
	HMODULE dll_handle;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg LRESULT OnUserDefinedMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CEdit m_ctrlEditArea;
};
