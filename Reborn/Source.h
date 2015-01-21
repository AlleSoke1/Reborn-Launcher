
#ifndef SOURCE_H
#define SOURCE_H



class REBORN_MainForm : public CDialog
{
public:
	REBORN_MainForm(CWnd* pParent = NULL) : CDialog(REBORN_MainForm::IDD, pParent)
	{    }
	// Dialog Data, name of dialog form
	enum { IDD = IDD_DIALOG1 };
protected:
	virtual void DoDataExchange(CDataExchange* pDX) { CDialog::DoDataExchange(pDX); }
	//Called right after constructor. Initialize things here.
	virtual BOOL OnInitDialog();
public:
	DECLARE_MESSAGE_MAP()
		afx_msg void OnBnClickedOk();
		afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);

afx_msg void OnEmptyRememberdAccount();
afx_msg void OnRegister();
afx_msg void OnExitMenu();
afx_msg void OnAbout();
afx_msg void OnResetPassword();
};

class REBORN_Launcher : public CWinApp
{
public:
	REBORN_Launcher() {  }
public:
	virtual BOOL InitInstance()
	{
		CWinApp::InitInstance();
		REBORN_MainForm dlg;
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		return FALSE;
	} //close function
}; 
#endif