// CheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PCBATool.h"
#include "CheckDlg.h"

// Convert wstring to string
static std::string wstr2str(const std::wstring& arg)
{
    int requiredSize;
    requiredSize = WideCharToMultiByte(CP_ACP,0,arg.c_str(),arg.length(),NULL,0,NULL,NULL);
    std::string res;
    if (requiredSize<=0) {
        res = "";
        return res;
    }
    res.assign(requiredSize,'\0');
    WideCharToMultiByte(CP_ACP,0,arg.c_str(),arg.length(),const_cast<char*>(res.data()),requiredSize,NULL,NULL);
    return res;
}
// Convert string to wstring
static std::wstring str2wstr(const std::string& arg)
{
    int requiredSize;
    requiredSize = MultiByteToWideChar(CP_ACP,0,arg.c_str(),arg.length(),NULL,0);
    std::wstring res;
    if (requiredSize<=0) {
        res = L"";
        return res;
    }
    res.assign(requiredSize,L'\0');
    MultiByteToWideChar(CP_ACP,0,arg.c_str(),arg.length(),const_cast<wchar_t *>(res.data()),requiredSize);
    return res;
}
// CCheckDlg dialog

IMPLEMENT_DYNAMIC(CCheckDlg, CDialog)
CCheckDlg::CCheckDlg(CIniSettingBase &Configs,CIniLocalLan &LocalLang, CLogger *pLog /*=NULL*/,CWnd* pParent /*=NULL*/)
	: CDialog(CCheckDlg::IDD, pParent),m_Configs(Configs),m_LocalLang(LocalLang),m_pLog(pLog),m_pWorkThread(NULL),m_Start(FALSE),m_bUserStop(FALSE)
    ,m_bRedLedLight(TRUE),m_nAt(-1),m_nGti(-1)
{
}

CCheckDlg::~CCheckDlg()
{
}

void CCheckDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_STATUS, m_Status);
    DDX_Control(pDX, IDC_STATIC_PORT, m_LabelPort);
    DDX_Control(pDX, IDC_STATIC_LBPORT, m_LabelDevices);
    DDX_Control(pDX, IDC_STATIC_LBSTATUS,m_LabelStatus );
    DDX_Control(pDX, IDC_LIST_REPORT, m_listInfo);
    DDX_Control(pDX, IDC_STATIC_LBSN, m_LabelSn);
    DDX_Control(pDX, IDC_STATIC_SN, m_Sn);
}


BEGIN_MESSAGE_MAP(CCheckDlg, CDialog)
    ON_MESSAGE(WM_ASKBUY,&CCheckDlg::OnHandleAskBusyMsg)
    ON_MESSAGE(WM_USER_ACTION, &CCheckDlg::OnUserAction)
    ON_MESSAGE(WM_UPDATE_COM, &CCheckDlg::OnUpdateCom)
    ON_MESSAGE(WM_UPDATE_MSG,&CCheckDlg::OnHandleUpdateStatusMsg)
    ON_WM_DESTROY()
    ON_WM_PAINT()
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CCheckDlg message handlers
LRESULT CCheckDlg::OnUpdateCom(WPARAM wParam,LPARAM lParam)
{
    CString strCom;
    if(WM_COM_ADD == wParam ) {
        DWORD dwCom = (DWORD)lParam;
        m_nAt = (dwCom&0xffff0000)>>16;
        m_nGti = dwCom&0xffff;
        strCom.Format(TEXT("COM%d(%d)"),m_nGti,m_nAt);
        m_LabelPort.SetText((LPCTSTR)strCom,0xFF0000);
        if(m_Start) {
            OnStartTest(TRUE);
        }
        m_bRedLedLight = FALSE;
        RedrawWindow();
    } else {
        m_nAt = -1;
        m_nGti= -1;
        m_LabelPort.SetText(TEXT(""),0xFF0000);
        m_bRedLedLight = TRUE;
        if(!IsBusy()) {
            PostMessage(WM_UPDATE_MSG,UPDATE_PROMPT,PROMPT_EMPTY);
            PostMessage(WM_UPDATE_MSG,UPDATE_LIST,LIST_EMPTY);
        }
        RedrawWindow();
    }
#if 0
    SetDlgItemText(IDC_EDIT_GTI,m_ports.IntelPort[PORT_GTI].c_str());
    SetDlgItemText(IDC_EDIT_AT,m_ports.IntelPort[PORT_AT].c_str());
#endif
    return FALSE;
}
BOOL CCheckDlg::IsBusy()
{
    if(m_pWorkThread) return TRUE;
    return FALSE;
}
LRESULT CCheckDlg::OnHandleAskBusyMsg(WPARAM wParam,LPARAM lParam)
{
    return IsBusy();
}

