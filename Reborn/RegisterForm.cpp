#include "stdafx.h"

CEdit * user;
CEdit * pass;
CEdit * pass2;
CEdit * email;


BOOL REBORN_RegisterForm::OnInitDialog()
{
	CDialog::OnInitDialog();
	CDialog::SetWindowTextA("REBORN Register");
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);


	return true;
}

//-----------------------------------------------------------------------------------------
//Need a Message Map Macro for both CDialog and CWinApp
BEGIN_MESSAGE_MAP(REBORN_RegisterForm, CDialog)
ON_BN_CLICKED(IDOK, &REBORN_RegisterForm::OnBnClickedOk)
ON_EN_CHANGE(IDC_REG_PASSWORD, &REBORN_RegisterForm::OnEnChangeRedPassword)
END_MESSAGE_MAP()

void REBORN_RegisterForm::OnBnClickedOk()
{
	//Get USERNAME + PASSWORD FIELD DATA
	CStringA user, pass, pass2, email;
	GetDlgItem(IDC_REG_USERNAME)->GetWindowTextA(user);
	GetDlgItem(IDC_REG_PASSWORD)->GetWindowTextA(pass);
	GetDlgItem(IDC_REG_PASSWORD2)->GetWindowTextA(pass2);
	GetDlgItem(IDC_REG_EMAIL)->GetWindowTextA(email);
	//	std::wstring user(uname),pass(pword); //fuck wstring.!!!
	std::string uconv(user), pconv(pass), p2conv(pass2), econv(email);

	if (uconv.length() < 6 || uconv.length() > 18)
	{
		MessageBox("Username must be between 6 and 18 letters/numbers","WARNING",MB_ICONWARNING);
		//ui->Register_2->setEnabled(true);
		return;
	}

	if (pconv.length() < 6 || pconv.length() > 18 || p2conv.length() < 6 || p2conv.length() > 18)
	{
		MessageBox("Password must be between 6 and 18 characters","Warning",MB_ICONWARNING);
		//ui->Register_2->setEnabled(true);
		return;
	}

	if (econv.length() < 5)
	{
		MessageBox("Email must be longer than 5 characters.");
		//ui->Register_2->setEnabled(true);
		return;
	}

	char buff[120];
	wsprintf(buff, "USER: %s , PASS %s , Pass2 %s , email %s", uconv.c_str(), pconv.c_str(), p2conv.c_str() ,econv.c_str());
	//MessageBox(buff, "CREATE", MB_OK);

	std::string api = API_URL;
	std::string beforeEnc = "register&" + uconv + "&" + pconv + "&" + p2conv + "&" + econv;
	std::string data = base64_encode((unsigned char *)beforeEnc.c_str(),beforeEnc.length());
	std::string tempData = api+"?r="+data;
	//std::string tempSend = API_URL + base64_encode((unsigned const char*)tempData.c_str(), tempData.length());
	
	std::string temp;
	//wsprintf(temp,"%s")

	temp = get(tempData);

	if (temp.length() > 0)
	{
		std::vector<std::string> delimited;
		SplitStringDelim(temp, ',', delimited); //split vector ^^
		if (delimited[0].compare("NOK") == 0)
		{
			if (delimited[1].compare("99") == 0)
			{
				MessageBoxA(delimited[2].c_str(), "Response", MB_OK);
			}
		}
		else if (delimited[0].compare("OK") == 0)
		{
			MessageBoxA(delimited[0].c_str(), "RESULT", MB_OK);
		//  MessageBoxA(delimited[1].c_str(), "RESULT2", MB_OK);
		//	MessageBoxA(delimited[2].c_str(), "RESULT3", MB_OK);
		}
	}

	//MessageBoxA(temp.c_str(), "Hello", MB_OK);
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void REBORN_RegisterForm::OnEnChangeRedPassword()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
