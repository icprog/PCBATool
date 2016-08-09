// GeneralSettings.cpp : implementation file
//

#include "stdafx.h"
#include "../PCBATool.h"
#include "GeneralSettingsDlg.h"


// CGeneralSettings dialog

IMPLEMENT_DYNAMIC(CGeneralSettingsDlg, CDialog)

CGeneralSettingsDlg::CGeneralSettingsDlg( CIniSettingBase &Configs,CIniLocalLan &LocalLang,CWnd* pParent /*=NULL*/)
	: CDialog(CGeneralSettingsDlg::IDD, pParent), m_Configs(Configs),m_LocalLang(LocalLang)
{

}

CGeneralSettingsDlg::~CGeneralSettingsDlg()
{
}

void CGeneralSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_DEVICETY, m_cbDeviceType);
}


BEGIN_MESSAGE_MAP(CGeneralSettingsDlg, CDialog)
    ON_MESSAGE(WM_SAVE_CONFIG,&CGeneralSettingsDlg::OnSaveConfig)
END_MESSAGE_MAP()


// CGeneralSettings message handlers
BOOL CGeneralSettingsDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    InitCmbDBType(m_Configs.strDeviceType.c_str());
    ((CButton*)GetDlgItem(IDC_CHECK_AUTO    ))->SetCheck(m_Configs.bAutoTest                ?BST_CHECKED:BST_UNCHECKED);
    ((CButton*)GetDlgItem(IDC_CHECK_ONLYAT  ))->SetCheck(m_Configs.bOnlyAt                  ?BST_CHECKED:BST_UNCHECKED);
    ((CButton*)GetDlgItem(IDC_CHECK_DATBASE ))->SetCheck(m_Configs.bDataBase                ?BST_CHECKED:BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_FAIL_EXIT))->SetCheck(m_Configs.bEndIfFail              ?BST_CHECKED:BST_UNCHECKED);
    ((CButton*)GetDlgItem(IDC_CHECK_REBOOT))->SetCheck(m_Configs.bReboot                 ?BST_CHECKED:BST_UNCHECKED);
    
    ((CButton*)GetDlgItem(IDC_CHECK_SIM     ))->SetCheck(m_Configs.bTestOption[TEST_SIM]    ?BST_CHECKED:BST_UNCHECKED);    
    ((CButton*)GetDlgItem(IDC_CHECK_WIFITEST))->SetCheck(m_Configs.bTestOption[TEST_WIFI]   ?BST_CHECKED:BST_UNCHECKED);    
    ((CButton*)GetDlgItem(IDC_CHECK_BT      ))->SetCheck(m_Configs.bTestOption[TEST_BT]     ?BST_CHECKED:BST_UNCHECKED);    
    ((CButton*)GetDlgItem(IDC_CHECK_SENSOR  ))->SetCheck(m_Configs.bTestOption[TEST_SENSOR] ?BST_CHECKED:BST_UNCHECKED);    
    ((CButton*)GetDlgItem(IDC_CHECK_GPS     ))->SetCheck(m_Configs.bTestOption[TEST_GPS]    ?BST_CHECKED:BST_UNCHECKED);    
    ((CButton*)GetDlgItem(IDC_CHECK_CAMERA  ))->SetCheck(m_Configs.bTestOption[TEST_CAMERA] ?BST_CHECKED:BST_UNCHECKED);    
    ((CButton*)GetDlgItem(IDC_CHECK_CODEC   ))->SetCheck(m_Configs.bTestOption[TEST_CODEC]  ?BST_CHECKED:BST_UNCHECKED);    
    
    //m_cbDeviceType.InsertString(0,_T("Sofia3GR"));
    //m_cbDeviceType.InsertString(1,_T("XMM6321"));
    //m_cbDeviceType.InsertString(2,_T("WifiOnly"));
    m_cbDeviceType.SetCurSel(m_Configs.nDeviceType);
#if 0
    ((CButton*)GetDlgItem(IDC_CHECK_FUSE  ))->SetCheck(m_Configs.bFuse    ?BST_CHECKED:BST_UNCHECKED);
    ((CButton*)GetDlgItem(IDC_CHECK_WV    ))->SetCheck(m_Configs.bWideVine?BST_CHECKED:BST_UNCHECKED);
    ((CButton*)GetDlgItem(IDC_CHECK_USEDB ))->SetCheck(m_Configs.bUseDB   ?BST_CHECKED:BST_UNCHECKED);
    if( m_Configs.bIsBin) {
        ((CButton*)GetDlgItem(IDC_CHECK_BIN))->SetCheck(BST_CHECKED);
        ((CButton*)GetDlgItem(IDC_CHECK_XML))->SetCheck(BST_UNCHECKED);
    } else {
        ((CButton*)GetDlgItem(IDC_CHECK_BIN))->SetCheck(BST_UNCHECKED);
        ((CButton*)GetDlgItem(IDC_CHECK_XML))->SetCheck(BST_CHECKED);
    }
    InitCtrl();
