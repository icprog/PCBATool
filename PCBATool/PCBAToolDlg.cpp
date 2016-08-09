
// PCBAToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PCBATool.h"
#include "PCBAToolDlg.h"
#include <Dbt.h>
#include "./config/PtConfigDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CwvpctoolDlg dialog
UINT ScanDeviceThread(LPVOID lpParam)
{
	CPCBAToolDlg *pDlg = (CPCBAToolDlg *)lpParam;
	pDlg->ScanThreadProc();
    /*pDlg->m_hScanThread = NULL; **/
	return 0;
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg(CIniSettingBase &Configs,CIniLocalLan &LocalLang);
// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
private:
    CLabel          m_rklink;
    CLabel          m_AppName;
    CIniSettingBase &m_Configs;
    CIniLocalLan    &m_LocalLang;
// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg(CIniSettingBase &Configs,CIniLocalLan &LocalLang) : CDialog(CAboutDlg::IDD)
    ,m_Configs(Configs),m_LocalLang(LocalLang)
{
}
void CAboutDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    OnOK();
}
BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_rklink.SetTransparent(TRUE);
	m_rklink.SetFontSize(8);
	m_rklink.SetFontBold(TRUE);
    m_rklink.SetText(TEXT("福州瑞芯微电子有限公司"), 0xFF1111);
    m_rklink.SetLink(TRUE,FALSE);
    m_rklink.SetHyperLink(CString(TEXT("http://www.rock-chips.com/")));
    /*m_rklink.SetLinkCursor( (HCURSOR)IDC_IBEAM); **/
    /*m_rklink.FlashText(TRUE);**/
    m_AppName.SetTransparent(TRUE);
    m_AppName.SetFontSize(8);
    m_AppName.SetFontBold(TRUE);
    m_AppName.SetText((std::wstring(TEXT("PCBATool"))+ TEXT(APP_VERSION)).c_str(), 0xFF1111);
    /*GetDlgItem(IDC_STATIC_APPNAME)->SetWindowText((m_LocalLang.GetStr(TEXT("APPNAME")) + TEXT(APP_VERSION)).c_str());**/
    return FALSE;
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_RK, m_rklink);
    DDX_Control(pDX, IDC_STATIC_APPNAME, m_AppName);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CPCBAToolDlg dialog




CPCBAToolDlg::CPCBAToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPCBAToolDlg::IDD, pParent),m_pLog(NULL),m_bStart(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPCBAToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPCBAToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_DEVICECHANGE()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_START, &CPCBAToolDlg::OnBnClickedButtonStart)
    ON_UPDATE_COMMAND_UI(ID_SETTING, &CPCBAToolDlg::OnUpdateSetting)
    ON_COMMAND(ID_SETTING, &CPCBAToolDlg::OnSetting)
    ON_COMMAND(ID_HELP_OPENLOGFOLDER, &CPCBAToolDlg::OnHelpOpenlogfolder)
    ON_WM_SIZE()
    ON_COMMAND(ID_HELP_ABOUT, &CPCBAToolDlg::OnHelpAbout)
	ON_COMMAND(ID_SETTING_LOGIN, &CPCBAToolDlg::OnSettingLogin)
	ON_COMMAND(ID_USER_LOGINOUT, &CPCBAToolDlg::OnUserLogout)
	ON_COMMAND(ID_USER_CHANGEPASSWORD, &CPCBAToolDlg::OnUserChangepassword)
	ON_UPDATE_COMMAND_UI(ID_USER_CHANGEPASSWORD, &CPCBAToolDlg::OnUpdateUserChangepassword)
END_MESSAGE_MAP()


// CPCBAToolDlg message handlers

