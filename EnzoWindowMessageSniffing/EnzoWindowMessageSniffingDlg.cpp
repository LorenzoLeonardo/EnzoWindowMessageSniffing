
// EnzoWindowMessageSniffingDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "EnzoWindowMessageSniffing.h"
#include "EnzoWindowMessageSniffingDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEnzoWindowMessageSniffingDlg dialog



CEnzoWindowMessageSniffingDlg::CEnzoWindowMessageSniffingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ENZOWINDOWMESSAGESNIFFING_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	dll_handle = NULL;
}

void CEnzoWindowMessageSniffingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_AREA, m_ctrlEditArea);
}

BEGIN_MESSAGE_MAP(CEnzoWindowMessageSniffingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_ENZO_MESSAGE, OnUserDefinedMessage)
END_MESSAGE_MAP()


BOOL CEnzoWindowMessageSniffingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	shiftPressed = GetKeyState(VK_SHIFT);
	capsLockOn = GetKeyState(VK_CAPITAL);

	HHOOK process_hook;
	dll_handle = LoadLibrary(_T("EnzoSniffingDLL.dll"));
	
	if (dll_handle)
	{
		installProc = (LPInstallHook)GetProcAddress(dll_handle, "InstallHook");
		uninstallProc = (LPUninstallHook)GetProcAddress(dll_handle, "UninstallHook");
	}
	
	installProc(m_hWnd);
	return TRUE;  // return TRUE  unless you set the focus to a control
}
CString CEnzoWindowMessageSniffingDlg::TranslateKeypressed(CString csStr)
{
	if (!csStr.Compare(_T("Enter")) || !csStr.Compare(_T("Num Enter")))
		return _T("Enter");
	else if (!csStr.Compare(_T(".")) || !csStr.Compare(_T("Num Del")))
		return _T(".");
	else if (!csStr.Compare(_T("Space")))
		return _T("Space");
	else if (!csStr.Compare(_T("Num 1")))
		return _T("1");
	else if (!csStr.Compare(_T("Num 2")))
		return _T("2");
	else if (!csStr.Compare(_T("Num 3")))
		return _T("3");
	else if (!csStr.Compare(_T("Num 4")))
		return _T("4");
	else if (!csStr.Compare(_T("Num 5")))
		return _T("5");
	else if (!csStr.Compare(_T("Num 6")))
		return _T("6");
	else if (!csStr.Compare(_T("Num 7")))
		return _T("7");
	else if (!csStr.Compare(_T("Num 8")))
		return _T("8");
	else if (!csStr.Compare(_T("Num 9")))
		return _T("9");
	else if (!csStr.Compare(_T("Num 0")))
		return _T("0");
	else
		return csStr;
}
LRESULT CEnzoWindowMessageSniffingDlg::OnUserDefinedMessage(WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT hooked = *((KBDLLHOOKSTRUCT*)lParam);
	TCHAR charPressed = hooked.vkCode;
	
	shiftPressed = GetKeyState(VK_SHIFT);
	capsLockOn = GetKeyState(VK_CAPITAL);

	if ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN))
	{
		if (hooked.vkCode == VK_LSHIFT || hooked.vkCode == VK_RSHIFT)
		{
			shiftPressed = true;
		}
		else if (hooked.vkCode == VK_CAPITAL)
		{
			capsLockOn = true;
		}
		else
		{
			
			if ((0 != isalpha(charPressed)) && (shiftPressed ^ capsLockOn))
			{
				charPressed = tolower(charPressed);
			}
		}

		DWORD dwMsg = 1;
		dwMsg += hooked.scanCode << 16;
		dwMsg += hooked.flags << 24;

		TCHAR key[16];
		GetKeyNameText(dwMsg, key, 15);

		CString csStr;
		CString csWin;

		
		csStr = TranslateKeypressed(key);		
		if (!csStr.Compare(_T("Backspace")))
		{
			m_ctrlEditArea.GetWindowText(csWin);
			if (csWin.GetLength() > 0)
			{
				csWin.Delete(csWin.GetLength() - 1, 1);
				m_ctrlEditArea.SetWindowText(csWin);
			}
		}
		else if (!csStr.Compare(_T("Enter")))
		{
			m_ctrlEditArea.GetWindowText(csWin);
			csWin += _T("\r\n");
			m_ctrlEditArea.SetWindowText(csWin);
		}
		else if(isalnum(charPressed))
		{
			m_ctrlEditArea.GetWindowText(csWin);
			csWin = csWin + csStr;
			m_ctrlEditArea.SetWindowText(csWin);
		}
		else if (!csStr.Compare(_T(".")) || !csStr.Compare(_T("0")))
		{
			m_ctrlEditArea.GetWindowText(csWin);
			csWin = csWin + csStr;
			m_ctrlEditArea.SetWindowText(csWin);
		}
		
	}
	else if ((wParam == WM_SYSKEYUP) || (wParam == WM_KEYUP))
	{
		if (hooked.vkCode == VK_LSHIFT || hooked.vkCode == VK_RSHIFT)
		{
			shiftPressed = false;
		}
		else if (hooked.vkCode == VK_CAPITAL)
		{
			capsLockOn = false;
		}
	}


	
	return 0;
}
void CEnzoWindowMessageSniffingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEnzoWindowMessageSniffingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEnzoWindowMessageSniffingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

