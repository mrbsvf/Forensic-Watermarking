
// WM.h: The main header file for the ROJECT_NAME application.

#pragma once

#ifndef __AFXWIN_H__
	#error "Include 'stdafx.h' before including this file for PCH."
#endif

#include "resource.h"		// The main symbol.

// CWMApp:
// See WM.cpp for an implementation of this class.

class CWMApp : public CWinApp
{
public:
	CWMApp();

	// Override.
public:
	virtual BOOL InitInstance();

// implementation.

	DECLARE_MESSAGE_MAP()
};

extern CWMApp theApp;