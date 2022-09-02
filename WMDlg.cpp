// WMDlg.cpp: implementation file

#include "stdafx.h"
#include "WM.h"
#include "WMDlg.h"
#include "afxdialogex.h"
#include "Headers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for application information.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog data.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support.

	// implementation.
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


// CWMDlg dialog box
CWMDlg::CWMDlg(CWnd* pParent /*=nullptr*/) : CDialogEx(IDD_WM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWMDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CWMDlg::OnBnClickedLoginbutton)
	ON_BN_CLICKED(IDC_BUTTON2, &CWMDlg::OnBnClickedImagebutton)
	ON_BN_CLICKED(IDC_BUTTON3, &CWMDlg::OnBnClickedInsertbutton)
	ON_BN_CLICKED(IDC_BUTTON4, &CWMDlg::OnBnClickedExtrabutton)
END_MESSAGE_MAP()


// CWMDlg message handler

BOOL CWMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add the "About..." menu item to the system menu.

	// IDM_ABOUTBOX must be in system command scope.
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

	// Set the icon for this dialog box. If the application's main window is not a dialog box,
	// The framework does this automatically.
	SetIcon(m_hIcon, TRUE);			// Set the large icon.
	SetIcon(m_hIcon, FALSE);		// Set the small icon.

	// TODO: Add an extra initialization action here.

	return TRUE;  // Returns TRUE if focus is not set on the control.
}

void CWMDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// To draw an icon when adding a minimize button to a dialog box
//  I need the code below. For MFC applications that use the document/view model,
//  The framework does this for you automatically.

void CWMDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // The device context for drawing.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center the icon in the client rectangle.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Make the system visible while the user drags the minimized window
// Call this function.
HCURSOR CWMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CWMDlg::OnBnClickedLoginbutton()
{
	if (LogInDialog.DoModal() == IDOK) {
	
		QrImg = LogInDialog.QrImg;     // Get the generated 32x32 barcode.

	}
}


void CWMDlg::OnBnClickedImagebutton()
{
	CFileDialog finder(true);

	if (finder.DoModal() == IDOK)
	{
		if (finder.GetFileName().Find(_T(".bmp")) != -1 || finder.GetFileName().Find(_T(".jpg")) != -1 || finder.GetFileName().Find(_T(".png")) != -1 || finder.GetFileName().Find(_T(".PNG")) != -1)
		{
			CT2CA path1(finder.GetPathName());

			HostImg = imread(string(path1), 1);

		}
		else
			AfxMessageBox(_T("Select The Photo"));
	}
	else {
		return;
	}

	//imshow("original", HostImg);
}


void CWMDlg::OnBnClickedInsertbutton()
{
	if (HostImg.empty()) {
		AfxMessageBox(_T("Not exist image"));
		return;
	}
	if (QrImg.empty()) {
		AfxMessageBox(_T("Not logged In"));
		return;
	}


	MarkedImg = EmbedWatermark(HostImg, QrImg);

	//imshow("Marked Image", MarkedImg);
}


void CWMDlg::OnBnClickedExtrabutton()
{
	Mat Marked_Image;
	CFileDialog finder(true);

	if (finder.DoModal() == IDOK)
	{
		if (finder.GetFileName().Find(_T(".bmp")) != -1 || finder.GetFileName().Find(_T(".jpg")) != -1 || finder.GetFileName().Find(_T(".png")) != -1 || finder.GetFileName().Find(_T(".PNG")) != -1)
		{
			CT2CA path1(finder.GetPathName());

			Marked_Image = imread(string(path1), 1);

		}
		else
			AfxMessageBox(_T("Select The Photo"));
	}
	else {
		return;
	}

	ExtractWatermark(Marked_Image);
	
}
