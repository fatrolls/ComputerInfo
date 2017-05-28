// InjectDllDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InjectDll.h"
#include "InjectDllDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInjectDllDlg dialog

CInjectDllDlg::CInjectDllDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInjectDllDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInjectDllDlg)
	m_pid = 0;
	m_txtdll = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInjectDllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInjectDllDlg)
	DDX_Text(pDX, IDC_EDITPID, m_pid);
	DDX_Text(pDX, IDC_EDITDLL, m_txtdll);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInjectDllDlg, CDialog)
	//{{AFX_MSG_MAP(CInjectDllDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_UNLOAD, OnUnload)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInjectDllDlg message handlers

BOOL CInjectDllDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CInjectDllDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CInjectDllDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CInjectDllDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CInjectDllDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	char * dll = (LPSTR)(LPCTSTR)this->m_txtdll;
	InjectDll(this->m_pid, dll);
}

void CInjectDllDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialog::OnClose();
}

void CInjectDllDlg::InjectDll(DWORD dwpid, char *szDllName)
{
	if(dwpid == 0 || strlen(szDllName) == 0)
	{
		return ;
	}
	char * pFunName = "LoadLibraryA";
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwpid);
	if(hProcess == NULL)
	{
		return ;
	}
	int nDllLen = strlen(szDllName) + sizeof(char);

	PVOID pDllAddr = VirtualAllocEx(hProcess, NULL, nDllLen, MEM_COMMIT, PAGE_READWRITE);
	if(pDllAddr == NULL)
	{
		CloseHandle(hProcess);
		return;
	}

	DWORD dwWriteNum = 0;
	WriteProcessMemory(hProcess, pDllAddr, szDllName, nDllLen, &dwWriteNum);
	FARPROC pFunAddr = GetProcAddress(GetModuleHandle(_T("Kernel32.dll")), pFunName);

	HANDLE hThread = CreateRemoteThread(hProcess,
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)pFunAddr,
		pDllAddr,
		0,
		NULL);
	if(hThread)
	{
		AfxMessageBox("inject handle well");
	}

	if(WAIT_OBJECT_0 == WaitForSingleObject(hThread, INFINITE))
	{
		AfxMessageBox("inject successed!");
	}

	CloseHandle(hThread);
	CloseHandle(hProcess);
}

#include <tlhelp32.h>
void CInjectDllDlg::UnInjectDLL(DWORD dwPid, char *szDllName)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,
		dwPid);
	MODULEENTRY32 Me32 = {0};
	Me32.dwSize = sizeof(MODULEENTRY32);
	BOOL bRet = Module32First(hSnap, &Me32);
	while(bRet)
	{
		if(strcmp(Me32.szExePath, szDllName) == 0)
		{
			break;
		}
		bRet = Module32Next(hSnap, &Me32);
	}
	CloseHandle(hSnap);

	char *pFunName = "FreeLibrary";

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,
		FALSE, dwPid);
	FARPROC pFunAddr = GetProcAddress(GetModuleHandle("kernel32.dll"),
		pFunName);
	HANDLE hThread = CreateRemoteThread(hProcess,
		NULL,0,
		(LPTHREAD_START_ROUTINE)pFunAddr,
		Me32.hModule,
		0,NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hProcess);
}

void CInjectDllDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
	::EndDialog(this->m_hWnd, 0);
}

void CInjectDllDlg::OnUnload() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	char * dll = (LPSTR)(LPCTSTR)this->m_txtdll;
	UnInjectDLL(this->m_pid, dll);
	CDialog::OnClose();
}