LRESULT CCheckDlg::OnUserAction(WPARAM wParam,LPARAM lParam)
{
    if(ACTION_START == wParam) {
        m_Start = TRUE;
        if(IsBusy()){
            /*SetRunStatus(GetLocalString(TEXT("IDS_DETECTING")).c_str(),RGB(0,255,0)); **/
        }
        if(OnStartTest(TRUE)) {
            /*m_bUserStop = FALSE; **/
            /*SetRunStatus(GetLocalString(TEXT("IDS_RUNNING")).c_str(),RGB(0,255,0)); **/
        } else {
            SetRunStatus(GetLocalString(TEXT("")).c_str(),RGB(0,255,0));
        }
    } else if(ACTION_STOP == wParam){
        m_Start = FALSE;
        if(IsBusy()) {
            m_bUserStop = TRUE;
            /*SetRunStatus(GetLocalString(TEXT("IDS_STOPPING")).c_str(),RGB(0,255,0)); **/
        } else {
            SetRunStatus(GetLocalString(TEXT("")).c_str(),RGB(0,255,0));
        }
    } else if(ACTION_FORCE_STOP == wParam){

    }
    return FALSE;
}
BOOL CCheckDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE )
	{
		return TRUE;
	}
	if ( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN )
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
BOOL CCheckDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_hGreenLedBitmap   = m_hRedLedBitmap = NULL;
    m_hRedLedBitmap     = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BMP_REDLED));
    m_hGreenLedBitmap   = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BMP_GREENLED));
    InitCtrl();

	//GetLocalString(TEXT("LANG:IDS_APPNAME"));
    return TRUE;

}
UINT TestDeviceThread(LPVOID lpParam)
{
	CCheckDlg *pDlg  = (CCheckDlg*)lpParam;
    return pDlg->ThisTestThreadProc();
}

