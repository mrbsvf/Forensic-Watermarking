﻿// WM.cpp: Defines the class behavior for the application.

#include "stdafx.h"
#include "WM.h"
#include "WMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWMApp
BEGIN_MESSAGE_MAP(CWMApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Create CWMApp
CWMApp::CWMApp()
{
	// support restart manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: Add the generation code here.
	// Place all important initialization tasks in InitInstance.
}

// The only CWMApp object.
CWMApp theApp;

// Initialize CWMApp
BOOL CWMApp::InitInstance()
{
	// The application manifest uses ComCtl32.dll version 6 or later to create visual styles.
	// If you specify to use, InitCommonControlsEx() is required on Windows XP.
	// You cannot create a window without using InitCommonControlsEx().
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// to include all common control classes for use in the application
	// Set this item.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// In the dialog box, the shell tree view or
	// If the shell contains a list view control, create a shell manager.
	CShellManager *pShellManager = new CShellManager;

	// Enable "Windows Prototype" visual manager to use themes from MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// standard initialization
	// If you want to reduce the size of the final executable without using these functions
	// specific initialization not needed below
	// The routine must be removed.
	// Change the registry key where those settings are stored.
	// TODO: replace this string with the name of a company or organization
	// Be sure to modify it with the appropriate content.
	SetRegistryKey(_T("Applications generated by the local application wizard"));

	CWMDlg dlg;

	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Click [OK] here to process when the dialog box disappears.
		//  Place your code.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Click [Cancel] here to handle when the dialog box disappears.
		//  Place your code.
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "WARNING: Failed to create dialog, application quits unexpectedly.\n");
		TRACE(traceAppMsg, 0, "WARNING: Cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS when using MFC controls in a dialog box.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// Since the dialog is closed, set FALSE to allow the application to exit without starting the application's message pump.
	// return.
	return FALSE;
}

