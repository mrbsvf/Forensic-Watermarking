// LogIn.cpp: implementation file
//

#include "stdafx.h"
#include "WM.h"
#include "LogIn.h"
#include "afxdialogex.h"

#include "QRmain.h"


// LogIn dialog box

IMPLEMENT_DYNAMIC(LogIn, CDialogEx)

LogIn::LogIn(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LoginDlg, pParent)
{

}

LogIn::~LogIn()
{
}

void LogIn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, ID);
}


BEGIN_MESSAGE_MAP(LogIn, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &LogIn::OnBnClickedLog)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void LogIn::OnBnClickedLog()
{
	CString data;   // CString variable to store the string input to m_Editln

	CString tmpStr = _T("");
	int v;

	// Assign the string input to the edit control to CString
	ID.GetWindowText(data);

	QrImg = doBasicDemo(data); // Qrcode Image

	OnOK();
}


BOOL LogIn::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add specialized code here and/or call base classes.
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		OnBnClickedLog();
		return true;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
