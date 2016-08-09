#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "../label/label.h"


// CGeneralSettings dialog

class CGeneralSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CGeneralSettingsDlg)

public:
	CGeneralSettingsDlg(CIniSettingBase &Configs,CIniLocalLan &LocalLang,CWnd* pParent = NULL);
	virtual ~CGeneralSettingsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_GENERAL };
private:
	CIniSettingBase &m_Configs;
    CIniLocalLan    &m_LocalLang;
    BOOL                isVaildFile(CString strXmlFileName);
    CString             BrowseAndGetSingleFile(CString strInitialPath,CString strFilter);
    void                UpdateUIStatus(DWORD dwMask);
    BOOL                SaveConfig();
    void                InitCtrl();
    void                InitCmbDBType(const CString& str);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
    /*CLabel lbKeyBoxBin; **/
    /*CLabel lbFuseScript;**/
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg LRESULT OnSaveConfig(WPARAM wParam,LPARAM lParam);
    CComboBox m_cbDeviceType;
};