BOOL CPCBAToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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
    TCHAR MyexeFullPath[MAX_PATH] = TEXT("C:\\");
    if(GetModuleFileName(NULL,MyexeFullPath,MAX_PATH)) {
        TCHAR *pos = _tcsrchr(MyexeFullPath, TEXT('\\'));
        if(pos) {
            pos[1] = 0;
            m_strModulePath = MyexeFullPath;
        }
    }
    m_Configs.LoadToolSetting((LPCTSTR)(m_strModulePath+ TEXT("config.ini")));
    if(m_Configs.szLan.empty()) {
        m_Configs.szLan = TEXT("1.txt");
    }
    if(m_Configs.nLogLevel != DLEVEL_NONE ) {
        CLogger::DEBUG_LEVEL level = m_Configs.nLogLevel == DLEVEL_DEBUG?CLogger::DEBUG_ALL:
            (m_Configs.nLogLevel  == DLEVEL_INFO ?CLogger::DEBUG_INFO:CLogger::DEBUG_ERROR);
        if(m_Configs.strLogPath.empty()) {
            m_Configs.strLogPath = (LPCTSTR)(m_strModulePath + TEXT("log\\"));
        }
        m_pLog = CLogger::StartLog((m_Configs.strLogPath + CLogger::TimeStr(true, true)).c_str(), level);    
    }
	m_LocalLang.LoadToolSetting((LPCTSTR)(m_strModulePath +TEXT("Lan\\") + m_Configs.szLan.c_str()));
	m_LocalLang.TreeControls(m_hWnd,m_Configs.bDebug?TRUE:FALSE,this->IDD,true);
	if(!m_Configs.bDebug) {
		WalkMenu(GetMenu(),TEXT("MENU"));
	}
	GetWindowText(m_DlgTitle);
#ifdef USER_LOGIN
	bAdmin = m_Configs.bLogin;
	if(bAdmin) {
		SetWindowText(((LPCTSTR)m_DlgTitle + GetLocalString(TEXT("IDS_TEXT_ADMIN"))).c_str());
	} else {
		SetWindowText(((LPCTSTR)m_DlgTitle + GetLocalString(TEXT("IDS_TEXT_USER"))).c_str());
	}
#else 
	bAdmin = TRUE;
#endif

    LDEGMSG((CLogger::DEBUG_INFO,"APP START ..."));
    SetDlgItemText(IDC_BUTTON_START,GetLocalString(TEXT("IDS_START")).c_str());
    ShowCheckDlg(TRUE);

	m_hDevScanEvent     = CreateEvent(NULL,FALSE,FALSE,TEXT("DEVICE_SCAN_EVENT"));
    m_hDevScanExitEvent = CreateEvent(NULL,FALSE,FALSE,TEXT("DEVICE_SCAN_EXIT_EVENT"));
    if(m_hDevScanEvent&&m_hDevScanExitEvent) {
        m_hScanThread   = AfxBeginThread(::ScanDeviceThread,(LPVOID)this);
        SetEvent(m_hDevScanEvent);
    }


    RegDevNotification(m_hWnd);

	return TRUE;  // return TRUE  unless you set the focus to a control
}
VOID CPCBAToolDlg::WalkMenu(CMenu *pMenu,CString strMainKeyPart)
{
		CString strKey;
		int     id ;
		UINT    i;
		if(NULL == pMenu) return ;
		for (i=0;i < pMenu->GetMenuItemCount();i++) {
				strKey.Format(_T("%s_%d"),strMainKeyPart,i);
				id = pMenu->GetMenuItemID(i);
				if (0 == id) { 
						/*If nPos corresponds to a SEPARATOR menu item, the return value is 0. **/
				} else if (-1 == id) { 
						/*If the specified item is a pop-up menu (as opposed to an item within the pop-up menu), the return value is –1 **/
						pMenu->ModifyMenu(i, MF_BYPOSITION, i, m_LocalLang.GetStr((LPCTSTR)strKey).c_str());
						WalkMenu(pMenu->GetSubMenu(i),strKey);
				} else {
						pMenu->ModifyMenu(id, MF_BYCOMMAND, id,m_LocalLang.GetStr((LPCTSTR)strKey).c_str());
				}
		}
}
void CPCBAToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout(m_Configs,m_LocalLang);
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

void CPCBAToolDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPCBAToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CPCBAToolDlg::AppExit(BOOL bSaveConfig)
{
    if(bSaveConfig) m_Configs.SaveToolSetting(std::wstring(TEXT("")));


#if 1
    if (NULL != m_hScanThread) {
        SetEvent(m_hDevScanExitEvent);
        if(WAIT_OBJECT_0 != WaitForSingleObject(m_hScanThread->m_hThread,1000)) {
            LDEGMSGW((CLogger::DEBUG_INFO,TEXT("deviceScanThread no exit\r\n")));
        } else {
            LDEGMSGW((CLogger::DEBUG_INFO,TEXT("deviceScanThread exit\r\n")));
        }
    }
    if (NULL != m_hDevScanEvent) {
        CloseHandle(m_hDevScanEvent);
    }
    if (NULL != m_hDevScanExitEvent) {
        CloseHandle(m_hDevScanExitEvent);
    }
    if(m_pLog)      delete m_pLog;
#endif

    DestoryCheckDlg();
    return TRUE;
}
#if 0 
void CPCBAToolDlg::OnBnClickedOk()
{
    if(!IsEnableConfig()) return ;
    if(IsBusy())    return ;
    if(AppExit(TRUE)) {
        CDialog::OnOK();
    }
}
#endif
void CPCBAToolDlg::OnClose()
{
    if(!IsEnableConfig()) return ;
    if(IsBusy())    return ;
    if(AppExit(TRUE)) {
        CDialog::OnOK();
    }
}
void CPCBAToolDlg::OnCancel()
{
    if(!IsEnableConfig()) return ;
    if(IsBusy())    return ;
    if(AppExit(TRUE)) {
        CDialog::OnOK();
    }
}

BOOL CPCBAToolDlg::IsEnableConfig()
{
#if 0
    if(m_pWorkThread) return FALSE;
	if(m_Configs.bAutoTest&&m_bStarWrite) return FALSE;
    if(!bAdmin) false;
#endif
    BOOL bBusy = IsBusy();
	if (bBusy)
	{
		return FALSE;
	}
	if(!bAdmin) 
	{
		return FALSE;
	}
    return TRUE;	
}
VOID CPCBAToolDlg::SpreadX(LPCRECT rct,int idx_start,int idx_cnt)
{
    DWORD dwLen;
    CRect rc2,rc3;
    int    cnt_end;
    if(0 == idx_cnt) return ;
    dwLen  = rct->right  - rct->left;
    dwLen /= idx_cnt;
    cnt_end= idx_start + idx_cnt;
    rc2.SetRect(rct->left,rct->top,rct->left + dwLen,rct->bottom);
    for(int i = idx_start; i < cnt_end; i ++) {
        rc3.CopyRect(rc2);
        rc3.InflateRect(-3,-3,-3,-3);
        if(m_CheckDlg[i]) {
            m_CheckDlg[i]->MoveWindow(rc3.left,rc3.top,rc3.Width(),rc3.Height());
        }
        rc2.OffsetRect(dwLen,0);
    }

}
VOID CPCBAToolDlg::ReSizeCheckDlg(LPCRECT rct)
{
    CRect rc,rc2/*,rc3 **/;
    DWORD dwHigh;
    int   line1;
    rc.CopyRect(rct);
    rc.InflateRect(-0,-40,0,0);
    if(4 > DLG_CNT){ 
        SpreadX(rc,0,DLG_CNT);
    } else {
        dwHigh = rc.Height()/2;
        line1 = DLG_CNT/2 + (DLG_CNT%2?1:0);

        rc2.SetRect(rc.left,rc.top,rc.right,rc.top + dwHigh );
        /*rc3.CopyRect(rc2);
        rc3.InflateRect(0,-2,0,-2);**/
        SpreadX(rc2,0,line1);

        rc2.OffsetRect(0,dwHigh);
        /*rc3.CopyRect(rc2);
        rc3.InflateRect(0,-2,0,-2); **/
        SpreadX(rc2,line1,DLG_CNT - line1);
    }
}
BOOL CPCBAToolDlg::ShowCheckDlg(BOOL bInit)
{
    CRect rc;
    if(bInit) {
        for(int i = 0; i < DLG_CNT; i ++) {
            m_CheckDlg[i] = new CCheckDlg(m_Configs,m_LocalLang,m_pLog,this);
            if(m_CheckDlg[i]) {
                m_CheckDlg[i]->Create(CCheckDlg::IDD,this);
            }

        }
    }
    /*resize **/
    GetClientRect(rc);
    ReSizeCheckDlg(rc);
    return TRUE;

}
BOOL CPCBAToolDlg::DestoryCheckDlg()
{
    for(int i = 0; i < DLG_CNT; i ++) {
        if(m_CheckDlg[i]) {
            m_CheckDlg[i]->DestroyWindow();
            delete m_CheckDlg[i];
            m_CheckDlg[i] = NULL;
        }
    }
    return TRUE;
}
BOOL CPCBAToolDlg::IsBusy()
{
    /*Ask child window **/
    /*if(m_pGenThread) return TRUE; **/
    for(int i = 0; i < DLG_CNT; i ++) {
        if(m_CheckDlg[i]) {
            if(m_CheckDlg[i]->SendMessage(WM_ASKBUY,0,0)) return TRUE;
        }
    }

    return FALSE;
}


