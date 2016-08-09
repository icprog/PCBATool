
// PCBATool.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "./settings/SettingBase.h"
#include "..\targetdevice\common\target.h"
#include "./common/common.h"


// CPCBAToolApp:
// See PCBATool.cpp for the implementation of this class
//

class CPCBAToolApp : public CWinAppEx
{
public:
	CPCBAToolApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CPCBAToolApp theApp;