VOID CCheckDlg::ParseTestResult(std::string strResult)
{
	std::string::size_type  posl1,posl2,posl3,posr;
	std::string strItem,strItemName,strTestStatus;
	CString strPrompt;
	if (strResult.empty())
	{
		return;
	}
	posl1 = strResult.find_first_of("{")+1;
	posr = strResult.find_first_of(";",posl1);
	while (std::string::npos != posr)
	{
		strItem = strResult.substr(posl1,posr-posl1);
		posl2 = strItem.find_first_of("=");
		strItemName = strItem.substr(0,posl2);
		strTestStatus = strItem.substr(posl2+1);
		if (!strTestStatus.compare("ST_PASS")||!strTestStatus.compare("ST_DONE"))
		{
			strPrompt.Format(_T("%s:%s"),CSettingBase::str2wstr(strItemName).c_str(),_T("通过"));
			AddPrompt(strPrompt,LIST_INFO);
		}
		else if (!strTestStatus.compare("ST_FAIL"))
		{
			strPrompt.Format(_T("%s:%s"),CSettingBase::str2wstr(strItemName).c_str(),_T("失败"));
			AddPrompt(strPrompt,LIST_ERR);
		}
		else 
		{
			strPrompt.Format(_T("%s:%s"),CSettingBase::str2wstr(strItemName).c_str(),_T("测试中..."));
			AddPrompt(strPrompt,LIST_INFO);
		}
		posl1 = posr+1;
		posr = strResult.find_first_of(";",posl1);
	}
}
BOOL CCheckDlg::ThisTestThreadProc()
{
    BOOL        bHavePrintTestPrompt    = FALSE; /*used for first time entering test status **/
    STATUS_DATA ptest_status            = {0};
    BOOL        bRet;
    CLogger     *logger                 = NULL;
    std::string strDevSn;
    std::string strPcbSn;
	std::string strTemp;
	std::string	strResult,strErrCode;
	std::string	strResultRouteName,strResultiInfoMsg,strResultRtnMsg;
    CString     strPrompt;
    CTarget     dut;
    BOOL        bSuccess                = FALSE;
    int         iRetry; 
    int         RealCom;
    bool        get_pass[20]            = {0};
    if (m_listInfo.GetCount()>0) {
        PostMessage(WM_UPDATE_MSG,UPDATE_LIST,LIST_EMPTY);
    }
    PostMessage(WM_UPDATE_MSG,UPDATE_TEST_ENTER);
    PostMessage(WM_UPDATE_MSG,UPDATE_PROMPT,PROMPT_TESTING);
	//GetLocalString(TEXT("LANG:IDS_APPNAME"));

    /*start...**/
    if(m_Configs.nLogLevel == DLEVEL_DEBUG) {
        logger = CLogger::StartLog((m_Configs.strLogPath + TEXT("Checking_") + CLogger::TimeStr(true, true) ).c_str(), CLogger::DEBUG_ALL);   
        dut.StartLog(logger);
    } else {
        dut.StartLog(m_pLog);
    }
	dut.SetDeviceType(m_Configs.nDeviceType);

    //Sleep(5000);
    //1.connect to device
    RealCom = m_nGti;
    iRetry = 0;
    strPrompt.Format(GetLocalString(_T("IDS_INFO_CONN_DEVICE")).c_str(),RealCom);
    AddPrompt(strPrompt,LIST_INFO);
    while( iRetry < 6 ){
        iRetry ++;
		RealCom = m_nGti;
        if( -1 != RealCom) {
            /*connect doing **/
            if (ConnectDUT(dut,RealCom,TRUE)) {
                break;
            }
        }
        if (m_bUserStop) {
            goto exittestthreadproc;
        }    
		Sleep(1000);
    }
    /*LOGER(); **/
    if(6 == iRetry) {
        AddPrompt(GetLocalString(_T("IDS_ERROR_CONN_FAIL")).c_str(),LIST_ERR);
		goto exittestthreadproc;
    }

    //AddPrompt(GetLocalString(TEXT("IDS_INFO_READPCBSN")).c_str(),LIST_INFO);
    //bRet = dut.ReadSn(strPcbSn, strDevSn);
    //if(!bRet) {
    //    LDEGMSG((CLogger::DEBUG_INFO,"ReadSn fail\r\n"));
    //    AddPrompt(GetLocalString(TEXT("IDS_ERROR_READSN")).c_str(),LIST_ERR);
    //    goto exittestthreadproc;
    //}
    //strPrompt.Format(TEXT("SN:%s"),CSettingBase::str2wstr(strPcbSn).c_str());
    //AddPrompt(strPrompt,LIST_INFO);
    if (m_Configs.bDataBase)
    {
      
    }
	//start testing
	AddPrompt(GetLocalString(TEXT("IDS_INFO_PCBS_START")).c_str(),LIST_INFO);
	if (!dut.StartTest(strTemp))
	{
			LDEGMSG((CLogger::DEBUG_INFO,"StartTest fail\r\n"));
			strErrCode = "406";
			goto exittestthreadproc;
	}
	//strPrompt.Format(_T("%s"),CSettingBase::str2wstr(strTemp).c_str());
	//AddPrompt(strPrompt,LIST_INFO);
	
	//get test status
	iRetry = 0;
	bool flag = true;
	while(1)
	{
		if (m_bUserStop) {
			goto exittestthreadproc;
		} 
		if (!dut.QueryStatus(strTemp))
		{
			LDEGMSG((CLogger::DEBUG_INFO,"QueryStatus fail\r\n"));
			AddPrompt(GetLocalString(_T("IDS_ERROR_GETTESTSTATUS_FAIL")).c_str(),LIST_ERR);
            strErrCode = "407";
			goto exittestthreadproc;
		}
		if (strstr(strTemp.c_str(),"{STATUS=PCBS_STARTING}") != 0)
		{
			if (flag)
			{
				AddPrompt(GetLocalString(TEXT("IDS_INFO_START_TESTING")).c_str(),LIST_INFO);
				flag = false;
			}
			Sleep(100);
			continue;
		}
        else if(strstr(strTemp.c_str(),"{STATUS=ST_TESTING}") != 0)
        {
            ++iRetry;
        }
		else if(strstr(strTemp.c_str(),"{STATUS=ST_PASS}") != 0)
		{
				break;
		}
		else if(strstr(strTemp.c_str(),"{STATUS=ST_FAIL}") != 0)
		{
			goto exittestthreadproc;
		}
		else
		{
            iRetry = 0;
			ParseTestResult(strTemp);
			if (m_Configs.bEndIfFail)
			{
				if (strstr(strTemp.c_str(),"ST_FAIL") !=0)
				{
						goto exittestthreadproc;
				}
			}
		}
		Sleep(100);
        if (iRetry > 50)        //5秒
        {
            goto exittestthreadproc;
        }
	}
    bSuccess = TRUE;
exittestthreadproc:

    if(m_bUserStop) {
        m_bUserStop = FALSE;
        PostMessage(WM_UPDATE_MSG,UPDATE_PROMPT,PROMPT_ABORT);
        strPrompt.Format(GetLocalString(TEXT("IDS_INFO_USER_ABORT")).c_str());
        AddPrompt(strPrompt,LIST_ERR);
    } else if (bSuccess) {
        AddPrompt(GetLocalString(TEXT("IDS_INFO_ALL_TEST_PASS")).c_str(),LIST_INFO);
        PostMessage(WM_UPDATE_MSG,UPDATE_PROMPT,PROMPT_PASS);
    } else {
        PostMessage(WM_UPDATE_MSG,UPDATE_PROMPT,PROMPT_FAIL);
        AddPrompt(GetLocalString(TEXT("IDS_INFO_ALL_TEST_FAIL")).c_str(),LIST_ERR);
    }
    if (m_Configs.bDataBase)
    {
       
    }
    //if (bSuccess)
    //{
    //    if (m_Configs.bReboot)
    //    {
    //        AddPrompt(GetLocalString(TEXT("IDS_INFO_PCBA_REBOOT")).c_str(),LIST_INFO);
    //        if (!dut.EndTest())
    //        {
    //            LDEGMSG((CLogger::DEBUG_INFO,"EndTest fail\r\n"));
    //        }
    //    }
    //}
    if (bSuccess)
    {
        if (m_Configs.bReboot)
        {
            AddPrompt(GetLocalString(TEXT("IDS_INFO_PCBA_REBOOT")).c_str(),LIST_INFO);
            dut.SetMode(MODE_NORMAL);
            Sleep(1000);
            dut.reboot();
            Sleep(1000);
            AddPrompt(GetLocalString(TEXT("IDS_INFO_REMOVE_DEVICE")).c_str(),LIST_INFO);
        }
        else
        {
            if (!dut.EndTest())
            {
                LDEGMSG((CLogger::DEBUG_INFO,"EndTest fail\r\n"));
            }
        }
    }
    dut.StopLog();
    if(logger) delete logger;

    m_pWorkThread = NULL;
    /*exit will set button status,so we must set m_pWorkThread = NULL before **/
    PostMessage(WM_UPDATE_MSG,UPDATE_TEST_EXIT);
    return TRUE;
}
BOOL CCheckDlg::TimerElapseOK(DWORD ticks,DWORD LastTicks)
{
    DWORD       Curticks;
    Curticks    = GetTickCount();
    if(Curticks >= LastTicks ) {
        return (Curticks - LastTicks) > ticks;
    } else {
        return (0xFFFFFFFF - LastTicks + (Curticks + 1)) > ticks;
    }
}
/*
paramters:
 bPrint
 TRUE:
 FALSE
return:
 TRUE:all test case valid 
 FALSE:not all test valid

**/
BOOL CCheckDlg::parse_status(STATUS_DATA *ptest_status,BOOL bPrint)
{
    CString strPrompt;
    BOOL    bRet = TRUE;
    if(0 == ptest_status->sim) {
        bRet = FALSE;
        if(bPrint) {
            strPrompt.Format(GetLocalString(TEXT("IDS_INFO_%s_FAIL")).c_str(),TEXT("SIM"));
            AddPrompt(strPrompt,LIST_ERR);
        }
    } else {
        if(bPrint) {
            strPrompt.Format(GetLocalString(TEXT("IDS_INFO_%s_PASS")).c_str(),TEXT("SIM"));
            AddPrompt(strPrompt,LIST_INFO);
        }
    }
    if(0 == ptest_status->wifi) {
        bRet = FALSE;
        if(bPrint) {
            strPrompt.Format(GetLocalString(TEXT("IDS_INFO_%s_FAIL")).c_str(),TEXT("WIFI"));
            AddPrompt(strPrompt,LIST_ERR);
        }
    } else {
        if(bPrint) {
            strPrompt.Format(GetLocalString(TEXT("IDS_INFO_%s_PASS")).c_str(),TEXT("WIFI"));
            AddPrompt(strPrompt,LIST_INFO);
        }
    }
    if(0 == ptest_status->snesor) {
        bRet = FALSE;
        if(bPrint) {
            strPrompt.Format(GetLocalString(TEXT("IDS_INFO_%s_ITEM")).c_str(),TEXT("SENSOR"));
            AddPrompt(strPrompt,LIST_ERR);
        }
    } else {
        if(bPrint) {
            strPrompt.Format(GetLocalString(TEXT("IDS_INFO_%s_PASS")).c_str(),TEXT("SENSOR"));
            AddPrompt(strPrompt,LIST_INFO);
        }
    }
    if(0 == ptest_status->bt) {
        bRet = FALSE;
        if(bPrint) {
            strPrompt.Format(GetLocalString(TEXT("IDS_INFO_%s_FAIL")).c_str(),TEXT("BT"));
            AddPrompt(strPrompt,LIST_ERR);
        }
    } else {
        if(bPrint) {
            strPrompt.Format(GetLocalString(TEXT("IDS_INFO_%s_PASS")).c_str(),TEXT("BT"));
            AddPrompt(strPrompt,LIST_INFO);
        }
    }
    if(0 == ptest_status->codec) {
        bRet = FALSE;
        if(bPrint) {
            strPrompt.Format(GetLocalString(TEXT("IDS_INFO_%s_FAIL")).c_str(),TEXT("CODEC"));
            AddPrompt(strPrompt,LIST_ERR);
        }
    } else {
        if(bPrint) {
            strPrompt.Format(GetLocalString(TEXT("IDS_INFO_%s_PASS")).c_str(),TEXT("CODEC"));
            AddPrompt(strPrompt,LIST_INFO);
        }
    }
    if(0 == ptest_status->gps) {
        bRet = FALSE;
        if(bPrint) {
            strPrompt.Format(GetLocalString(TEXT("IDS_INFO_%s_FAIL")).c_str(),TEXT("GPS"));
            AddPrompt(strPrompt,LIST_ERR);
        }
    } else {
        if(bPrint) {
            strPrompt.Format(GetLocalString(TEXT("IDS_INFO_%s_PASS")).c_str(),TEXT("GPS"));
            AddPrompt(strPrompt,LIST_INFO);
        }
    }
    if(0 == ptest_status->camera) {
        bRet = FALSE;
        if(bPrint) {
            strPrompt.Format(GetLocalString(TEXT("IDS_INFO_%s_FAIL")).c_str(),TEXT("CAMERA"));
            AddPrompt(strPrompt,LIST_ERR);
        }
    } else {
        if(bPrint) {
            strPrompt.Format(GetLocalString(TEXT("IDS_INFO_%s_PASS")).c_str(),TEXT("CAMERA"));
            AddPrompt(strPrompt,LIST_INFO);
        }
    }
    return bRet;
}

