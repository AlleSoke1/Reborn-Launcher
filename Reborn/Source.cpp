#include "stdafx.h"
// Set section alignment to be nice and small
#pragma comment(linker, "/FILEALIGN:0x200")

// Merge all default sections into the .text (code) section.
#pragma comment(linker,"/merge:.rdata=.text")
#pragma optimize("gsy", on)


//-----------------------------------------------------------------------------------------

using namespace std;

//Globals
CDialog * RebornForm;
CEdit * username;
CEdit * password;

BOOL REBORN_MainForm::OnInitDialog()
{

	//Load Icon :)
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);
	//Only run with min 6 arguments from dnlauncher redirect else attention the user
	LPWSTR *szArglist;
	int nArgs = 0;
	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	
	if (nArgs < 6)
	{
		MessageBoxA("Load game via DNLauncher.exe!", "Attention!", MB_ICONWARNING);
		ExitProcess(0);
	}

	//Check Upgrade ?
	FS_CheckUpgrade();



	//Read Account Data
	if (FileExists(STORE_ACCOUNT))
	{
		std::vector<std::string> Account = ReadAccountData();
		GetDlgItem(IDC_USERNAME)->SetWindowTextA(xorIT(Account[0]).c_str());
		GetDlgItem(IDC_PASSWORD)->SetWindowTextA(xorIT(Account[1]).c_str());	
		CButton * pBtn = (CButton *)GetDlgItem(IDC_CHECK1);
		ASSERT(pBtn);
		pBtn->SetCheck(1);
	}
	CDialog::OnInitDialog();
	CDialog::SetWindowTextA("REBORN Launcher");
	GetDlgItem(IDOK)->SetWindowTextA("PLAY");
	GetDlgItem(IDC_T_USERNAME)->SetWindowTextA("Username");
	GetDlgItem(IDC_T_PASSWORD)->SetWindowTextA("Password");
	
	//SECURITY CHECK   
	if (API_URL[7] != 'r')
	{
		ExitProcess(0);
	}

	if (IsDebuggerPresent())
    {
		MessageBox("A debugger has been found in your system.","ERROR",MB_ICONERROR);
		ExitProcess(0);
	}
	/**/

	return true;
}

//-----------------------------------------------------------------------------------------
//Need a Message Map Macro for both CDialog and CWinApp
BEGIN_MESSAGE_MAP(REBORN_MainForm, CDialog)
ON_BN_CLICKED(IDOK, &REBORN_MainForm::OnBnClickedOk)

ON_COMMAND(ID_OPTIONS_EMPTYREMEMEREDACCOUNT, &REBORN_MainForm::OnEmptyRememberdAccount)
ON_COMMAND(ID_OPTIONS_REGISTERACCOUNT, &REBORN_MainForm::OnRegister)
ON_COMMAND(ID_OPTIONS_EXIT, &REBORN_MainForm::OnExitMenu)
ON_COMMAND(40002, &REBORN_MainForm::OnAbout)
ON_COMMAND(ID_OPTIONS_RESETPASSWORD, &REBORN_MainForm::OnResetPassword)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------------------------
REBORN_Launcher Init;  //Starts the Application﻿