#endif
    return TRUE;
}

LRESULT CGeneralSettingsDlg::OnSaveConfig(WPARAM wParam,LPARAM lParam)
{
    return SaveConfig();
}

void CGeneralSettingsDlg::InitCtrl()
{
#if 0
    lbKeyBoxBin.SetTextColor(RGB(0,0,255));
    lbKeyBoxBin.SetFontSize(10);
    lbKeyBoxBin.SetFontBold(TRUE);

    lbFuseScript.SetTextColor(RGB(0,0,255));
    lbFuseScript.SetFontSize(10);
    lbFuseScript.SetFontBold(TRUE);

    SetDlgItemText(IDC_EDIT_SCRIPT    , m_Configs.strFuseScriptFileName.c_str());
    SetDlgItemText(IDC_EDIT_KEYBOXBIN , m_Configs.strXmlFileName.c_str());
    UpdateUIStatus(MASK_FUSE|MASK_WV|MASK_WV);
#endif
    
}
void CGeneralSettingsDlg::InitCmbDBType(const CString& str )
{
    int npos = 0,npos_before =0;
    int nCount = 0;
    CString strCmbDBType = _T("");
    CString strValue = _T("");
    strCmbDBType = str;

    if (str.IsEmpty())
    {
        return;
    }
    while ( npos>= 0 )
    {
        npos = strCmbDBType.Find(',',npos_before);
        if (npos>0)
        {
            strValue = strCmbDBType.Mid(npos_before,npos-npos_before);
            m_cbDeviceType.InsertString(nCount,strValue);
            npos_before = npos+1;
            nCount++;
        }
    }
    strValue = strCmbDBType.Right(strCmbDBType.GetLength()-npos_before);
    m_cbDeviceType.InsertString(nCount,strValue);

}
void CGeneralSettingsDlg::UpdateUIStatus(DWORD dwMask)
{
    /*basic **/
    if(dwMask&MASK_BASIC) {
    

    }
#if 0
    /*widevine **/
    if(dwMask&MASK_WV){
        if(m_Configs.bWideVine) {
            GetDlgItem(IDC_CHECK_XML   )->EnableWindow(TRUE);
            GetDlgItem(IDC_CHECK_BIN   )->EnableWindow(TRUE);
            GetDlgItem(IDC_CHECK_USEDB   )->EnableWindow(TRUE);
            if(m_Configs.bUseDB) {
                GetDlgItem(IDC_EDIT_KEYBOXBIN  )->EnableWindow(FALSE);
                GetDlgItem(IDC_BUTTON_KEYBOXBIN)->EnableWindow(FALSE);
                GetDlgItem(IDC_STATIC_XML)->EnableWindow(FALSE);
                /*lbKeyBoxBin.EnableWindow(FALSE); **/
            } else {
                GetDlgItem(IDC_EDIT_KEYBOXBIN  )->EnableWindow(TRUE);
                GetDlgItem(IDC_BUTTON_KEYBOXBIN)->EnableWindow(TRUE);
                GetDlgItem(IDC_STATIC_XML)->EnableWindow(TRUE);
                /*lbKeyBoxBin.EnableWindow(TRUE); **/
            }
        } else {
            GetDlgItem(IDC_CHECK_XML   )->EnableWindow(FALSE);
            GetDlgItem(IDC_CHECK_BIN   )->EnableWindow(FALSE);
            GetDlgItem(IDC_EDIT_KEYBOXBIN  )->EnableWindow(FALSE);
            GetDlgItem(IDC_BUTTON_KEYBOXBIN)->EnableWindow(FALSE);
            GetDlgItem(IDC_STATIC_XML)->EnableWindow(FALSE);
            /*lbKeyBoxBin.EnableWindow(FALSE); **/
            GetDlgItem(IDC_CHECK_USEDB   )->EnableWindow(FALSE);
        }
    }
    /*fuse **/
    if(dwMask&MASK_FUSE){
        BOOL enable;
        if(m_Configs.bFuse) {
            enable = TRUE;
        } else {
            enable = FALSE;
        }
        GetDlgItem(IDC_EDIT_SCRIPT  )->EnableWindow(enable);
        GetDlgItem(IDC_STATIC_SCRIPT)->EnableWindow(enable);
        GetDlgItem(IDC_BUTTON_SCRIPT)->EnableWindow(enable);
    }
#endif
}

