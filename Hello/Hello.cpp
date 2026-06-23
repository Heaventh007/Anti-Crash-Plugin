// Hello.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

BOOL UnloadPlugin;
HANDLE ThreadHandle;
DWORD ThreadID;

VOID HandleBO2(); 
VOID HandleBO1();
VOID HandleMW2();
VOID HandleMW3(); 
VOID CleanupHooks();

DWORD HandleTitleChange() {
	DWORD LastTitleID = 0;

	while (!UnloadPlugin) {
		DWORD CurrentTitleID = XamGetCurrentTitleId();

		if (CurrentTitleID != LastTitleID) {
			if (CurrentTitleID)
				DbgPrint("Checking Title: %X\n", CurrentTitleID);

			if (CurrentTitleID == 0x415608C3) HandleBO2();
			if (CurrentTitleID == 0x41560855) HandleBO1();
			if (CurrentTitleID == 0x415608CB) HandleMW3();
			if (CurrentTitleID == 0x41560817) HandleMW2();

			LastTitleID = CurrentTitleID;
		}

		Sleep(140);
	}

	return 0;
}
 

VOID OnAttachProcess() {
	LOGPRINT_NL("[Anti Crash Plugin] Load Started.\n");

	ExCreateThread(&ThreadHandle, NULL, &ThreadID, XapiThreadStartup, LPTHREAD_START_ROUTINE(HandleTitleChange), NULL, CREATE_SUSPENDED | EX_CREATE_FLAG_SYSTEM);
	XSetThreadProcessor(ThreadHandle, 4);
	SetThreadPriority(ThreadHandle, THREAD_PRIORITY_NORMAL);
	ResumeThread(ThreadHandle);

	LOGPRINT_NL("[Anti Crash Plugin] Sucessfully Loaded.\n");
}

VOID OnDetachProcess() {
	LOGPRINT_NL("[Anti Crash Plugin] Unloading.\n");
	UnloadPlugin = TRUE;

	CleanupHooks();

	LOGPRINT_NL("[Anti Crash Plugin] Unloaded.\n");
	Sleep(1200);
}

BOOL WINAPI DllMain(HANDLE hInst, DWORD dwReason, LPVOID lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) OnAttachProcess();
	if (dwReason == DLL_PROCESS_DETACH) OnDetachProcess();

	return TRUE;
}