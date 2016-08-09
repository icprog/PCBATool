#pragma once
#include "afxwin.h"


// CCheckDlg dialog
#include "./Label/Label.h"
#include "afxcmn.h"
#include "./XListBox/XListBox.h"
#include "./FontStatic/FontStatic.h"
#include "../targetdevice/common/target.h"
#include "./pcba_status/ptest_status.h"
class CCheckDlg : public CDialog
{
	DECLARE_DYNAMIC(CCheckDlg)
    friend UINT TestDeviceThread(LPVOID lpParam);
public:
    CCheckDlg(CIniSettingBase &Configs,CIniLocalLan &LocalLang, CLogger *pLog = NULL,CWnd* pParent = NULL);
	virtual ~CCheckDlg();

// Dialog Data
	enum { IDD = IDD_CHECK_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
private:
    CWinThread          *m_pWorkThread;
    BOOL                ThisTestThreadProc();
    BOOL                OnStartTest(BOOL bAuto);
    BOOL                IsBusy();
    VOID                AddPrompt(CString strPrompt,int flag);
	//ldq
	VOID				ParseTestResult(std::string strResult);
    std::wstring        GetLocalString(std::wstring strKey);
    CIniSettingBase     &m_Configs;
    CIniLocalLan        &m_LocalLang;
    CLogger             *m_pLog;
    int                 m_nAt;
    int                 m_nGti;
    BOOL                m_Start;
    BOOL                m_bUserStop;
    
    VOID                CheckDlgReSize(int cx, int cy);

    /***/
    BOOL                m_bRedLedLight;
	HBITMAP             m_hGreenLedBitmap;
	HBITMAP             m_hRedLedBitmap;
    VOID                InitCtrl();
    VOID SetRunStatus(const TCHAR *szText,DWORD dwColor);
 
    BOOL                parse_status(STATUS_DATA *ptest_status,BOOL bPrint);
    BOOL                TimerElapseOK(DWORD ticks,DWORD LastTicks);
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL OnInitDialog();
    afx_msg LRESULT OnHandleAskBusyMsg(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnUserAction(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnUpdateCom(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnHandleUpdateStatusMsg(WPARAM wParam,LPARAM lParam);
    CLabel  m_LabelStatus;
    CLabel  m_LabelPort;
    CLabel  m_LabelDevices;
    CFontStatic  m_Status;
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    CXListBox m_listInfo;
    CStatic m_LabelSn;
    CStatic m_Sn;
};