void CPCBAToolDlg::OnBnClickedButtonStart()
{
#if 1

    if(m_bStart) {
        BOOL bBusy = IsBusy();
        if(bBusy && AfxMessageBox(GetLocalString(TEXT("IDS_FORCE_STOP")).c_str(),MB_YESNO) != IDYES) {
            return;
        }
        m_bStart = FALSE;
        if(/*bBusy **/1) {
            /*GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE); **/
            /*push stop **/
            for (int i=0;i < DLG_CNT;i++) {
                if(m_CheckDlg[i]) {
                    m_CheckDlg[i]->SendMessage(WM_USER_ACTION,ACTION_STOP,0);
                }
            }
            SetDlgItemText(IDC_BUTTON_START,GetLocalString(TEXT("IDS_START")).c_str());
        } 
        return;
    }
    /*notify all check windows to start test **/
    m_bStart = TRUE;
    SetDlgItemText(IDC_BUTTON_START,GetLocalString(TEXT("IDS_STOP")).c_str());
    for (int i=0;i<DLG_CNT;i++) {
        if(m_CheckDlg[i]) {
            m_CheckDlg[i]->SendMessage(WM_USER_ACTION,ACTION_START,0);
        }
    }
#else 
    // TODO: Add your control notification handler code here
    if( /*m_Configs.bAutoTest**/(1 < DLG_CNT) ||m_Configs.bAutoTest) {
        if(m_bStart) {
            BOOL bBusy = IsBusy();
            if(bBusy) {
                GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
                /*push stop **/
                for (int i=0;i<DLG_CNT;i++) {
                    m_CheckDlg[i]->SendMessage(WM_USER_ACTION,ACTION_STOP,0);
                }
                SetDlgItemText(IDC_BUTTON_START,GetLocalString(TEXT("IDS_STOPING")).c_str());
            } else {
                m_bStart = FALSE;
                SetDlgItemText(IDC_BUTTON_START,GetLocalString(TEXT("IDS_START")).c_str());
            }
            return ;
        } else {
            m_bStart = TRUE;
            SetDlgItemText(IDC_BUTTON_START,GetLocalString(TEXT("IDS_STOP")).c_str());
            OnStartTest(TRUE);
        }
    } else {
        if(m_bStart) {
            BOOL bBusy = IsBusy();
            if(bBusy) {
                m_bUserStop = TRUE;
                if(m_CheckDlg[0]) {
                    m_CheckDlg[0]->SendMessage(WM_USER_ACTION,ACTION_STOP,0);
                }
                SetDlgItemText(IDC_BUTTON_START,_T("STOPING"));
            } else {
                m_bStart = FALSE;
                if(OnStartTest(FALSE)) {
                    SetDlgItemText(IDC_BUTTON_START,GetLocalString(TEXT("IDS_START")).c_str());
                }
            }
        } else {
            m_bStart = TRUE;
            SetDlgItemText(IDC_BUTTON_START,GetLocalString(TEXT("IDS_STOP")).c_str());
            OnStartTest(FALSE);
        }
    }
#endif
}
BOOL CPCBAToolDlg::OnStartTest(BOOL bAuto)
{

#if 0
    for (int i=0;i<DLG_CNT;i++) {
        m_CheckDlg[i]->SendMessage(WM_USER_ACTION,ACTION_START,0);
    }
#endif
    m_csPortChange.Lock();
    if(!(m_ports.IntelPort[PORT_GTI].empty()||m_ports.IntelPort[PORT_AT].empty())){
        m_CheckDlg[0]->SendMessage(WM_USER_ACTION,ACTION_START,0);
    }
    m_csPortChange.Unlock();
    return TRUE;

}