BOOL CCheckDlg::OnStartTest(BOOL bAuto)
{
    CString szPromt;
    BOOL    isDevExist = FALSE;
    if(m_pWorkThread) {
        return FALSE; /*app is running **/
    }
    if(-1 == m_nAt||-1 == m_nGti)  {
        return FALSE; /*app is running **/
    }

    m_pWorkThread = AfxBeginThread(TestDeviceThread,this);
    if(NULL == m_pWorkThread) 
        goto onstarttestexit;
    return TRUE;
onstarttestexit:
    if(!szPromt.IsEmpty()) {
        if(bAuto) {
            AddPrompt(szPromt,LIST_ERR);
        } else {
            AfxMessageBox(szPromt);
        }
    }
    return FALSE;
}
VOID CCheckDlg::AddPrompt(CString strPrompt,int flag)
{
	PSTRUCT_LIST_LINE   pLine=NULL;
	SYSTEMTIME          curTime;

    LDEGMSGW((CLogger::DEBUG_INFO,(LPCTSTR)strPrompt));
	GetLocalTime( &curTime );
	pLine       = new STRUCT_LIST_LINE;
	if (!pLine) {
		return;
	}
    wsprintf(pLine->pszLineText,
        _T("%02d:%02d:%02d %03d\t%s "),
        curTime.wHour,curTime.wMinute,curTime.wSecond,curTime.wMilliseconds,
        (LPCTSTR)strPrompt);
    pLine->flag = flag;
    if(0 == PostMessage(WM_UPDATE_MSG,UPDATE_LIST,(LPARAM)pLine)) {
        delete pLine;
    }
    return;

}

