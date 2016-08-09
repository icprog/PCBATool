// PtConfig.cpp : implementation file
//

#include "stdafx.h"
#include "../PCBATool.h"
#include "PtConfigDlg.h"


// CPtConfig dialog
IMPLEMENT_DYNAMIC(CPtConfigDlg, CDialog)
CPtConfigDlg::CPtConfigDlg(CIniSettingBase &Configs,CIniLocalLan &LocalLang,CWnd* pParent):CDialog(CPtConfigDlg::IDD, pParent), 
			m_GneralSettings(Configs,LocalLang,this),m_Configs(Configs),m_LocalLang(LocalLang)
{

}

CPtConfigDlg::~CPtConfigDlg()
{
}

void CPtConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_SETTINGS, m_TabSettings);
}


BEGIN_MESSAGE_MAP(CPtConfigDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SETTINGS, &CPtConfigDlg::OnTcnSelchangeTabSettings)
	ON_BN_CLICKED(IDOK, &CPtConfigDlg::OnBnClickedOk)
    ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPtConfig message handlers
BOOL CPtConfigDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_TabSettings.InsertItem(0,_T("Basic"));
    /*m_TabSettings.InsertItem(1,_T("DataBase")); **/

	CRect rect;
	m_TabSettings.GetClientRect(&rect);
    rect.InflateRect(-4,-22,-4,-4);

    m_GneralSettings.Create(IDD_DIALOG_GENERAL, GetDlgItem(IDC_TAB_SETTINGS));
    /*m_DBSettings.EnableWindow(FALSE); **/
    m_GneralSettings.MoveWindow(&rect);

    m_pChildWnd[0] = &m_GneralSettings;
    m_pChildWnd[1] = NULL;
    /*m_pChildWnd[1] = &m_DBSettings; **/

    m_iCurWnd = -1;
    m_TabSettings.SetCurSel(0);
    ShowWnd(0);

    m_LocalLang.TreeControls(m_hWnd,m_Configs.bDebug?TRUE:FALSE,this->IDD,false);

	return TRUE;
}
void CPtConfigDlg::ShowWnd(int iCurWnd) 
{
    if((0 > iCurWnd)||(CHILDWNDCNT <= iCurWnd)) return ;
    if(0 <= m_iCurWnd&&CHILDWNDCNT > m_iCurWnd) {
        if(m_pChildWnd[m_iCurWnd]) {
            m_pChildWnd[m_iCurWnd]->ShowWindow(SW_HIDE);
        }
        m_iCurWnd = iCurWnd;
        if(m_pChildWnd[m_iCurWnd]) {
            m_pChildWnd[m_iCurWnd]->ShowWindow(SW_SHOW);
        }
    } else {
        m_iCurWnd = iCurWnd;
        if(m_pChildWnd[m_iCurWnd]) {
            m_pChildWnd[m_iCurWnd]->ShowWindow(SW_SHOW);
        }
    }
}
void CPtConfigDlg::OnTcnSelchangeTabSettings(NMHDR *pNMHDR, LRESULT *pResult)
{

    int nCurSel = m_TabSettings.GetCurSel();
    ShowWnd(nCurSel);
	if(pResult) *pResult = 0;
}

void CPtConfigDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
    if(!SaveConfig()) {
        /*AfxMessageBox(TEXT("Save config fail")); **/
        return ;
    }
    for(int i = 0; i < CHILDWNDCNT; i ++ ) {
        if(m_pChildWnd[i]) {
            m_pChildWnd[i]->DestroyWindow();
            m_pChildWnd[i] = NULL;
        }
    }
    OnOK();
}

BOOL CPtConfigDlg::SaveConfig()
{
    if(!m_GneralSettings.SendMessage(WM_SAVE_CONFIG,0,0)) 
        return FALSE;
#if 0
    if(m_Configs.bUseDB&&m_Configs.bWideVine) {
        if(!m_DBSettings.SendMessage(WM_SAVE_CONFIG,0,0)) 
            return FALSE;
    }
#endif
    return TRUE;
}
void CPtConfigDlg::OnCancel()
{
    // TODO: Add your message handler code here and/or call default
    CDialog::OnCancel();
}

void CPtConfigDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    CDialog::OnClose();
}