std::wstring CPCBAToolDlg::GetLocalString(std::wstring strKey)
{
#if 1
	 return m_LocalLang.GetLanStr(strKey);
#else 
	return strKey;
#endif
}

void CPCBAToolDlg::OnUpdateSetting(CCmdUI *pCmdUI)
{
    // TODO: Add your command update UI handler code here
    pCmdUI->Enable(IsEnableConfig());

}
BOOL CPCBAToolDlg::OnDeviceChange(UINT nEventType,DWORD_PTR dwData)
{
    PDEV_BROADCAST_HDR  lpdb        = (PDEV_BROADCAST_HDR)dwData;
    UINT                uiEvent     = nEventType;
    if ((uiEvent==DBT_DEVICEARRIVAL)||(uiEvent==DBT_DEVICEREMOVECOMPLETE)||(uiEvent==0x7)) {
        SetEvent(m_hDevScanEvent);
    }
    return 0;
}
void CPCBAToolDlg::OnSetting()
{
    // TODO: Add your command handler code here
    CPtConfigDlg PtConfigDlg(m_Configs,m_LocalLang);
    if(IDOK==PtConfigDlg.DoModal()){
        /*UpdateUIStatus(0); **/
    }

}
void CPCBAToolDlg::ScanThreadProc()
{
    DWORD       dwResult;
    BOOL        bChange;
#if ENUM_SER
    EnumSerial  *mpEnumSerial;
#else 
    INTELCOMPORT ports;
#endif
    HANDLE      hEventArray[2];
    hEventArray[0]   = m_hDevScanEvent;
    hEventArray[1]   = m_hDevScanExitEvent;
    while (TRUE) {
        dwResult = WaitForMultipleObjects(2,hEventArray,FALSE,INFINITE);
        if(WAIT_OBJECT_0 == dwResult ) {
            bChange = FALSE;
            if(CheckIntelUSBComPort(&ports)) {
                LDEGMSGW((CLogger::DEBUG_INFO,TEXT("GTI = %s"),ports.IntelPort[PORT_GTI].c_str()));
                LDEGMSGW((CLogger::DEBUG_INFO,TEXT("AT  = %s"),ports.IntelPort[PORT_AT].c_str()));
            }

            m_csPortChange.Lock();
            for(int i = 0 ; i < PORT_PORT_NUM;i++  ) {
                LDEGMSGW((CLogger::DEBUG_INFO,TEXT("m_ports %s;ports %s"),m_ports.IntelPort[i].c_str(),ports.IntelPort[i].c_str()));
                if(0 != m_ports.IntelPort[i].compare(ports.IntelPort[i])) {
                    m_ports.IntelPort[i]= ports.IntelPort[i];
                    bChange             = TRUE;
                    LDEGMSGW((CLogger::DEBUG_INFO,TEXT("Change")));
                } else {
                    LDEGMSGW((CLogger::DEBUG_INFO,TEXT("No Change")));
                }
            }
            m_csPortChange.Unlock();
#if 0
            if(!(m_ports.IntelPort[PORT_GTI].empty()||m_ports.IntelPort[PORT_AT].empty())){
                if (bChange&&m_Configs.bAutoTest&&m_bStarWrite) {
                    OnStartTest(TRUE);
                }
            }
#else 

#endif
            if(bChange){
                if(m_CheckDlg[0]) {
                    if(m_ports.IntelPort[PORT_AT].empty()||m_ports.IntelPort[PORT_GTI].empty()) {
                        m_CheckDlg[0]->PostMessage(WM_UPDATE_COM,WM_COM_RM,0);
                    } else {
                        short nAt  =  _ttoi(m_ports.IntelPort[PORT_AT].c_str());
                        short nGti =  _ttoi(m_ports.IntelPort[PORT_GTI].c_str());
                        m_CheckDlg[0]->PostMessage(WM_UPDATE_COM,WM_COM_ADD,nAt<<16|nGti);
                    }
                    /*
                    if((m_Configs.bAutoTest&&m_bStart)||((1 < DLG_CNT)&&m_bStart)) {
                        m_CheckDlg[0]->PostMessage(WM_USER_ACTION,ACTION_START,nAt<<16|nGti);
                    }
                    **/
                }
            }

        } else  if ((WAIT_OBJECT_0 + 1 )== dwResult) {
            LDEGMSGW((CLogger::DEBUG_INFO,TEXT("ScanThreadProc exit")));
            break;
        } else {
        }
    }
#if ENUM_SER
    if (NULL != mpEnumSerial) {
        delete mpEnumSerial;
        mpEnumSerial = NULL;
    }
#endif
}