std::wstring CCheckDlg::GetLocalString(std::wstring strKey)
{
    return m_LocalLang.GetLanStr(strKey);

}
VOID CCheckDlg::SetRunStatus(const TCHAR *szText,DWORD dwColor)
{
    /*m_Status.SetText(szText,COLORREF(dwColor)); **/
    m_Status.SetTextColor(dwColor);
    m_Status.SetWindowText(szText);
    m_Status.RedrawWindow();

}
LRESULT CCheckDlg::OnHandleUpdateStatusMsg(WPARAM wParam,LPARAM lParam)
{
    switch(wParam) {
        case UPDATE_TEST_EXIT:
            if(m_Start){
                /*SetRunStatus(GetLocalString(TEXT("IDS_DETECTING")).c_str(),RGB(0,255,0)); **/
            } else {
                /*SetRunStatus(GetLocalString(TEXT("IDS_IDLE")).c_str(),RGB(0,255,0)); **/
            }
            break;


        case UPDATE_PROMPT:
            if (lParam == PROMPT_TESTING) {
                SetRunStatus(GetLocalString(TEXT("IDS_TEXT_TEST")).c_str(),RGB(0,0,0));
            } else if (lParam==PROMPT_PASS) {
                SetRunStatus(GetLocalString(TEXT("IDS_TEXT_SUCCESS")).c_str(),RGB(0,255,0));
            } else if(lParam==PROMPT_FAIL) {
                SetRunStatus(GetLocalString(TEXT("IDS_TEXT_FAIL")).c_str(),RGB(255,0,0));
            } else if(lParam==PROMPT_ABORT) {
                SetRunStatus(GetLocalString(TEXT("ABORT")).c_str(),RGB(255,0,0));
            } else {
                SetRunStatus(GetLocalString(TEXT("")).c_str(),RGB(255,0,0));
            }
            break;
        case UPDATE_TEST_ENTER:
            /*SetRunStatus(GetLocalString(TEXT("IDS_RUNNING")).c_str(),RGB(0,255,0)); **/
            break;
        case UPDATE_LIST:
            if (lParam == LIST_EMPTY) {
                m_listInfo.ResetContent();
            } else {
                PSTRUCT_LIST_LINE pLine = (PSTRUCT_LIST_LINE)lParam;
                if (pLine->flag == LIST_INFO) {
                    m_listInfo.AddLine(CXListBox::White,CXListBox::Black,pLine->pszLineText); 
                } else if (pLine->flag == LIST_TIME) {
                    m_listInfo.AddLine(CXListBox::White,CXListBox::Purple,pLine->pszLineText);
                } else if (pLine->flag == LIST_WARN){ 
                    m_listInfo.AddLine(CXListBox::Black,CXListBox::Yellow,pLine->pszLineText);
                } else {
                    m_listInfo.AddLine(CXListBox::White,CXListBox::Red,pLine->pszLineText);
                }
                m_listInfo.SetCurSel(m_listInfo.GetCount()-1);
                delete pLine;
            }
            break;
    }

    return 0;
}

