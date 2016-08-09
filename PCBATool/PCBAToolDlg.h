
// PCBAToolDlg.h : header file
//

#pragma once
#include "CheckDlg.h"
#include "LoginDlg.h"
#include "ChangePasswordDlg.h"
// CPCBAToolDlg dialog
class CPCBAToolDlg : public CDialog
{
    friend UINT ScanDeviceThread(LPVOID lpParam);
// Construction
public:
	CPCBAToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PCBATOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
private:

    HANDLE              m_hDevScanEvent;
    HANDLE              m_hDevScanExitEvent;
    CWinThread          *m_hScanThread;
    void                ScanThreadProc();
    CCriticalSection    m_csPortChange;
    INTELCOMPORT        m_ports;

    CIniSettingBase m_Configs;
    CIniLocalLan    m_LocalLang;
    CLogger         *m_pLog;

    CString         m_strModulePath;
	CString         m_DlgTitle;

    BOOL            m_bStart;
    BOOL            m_bUserStop;
	BOOL            bAdmin;

    CDialog         *m_CheckDlg[MAX_DLG_CNT];
    BOOL            ShowCheckDlg(BOOL bInit);
    BOOL            DestoryCheckDlg();
    BOOL            IsBusy();
    BOOL            AppExit(BOOL bSaveConfig);
    BOOL            IsEnableConfig();
    BOOL            OnStartTest(BOOL bAuto);
    VOID            SpreadX(LPCRECT rct,int idx_start,int idx_cnt);
    VOID            ReSizeCheckDlg(LPCRECT rct);
    std::wstring    GetLocalString(std::wstring strKey);
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnClose();
    afx_msg void OnCancel();
    afx_msg void OnBnClickedButtonStart();
    afx_msg void OnUpdateSetting(CCmdUI *pCmdUI);
	afx_msg void OnUpdateUserChangepassword(CCmdUI *pCmdUI);
    afx_msg void OnSetting();
    afx_msg BOOL OnDeviceChange(UINT nEventType,DWORD_PTR dwData);


    afx_msg void OnHelpOpenlogfolder();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnHelpAbout();

	VOID CPCBAToolDlg::WalkMenu(CMenu *pMenu,CString strMainKeyPart);
	afx_msg void OnSettingLogin();
	afx_msg void OnUserLogout();
	afx_msg void OnUserChangepassword();
};