BOOL CGeneralSettingsDlg::isVaildFile(CString strXmlFileName)
{
    TCHAR *szType[] = {
        TEXT(".xml"),
        TEXT(".XML"),
        TEXT(".dat"),
        TEXT(".DAT")
    };
    int nDex = m_Configs.bIsBin?1:0;
    int extpos = strXmlFileName.Find(szType[nDex*2 + 0]);
    if(-1 == extpos ) {
        extpos = strXmlFileName.Find(szType[nDex*2 + 1]);
    }
    if( -1 == extpos ) {
        return FALSE;
    }
    return TRUE;
}

CString CGeneralSettingsDlg::BrowseAndGetSingleFile(CString strInitialPath,CString strFilter)
{
    CString         strFileName(TEXT(""));
    CFileDialog     filedlg(TRUE,NULL,NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFilter);
    TCHAR           pBuffer[MAX_PATH];
    memset(pBuffer,0,sizeof(TCHAR)*(MAX_PATH));
    swprintf(pBuffer,nof(pBuffer),(LPCTSTR)strInitialPath);
    filedlg.m_ofn.lpstrFile = pBuffer;
    filedlg.m_ofn.nMaxFile  = MAX_PATH;
    if( IDOK == filedlg.DoModal() ) {
        POSITION pos = filedlg.GetStartPosition();  
		while (pos != NULL)   {  
			strFileName =  filedlg.GetNextPathName(pos);
		}  
    } 
    return strFileName;
}
BOOL CGeneralSettingsDlg::SaveConfig()
{
    if(m_Configs.bWideVine||m_Configs.bFuse) {
    } else {
        AfxMessageBox(TEXT("Must Select a operation"));
        return FALSE;
    }
    m_Configs.bAutoTest                 = (((CButton*)GetDlgItem(IDC_CHECK_AUTO		))->GetCheck() == BST_CHECKED);
    m_Configs.bOnlyAt                   = (((CButton*)GetDlgItem(IDC_CHECK_ONLYAT   ))->GetCheck() == BST_CHECKED);
    m_Configs.bDataBase                 = (((CButton*)GetDlgItem(IDC_CHECK_DATBASE  ))->GetCheck() == BST_CHECKED);
	m_Configs.bEndIfFail				= (((CButton*)GetDlgItem(IDC_CHECK_FAIL_EXIT))->GetCheck() == BST_CHECKED);
    m_Configs.bReboot				    = (((CButton*)GetDlgItem(IDC_CHECK_REBOOT   ))->GetCheck() == BST_CHECKED);
    m_Configs.bTestOption[TEST_SIM]     = (((CButton*)GetDlgItem(IDC_CHECK_SIM      ))->GetCheck() == BST_CHECKED);
    m_Configs.bTestOption[TEST_WIFI]    = (((CButton*)GetDlgItem(IDC_CHECK_WIFITEST ))->GetCheck() == BST_CHECKED);
    m_Configs.bTestOption[TEST_BT]      = (((CButton*)GetDlgItem(IDC_CHECK_BT       ))->GetCheck() == BST_CHECKED);
    m_Configs.bTestOption[TEST_SENSOR]  = (((CButton*)GetDlgItem(IDC_CHECK_SENSOR   ))->GetCheck() == BST_CHECKED);
    m_Configs.bTestOption[TEST_GPS]     = (((CButton*)GetDlgItem(IDC_CHECK_GPS      ))->GetCheck() == BST_CHECKED);
    m_Configs.bTestOption[TEST_CAMERA]  = (((CButton*)GetDlgItem(IDC_CHECK_CAMERA   ))->GetCheck() == BST_CHECKED);
    m_Configs.bTestOption[TEST_CODEC]   = (((CButton*)GetDlgItem(IDC_CHECK_CODEC    ))->GetCheck() == BST_CHECKED);
    m_Configs.nDeviceType               = m_cbDeviceType.GetCurSel();

    return TRUE;
}
BOOL CGeneralSettingsDlg::PreTranslateMessage(MSG* pMsg)
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