void CCheckDlg::OnDestroy()
{
    CDialog::OnDestroy();
    // TODO: Add your message handler code here
    if (m_hRedLedBitmap) {
        DeleteObject(m_hRedLedBitmap);
        m_hRedLedBitmap = NULL;
    }

    if (m_hGreenLedBitmap) {
        DeleteObject(m_hGreenLedBitmap);
        m_hGreenLedBitmap = NULL;
    }

}

void CCheckDlg::OnPaint()
{
#if 0
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    // Do not call CDialog::OnPaint() for painting messages
#else 
    if (IsIconic()) {
#if 0
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
#endif
	} else {
        CPaintDC dc(this); // device context for painting
        HBITMAP hLedBitmap;
        if (m_bRedLedLight) {
            hLedBitmap = m_hRedLedBitmap;
        } else
            hLedBitmap = m_hGreenLedBitmap;
        // TODO: Add your message handler code here
        if (hLedBitmap) {
            CPaintDC dc(GetDlgItem(IDC_PICTURE_DEVICE));
            CDC ImageDC;
            ImageDC.CreateCompatibleDC(&dc);
            HGDIOBJ hOldGdiObject;
            hOldGdiObject = ImageDC.SelectObject(hLedBitmap);
            int nDstWidth,nDstHeight;
            RECT dstClientRect;
            dc.GetWindow()->GetClientRect(&dstClientRect);
            nDstHeight  = dstClientRect.bottom - dstClientRect.top;
            nDstWidth   = dstClientRect.right-dstClientRect.left;
            BITMAP  bmpStruct;
            BOOL        bRet;
            GetObject(hLedBitmap,sizeof(BITMAP),&bmpStruct);
            bRet = TransparentBlt(dc.m_hDC, 0, 0, nDstWidth, nDstHeight, ImageDC.m_hDC, 0, 0, bmpStruct.bmWidth, bmpStruct.bmHeight, RGB(255,255,255));
            ImageDC.SelectObject(hOldGdiObject);
        }
        CDialog::OnPaint();
	}
#endif
}

void CCheckDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    if(SIZE_RESTORED == nType ||SIZE_MAXIMIZED == nType){
        CheckDlgReSize(cx, cy);
    } else  {


    }
}
VOID CCheckDlg::CheckDlgReSize(int cx, int cy)
{

#define PIC_WITH 200
#define PIC_CTL 100
#define STA_CTL 180
#define PIC_SPN 40
    CRect rc,rc1,rc2,rclist,rctl;
    rc.SetRect(0,0,cx,cy);
#if 0
    if(1 < m_Configs.nMultiPort) {
        CWnd *hWndPic = GetDlgItem(IDC_PICTURE_DEVICE);
        if(hWndPic) {
            hWndPic->GetWindowRect(&rc1);
            ScreenToClient(&rc1);
            int x,y;
            x = (rc.Width() - rc1.Width())/ 2;
            rc1.OffsetRect(x - rc1.left,0);
            hWndPic->MoveWindow(rc1);
        } else {
            return ;
        }

        hWndPic = GetDlgItem(IDC_STATIC_STATUS);
        if(hWndPic) {
            hWndPic->GetWindowRect(&rc1);
            ScreenToClient(&rc1);
            int x,y;
            x = (rc.Width() - rc1.Width())/ 2;
            rc1.OffsetRect(x - rc1.left,0);
            hWndPic->MoveWindow(rc1);
        }

        hWndPic = GetDlgItem(IDC_LIST_REPORT);
        if(hWndPic) {
            hWndPic->GetWindowRect(&rc1);
            ScreenToClient(&rc1);
            int x,y;
            x = (rc.Width() - rc1.Width())/ 2;
            rc1.OffsetRect(x - rc1.left,0);
            hWndPic->MoveWindow(rc1);
        }
    } else 

    {
        rc1left.SetRect(PIC_SPN,0,PIC_SPN + PIC_WITH,cy);
        CWnd *hWndPic = GetDlgItem(IDC_PICTURE_DEVICE);
        if(hWndPic) {
            int y = (rc.Height() - PIC_WITH)/2;
            rc1.SetRect(rc1left.left,y,rc1left.left + PIC_WITH,y+PIC_WITH);
            hWndPic->MoveWindow(rc1);
        } else {
            return ;
        }
        hWndPic = GetDlgItem(IDC_STATIC_STATUS);
        if(hWndPic) {
            hWndPic->GetWindowRect(&rc1);
            ScreenToClient(&rc1);
            int y = (rc.Height() - PIC_WITH)/2;
            x = (rc.Width() - rc1.Width())/ 2;
            rc1.OffsetRect(x - rc1.left,0);
            hWndPic->MoveWindow(rc1);
        }



    }
#else 
    {
#if 1
        CWnd *hWndPic = GetDlgItem(IDC_PICTURE_DEVICE);
        if(hWndPic) {
            int x = (PIC_WITH - PIC_CTL)/ 2;
            rc1.SetRect(x,100,x +PIC_CTL,100 + PIC_CTL);
            hWndPic->MoveWindow(rc1);
        } else {
            return ;
        }

        hWndPic = GetDlgItem(IDC_STATIC_STATUS);
        if(hWndPic){
            int x = (PIC_WITH - STA_CTL)/ 2;
            rc2.SetRect(x,rc1.bottom,x + STA_CTL,rc.bottom);
            hWndPic->MoveWindow(rc2,1);
            /*m_Status.MoveWindow(rc2); **/
            hWndPic->RedrawWindow();
        }
#endif
    }
    rclist.SetRect(200,5,cx - 5,cy - 5);
    CWnd *hWndPic = GetDlgItem(IDC_LIST_REPORT);
    if(hWndPic) {
        hWndPic->MoveWindow(rclist);
    }
#endif

}

