// WMDlg.h: header file

#pragma once

//#include "LogIn.h"

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "LogIn.h"

using namespace std;
using namespace cv;

// CWMDlg dialog
class CWMDlg : public CDialogEx
{
	// Create.
public:
	CWMDlg(CWnd* pParent = nullptr);	// Standard constructor.

	// Dialog data.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support.


// implementation.
protected:
	HICON m_hIcon;

	// Created message map function
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	/*image variable */
	Mat QrImg; //Qrcode
	Mat HostImg; //Host image
	Mat MarkedImg; //QR Marked image

	/*Login*/
	LogIn LogInDialog; //LogIn dialog
	afx_msg void OnBnClickedLoginbutton();

	/*Select an image*/
	afx_msg void OnBnClickedImagebutton();

	/* Insert watermark */
	afx_msg void OnBnClickedInsertbutton();

	/* Extract watermark */
	afx_msg void OnBnClickedExtrabutton();
};