void REBORN_MainForm::OnBnClickedOk()
{
	

	//Get USERNAME + PASSWORD FIELD DATA
	CStringA uname, pword;
	GetDlgItem(IDC_USERNAME)->GetWindowTextA(uname);
	GetDlgItem(IDC_PASSWORD)->GetWindowTextA(pword);
//	std::wstring user(uname),pass(pword); //fuck wstring.!!!
	string uconv(uname),pconv(pword);
	char buff[128];
	wsprintf(buff,"%s&%s  XORED: %s %s",uconv.c_str(),capital(md5(pconv)).c_str(),xorIT(uconv).c_str(),xorIT(pconv).c_str());
	//MessageBoxA(buff, "OK", MB_OK); 
	
	//CHECK SIZE! Min 6 Max 18
	if (uconv.length() < 6 || uconv.length() > 18)
	{
		MessageBox("Username must between 6 and 18 characters!", "Attention!", MB_ICONEXCLAMATION);
		return;
	}
	if (pconv.length() < 6 | pconv.length() > 18)
	{
		MessageBox("Passsword must between 6 and 18 characters!", "Attention!", MB_ICONEXCLAMATION);
		return;
	}

	//Disable the OK button
	GetDlgItem(IDOK)->EnableWindow(false);
	//Get CHECKBOX status and save/remove account data.
	CButton *m_ctlCheck = (CButton*)GetDlgItem(IDC_CHECK1);
	int ChkBox = m_ctlCheck->GetCheck();
	if (ChkBox == BST_CHECKED)
	{
		if (FileExists(STORE_ACCOUNT) == true)
		{
			//MessageBox("STORE_ACCOUNT FOUND", "DEBUG", MB_OK);
			WriteAccountData(uconv, pconv, false);
		}
		else {
			WriteAccountData(uconv, pconv, true);
		}
	}
	else if (ChkBox == BST_UNCHECKED)
	{
		if (FileExists(STORE_ACCOUNT) == true)
		{
			FileDelete(STORE_ACCOUNT);
		}
	}
	//Validate the account with web :)
#if API_VER == 1
	std::string tempData = "login&"+uconv+"&"+capital(md5(pconv)).c_str();
#elif API_VER == 2
	std::string tempData = "login&" + uconv + "&" + capital(md5(pconv)).c_str() + "&" + std::to_string(API_VER); //add version2 api line
#endif
	std::string api = API_URL;
	std::string base64data = base64_encode((unsigned const char*)tempData.c_str(), tempData.length());
	std::string tempSend = api+"?r="+base64data;
	std::string temp;

	
	//send GET request
	temp = get(tempSend);

	//RESULT PARSE!
	if(temp.length() > 0)
	{
			std::vector<std::string> delimited;
			SplitStringDelim(temp,',', delimited); //split vector ^^
			if (delimited[0].compare("NOK") == 0)
			{
				MessageBoxA("Invalid Username or Password", "Error", MB_OK);
			}
			else if (delimited[0].compare("OK") == 0)
			{
				//MessageBoxA(delimited[0].c_str(), "RESULT", MB_OK);
				//MessageBoxA(delimited[1].c_str(), "IP", MB_OK);
				//MessageBoxA(delimited[2].c_str(), "PORT", MB_OK);
				//API V2
				//MessageBoxA(delimited[3].c_str(), "MD5SALTEDPASSWORD", MB_OK);

				char lpArguments[256];
				char lpApplicationName[128];
				//START PROCESS.
				if (FileExists(".//Connect.ini") == true)
				{
					char Custom_IP[256];
					char Custom_PORT[256];
					char Custom_EXECUTABLE[256];
					GetPrivateProfileString("SkyNest", "IP", "", Custom_PORT, 256, ".//Connect.ini");
					GetPrivateProfileString("SkyNest", "PORT", "", Custom_PORT, 256, ".//Connect.ini");
					GetPrivateProfileString("SkyNest", "Executable", "", Custom_EXECUTABLE, 256, ".//Connect.ini");	
					wsprintf(lpArguments, "/logintoken:%s&%s /ip:%s /port:%s /Lver:2 /use_packing /gamechanneling:0", uconv.c_str(), capital(md5(pconv)).c_str(), Custom_IP, Custom_PORT);
					wsprintf(lpApplicationName, "%s", Custom_EXECUTABLE);
				}else{
					wsprintf(lpArguments, "/logintoken:%s&%s /ip:%s /port:%s /Lver:2 /use_packing /gamechanneling:0", uconv.c_str(), capital(md5(pconv)).c_str(),delimited[1].c_str(), delimited[2].c_str());
					wsprintf(lpApplicationName, "Dragon.exe");
				}
				
				//MessageBox(lpArguments, "Arguments", MB_OK);
				
				SHELLEXECUTEINFO shExecInfo;

				shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);

				shExecInfo.fMask = NULL;
				shExecInfo.hwnd = NULL;
				shExecInfo.lpVerb = "runas";
				shExecInfo.lpFile = lpApplicationName;
				shExecInfo.lpParameters = lpArguments;
				shExecInfo.lpDirectory = NULL;
				shExecInfo.nShow = SW_NORMAL;
				shExecInfo.hInstApp = NULL;

				ShellExecuteEx(&shExecInfo);


				
				CDialog::OnOK();

			}
	}


	//enable the button
	GetDlgItem(IDOK)->EnableWindow(true);
	
	//ENABLE AFTER TESTS!
	//CDialog::OnOK();
}



void REBORN_MainForm::OnEmptyRememberdAccount()
{
	if (FileExists(STORE_ACCOUNT) == true)
	{
		//MessageBox("Data00.sky FOUND", "DEBUG", MB_OK);
		FileDelete(STORE_ACCOUNT);
	}

	GetDlgItem(IDC_USERNAME)->SetWindowTextA("");
	GetDlgItem(IDC_PASSWORD)->SetWindowTextA("");

	CButton * pBtn = (CButton *)GetDlgItem(IDC_CHECK1);
	ASSERT(pBtn);
	pBtn->SetCheck(0);
	MessageBoxA("All OK!", "OK", MB_OK);
}


void REBORN_MainForm::OnRegister()
{
	REBORN_RegisterForm Dlg;
	Dlg.DoModal();
}


void REBORN_MainForm::OnExitMenu()
{
	ExitProcess(0);
}


void REBORN_MainForm::OnAbout()
{
	MessageBoxA("Created by Alin1337", "About", MB_OK);
}


void REBORN_MainForm::OnResetPassword()
{
	MessageBoxA("SOON", "TESTING", MB_OK);
}
