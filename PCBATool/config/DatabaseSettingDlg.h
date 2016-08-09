#pragma once

#include "../settings/SettingBase.h"
#include "afxcmn.h"
// CDatabaseSettingDlg dialog
extern TCHAR *TableFormat;
class CDatabaseSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CDatabaseSettingDlg)

public:
	CDatabaseSettingDlg(CIniSettingBase &Configs,CIniLocalLan &LocalLang,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDatabaseSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DATABASE };
private:
    CIniSettingBase &m_Configs;
    CIniLocalLan    &m_LocalLang;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
    BOOL    SaveConfig();
    /*BOOL    OnUpdateTable(vector<CADOFieldInfo> * pvInfo = NULL,BOOL bInitial = TRUE); **/
    BOOL    UpdateCtrlStatus();

    afx_msg void OnBnClickedBtOk();
    afx_msg void OnBnClickedButtonCreate();
    CListCtrl m_ListTable;
    afx_msg void OnClose();
    afx_msg void OnBnClickedBtCancel();
    afx_msg void OnLvnItemchangedListInfo(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMRClickListInfo(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnPopmenuDelete();
    afx_msg void OnPopmenuAdd();
    afx_msg void OnPopmenuInsert();
    afx_msg LRESULT OnSaveConfig(WPARAM wParam,LPARAM lParam);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
