#pragma once
#include "afxcmn.h"
#include "GeneralSettingsDlg.h"

// CPtConfig dialog

class CPtConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CPtConfigDlg)

public:
	CPtConfigDlg(CIniSettingBase &Configs,CIniLocalLan &LocalLang,CWnd* pParent = NULL);
	virtual ~CPtConfigDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONFIG };
private:
    #define CHILDWNDCNT    2 
    CDialog             *m_pChildWnd[CHILDWNDCNT];
    int                 m_iCurWnd;
	CGeneralSettingsDlg m_GneralSettings;
	CIniSettingBase     &m_Configs;
    CIniLocalLan        &m_LocalLang;
    void                ShowWnd(int iCurWnd) ;
    
    BOOL                SaveConfig();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTabSettings(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_TabSettings;
	afx_msg void OnBnClickedOk();
    afx_msg void OnCancel();
    afx_msg void OnClose();
};