void CPCBAToolDlg::OnHelpOpenlogfolder()
{
    // TODO: Add your command handler code here
    ShellExecute(NULL, _T("open"), m_Configs.strLogPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void CPCBAToolDlg::OnSize(UINT nType, int cx, int cy)
{
    CRect rc;
    CDialog::OnSize(nType, cx, cy);
    // TODO: Add your message handler code here
    if(SIZE_RESTORED == nType ||SIZE_MAXIMIZED == nType){
        rc.SetRect(0,0,cx,cy);
        ReSizeCheckDlg(rc);
    } else  {


    }
}

void CPCBAToolDlg::OnHelpAbout()
{
    // TODO: Add your command handler code here
    CAboutDlg dlgAbout(m_Configs,m_LocalLang);
    dlgAbout.DoModal();
}

void CPCBAToolDlg::OnSettingLogin()
{
	// TODO: Add your command handler code here
#ifdef USER_LOGIN
	CLoginDlg login(m_Configs,m_LocalLang);
	if(login.DoModal() == IDOK) {
		bAdmin = TRUE;
		if(bAdmin) {
			SetWindowText(((LPCTSTR)m_DlgTitle + GetLocalString(TEXT("IDS_TEXT_ADMIN"))).c_str());
		} else {
			SetWindowText(((LPCTSTR)m_DlgTitle + GetLocalString(TEXT("IDS_TEXT_USER"))).c_str());
		}
	}
#else 
#endif
}

void CPCBAToolDlg::OnUserLogout()
{
	// TODO: Add your command handler code here
	if(bAdmin) {
		bAdmin              = FALSE;
		m_Configs.bLogin    = false;
		if(bAdmin) {
			SetWindowText(((LPCTSTR)m_DlgTitle + GetLocalString(TEXT("IDS_TEXT_ADMIN"))).c_str());
		} else {
			SetWindowText(((LPCTSTR)m_DlgTitle + GetLocalString(TEXT("IDS_TEXT_USER"))).c_str());
		}
	}
}
void CPCBAToolDlg::OnUpdateUserChangepassword(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(bAdmin == TRUE);

}
void CPCBAToolDlg::OnUserChangepassword()
{
	// TODO: Add your command handler code here

#ifdef USER_LOGIN
	CChangePasswordDlg login(m_Configs,m_LocalLang);
	if(login.DoModal() == IDOK) {
		AfxMessageBox(GetLocalString(TEXT("IDS_INFO_CHGPWD_OK")).c_str());
	}
#else 
#endif
}