VOID CCheckDlg::InitCtrl()
{
	CFont font;



    m_LabelDevices.SetText(TEXT("Devices:"),0xFF0000);
	m_LabelDevices.SetFontSize(14);
	m_LabelDevices.SetFontBold(TRUE);

    m_LabelPort.SetText(TEXT(""),0xFF0000);
	m_LabelPort.SetFontSize(14);
    m_LabelPort.SetBkColor(0xAAAAAAAA);
	m_LabelPort.SetFontBold(TRUE);

    m_LabelStatus.SetText(TEXT("Status:"),0xFF0000);
	m_LabelStatus.SetFontSize(14);
	m_LabelStatus.SetFontBold(TRUE);

#if 0
    m_Status.SetText(TEXT(""),0xFF0000);
	m_Status.SetFontSize(14);
    /*m_Status.SetBkColor(0xAAAAAAAA); **/
	m_Status.SetFontBold(TRUE);
#else 
    m_Status.SetFontStatic(_T("宋体"),60,RGB(0,0,0),FS_BOLD|FS_CENTER|FS_VCENTER);
#endif

    font.CreateFont(-13,10,0,0,FW_NORMAL,FALSE,FALSE,0,  
        ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,  
        DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,_T("宋体"));
    m_listInfo.SetFont(&font);
    font.Detach();
    m_listInfo.SetWindowBKColor(RGB(0,0,0));

}
