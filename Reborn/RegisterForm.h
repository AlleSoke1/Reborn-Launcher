
#ifndef REGFORM_H
#define REGFORM_H

class REBORN_RegisterForm : public CDialog
{
public:
	REBORN_RegisterForm(CWnd* pParent = NULL) : CDialog(REBORN_RegisterForm::IDD, pParent)
	{    }
	// Dialog Data, name of dialog form
	enum { IDD = IDD_DIALOG2 };
protected:
	virtual void DoDataExchange(CDataExchange* pDX) { CDialog::DoDataExchange(pDX); }
	//Called right after constructor. Initialize things here.
	virtual BOOL OnInitDialog();
public:
	DECLARE_MESSAGE_MAP()
afx_msg void OnBnClickedOk();
afx_msg void OnEnChangeRedPassword();
};

#pragma once
#endif
