
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
	m_shiftPressed = false;
	m_controlPressed = false;
	m_capsLockOn = GetKeyState(VK_CAPITAL);

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

LRESULT CEnzoWindowMessageSniffingDlg::OnUserDefinedMessage(WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT hooked = *((KBDLLHOOKSTRUCT*)lParam);
	TCHAR charPressed = hooked.vkCode;
	CString csWin = _T("");

	m_capsLockOn = GetKeyState(VK_CAPITAL);

	if ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN))
	{
		if (!m_controlPressed)
		{
			if (hooked.vkCode == VK_LSHIFT || hooked.vkCode == VK_RSHIFT)
			{
				m_shiftPressed = true;
			}
			else if (hooked.vkCode == VK_CAPITAL)
			{
				m_capsLockOn = true;
			}
			else if ((hooked.vkCode >= VK_LCONTROL) && (hooked.vkCode <= VK_RCONTROL))
			{
				m_controlPressed = true;
			}
			else if (hooked.vkCode == VK_BACK)
			{
				m_ctrlEditArea.GetWindowText(csWin);
				if (csWin.GetLength() > 0)
				{
					csWin.Delete(csWin.GetLength() - 1, 1);
					m_ctrlEditArea.SetWindowText(csWin);
				}
			}
			else if (hooked.vkCode == VK_RETURN)
			{
				m_ctrlEditArea.GetWindowText(csWin);
				csWin += _T("\r\n");
				m_ctrlEditArea.SetWindowText(csWin);
			}
			else if ((hooked.vkCode >= 0x30) && (hooked.vkCode <= 0x39))
			{
				char key[] = {')','!','@' ,'#' ,'$' ,'%' ,'^' ,'&' ,'*' ,'(' };

				if (m_shiftPressed)
					charPressed = key[hooked.vkCode - 0x30];
				m_ctrlEditArea.GetWindowText(csWin);
				csWin = csWin + charPressed;
				m_ctrlEditArea.SetWindowText(csWin);
			}
			else if ((hooked.vkCode >= 0x41) && (hooked.vkCode <= 0x5A))
			{
				if ((0 != isalpha(charPressed)) && !(m_shiftPressed ^ m_capsLockOn))
				{
					charPressed = tolower(charPressed);
				}

				m_ctrlEditArea.GetWindowText(csWin);
				csWin = csWin + charPressed;
				m_ctrlEditArea.SetWindowText(csWin);
			}
			//num keys
			else if ((hooked.vkCode >= VK_NUMPAD0) && (hooked.vkCode <= VK_NUMPAD9))
			{
				char key[] = { '0','1','2','3', '4','5','6','7','8','9' };

				m_ctrlEditArea.GetWindowText(csWin);
				csWin = csWin + key[hooked.vkCode - VK_NUMPAD0];
				m_ctrlEditArea.SetWindowText(csWin);
			}
			else if ((hooked.vkCode >= VK_MULTIPLY) && (hooked.vkCode <= VK_DIVIDE))
			{
				char key[] = { '*','+','/','-', '.','/' };

				m_ctrlEditArea.GetWindowText(csWin);
				csWin = csWin + key[hooked.vkCode - VK_MULTIPLY];
				m_ctrlEditArea.SetWindowText(csWin);
			}

			else if (hooked.vkCode == VK_SPACE)
			{
				m_ctrlEditArea.GetWindowText(csWin);
				csWin = csWin + _T(" ");
				m_ctrlEditArea.SetWindowText(csWin);
			}
			else if (hooked.vkCode == VK_TAB)
			{
				m_ctrlEditArea.GetWindowText(csWin);
				csWin = csWin + _T("\t");
				m_ctrlEditArea.SetWindowText(csWin);
			}
			//#define VK_OEM_4          0xDB  //  '[{' for US
			//#define VK_OEM_5          0xDC  //  '\|' for US
			//#define VK_OEM_6          0xDD  //  ']}' for US
			//#define VK_OEM_7          0xDE  //  ''"' for US
			else if(hooked.vkCode >= 0xDB && hooked.vkCode <= 0xDE)
			{
				char upperKey[] = {'{','|', '}', '"' };
				char lowerKey[] = {'[','\\' , ']' , '\''};

				if (m_shiftPressed)
					charPressed = upperKey[hooked.vkCode - 0xDB];
				else
					charPressed = lowerKey[hooked.vkCode - 0xDB];

				m_ctrlEditArea.GetWindowText(csWin);
				csWin = csWin + charPressed;
				m_ctrlEditArea.SetWindowText(csWin);
			}
			//#define VK_OEM_1          0xBA   // ';:' for US
			//#define VK_OEM_PLUS       0xBB   // '+' any country
			//#define VK_OEM_COMMA      0xBC   // ',' any country
			//#define VK_OEM_MINUS      0xBD   // '-' any country
			//#define VK_OEM_PERIOD     0xBE   // '.' any country
			//#define VK_OEM_2          0xBF   // '/?' for US
			//#define VK_OEM_3          0xC0   // '`~' for US
			else if (hooked.vkCode >= 0xBA && hooked.vkCode <= 0xC0)
			{
				char upperKey[] = { ':', '+', '<', '_','>','?','~'};
				char lowerKey[] = { ';', '=', ',', '-','.','/','`'};

				if (m_shiftPressed)
					charPressed = upperKey[hooked.vkCode - 0xBA];
				else
					charPressed = lowerKey[hooked.vkCode - 0xBA];

				m_ctrlEditArea.GetWindowText(csWin);
				csWin = csWin + charPressed;
				m_ctrlEditArea.SetWindowText(csWin);
			}

			m_ctrlEditArea.SetFocus();
			m_ctrlEditArea.SetSel(-1);
		}

	}
	else if ((wParam == WM_SYSKEYUP) || (wParam == WM_KEYUP))
	{
		if (hooked.vkCode == VK_LSHIFT || hooked.vkCode == VK_RSHIFT)
		{
			m_shiftPressed = false;
		}
		else if (hooked.vkCode == VK_CAPITAL)
		{
			m_capsLockOn = false;
		}
		else if ((hooked.vkCode >= VK_LCONTROL) && (hooked.vkCode <= VK_RCONTROL))
		{
			m_controlPressed = false;
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

