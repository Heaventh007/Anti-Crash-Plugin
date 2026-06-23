#include "stdafx.h"

wchar_t *Buffer = NULL;
DWORD CurrentGame = 0; 

// In case you were wondering, why I didn't use the title ID...
// Good question, I'd ask Heaventh from like 2 years ago. I really do not know.
enum LoadedGame {
	BO2 = 0x696969,
	BO1 = 0x696966,
	MW2 = 0x696996,
	MW3 = 0x696999,
};

void PrintNotify() {
	XNotifyQueueUI(XNOTIFYQUEUEUI_TYPE::XNOTIFYUI_TYPE_PREFERRED_REVIEW, 0, 0, L"Anti Crash Plugin - Loaded!\nCreated by: Heaventh!", 0);
}

char* Cmd_Argv(DWORD dwArg) {
	DWORD ArgvAddr = 0;

	if (CurrentGame == BO2) ArgvAddr = 0x821BBBD8;
	if (CurrentGame == BO1) ArgvAddr = 0x82164040;
	if (CurrentGame == MW2) ArgvAddr = 0x8211E910;
	if (CurrentGame == MW3) ArgvAddr = 0x82143050;

	if (ArgvAddr)
		return CALL_FUNC(char*, ArgvAddr, dwArg);

	return "";
}

bool IsGameUpdated(char* UpdateString) {
	if (stricmp(UpdateString, "nonce")) {
		LOGPRINT_NL("Game is not up to date, let's not try and patch it.");
		return FALSE;
	}

	return TRUE;
}

template<typename T>
bool IsFromHost(T From) {
	DWORD IsHostAddr = 0;
	DWORD PartyDataAddr = 0;

	// Looks like shit, who cares.
	if (CurrentGame == BO1) {
		IsHostAddr = 0x822407F8;
		PartyDataAddr = 0x829F7038;
	} if (CurrentGame == BO2) {
		IsHostAddr = 0x82271010;
		PartyDataAddr = 0x82CAC160;
	} if (CurrentGame == MW2) {
		PartyDataAddr = 0x8268EE78;
		IsHostAddr = 0x82142AA8;
	} if (CurrentGame == MW3) {
		PartyDataAddr = 0x82720D90;
		IsHostAddr = 0x82170778;
	}

	if (!IsHostAddr || !PartyDataAddr)
		return TRUE;

	return CALL_FUNC(bool, IsHostAddr, PartyDataAddr, From);
}

template<typename T>
bool HandleConnectionlessPacket(T From, msg_t* pMsg) {
	char* szCommand = Cmd_Argv(0);

	if (!stricmp(szCommand, "relay")) {
		int iResultPlr = atoi(Cmd_Argv(1));

		if (iResultPlr > 17 || iResultPlr < 0) {
			LOGPRINT_NL("[Anti Crash Plugin] Prevented a crash exploit!");
			return FALSE
		}
	}

	if (!stricmp(szCommand, "mstart")) {
		if (!IsFromHost<T>(From)) { // Credit to BlastsMods for the host check idea.
			LOGPRINT_NL("[Anti Crash Plugin] Prevented an invalid migration screen!");
			return FALSE
		}
	}

	return TRUE;
}

Detour<BOOL> CL_DispatchConnectionlessPacket_Detour;
BOOL CL_DispatchConnectionlessPacketBO2_Hook(DWORD dwLocalClientNum, netadrBO2_t From, msg_t* pMsg, DWORD Time) {
	if (HandleConnectionlessPacket<netadrBO2_t>(From, pMsg))
		return CL_DispatchConnectionlessPacket_Detour.CallOriginal(dwLocalClientNum, From, pMsg, Time);

	return true;
}

BOOL CL_DispatchConnectionlessPacketBO1_Hook(DWORD dwLocalClientNum, netadrBO1_t From, msg_t* pMsg, DWORD Time) {
	if (HandleConnectionlessPacket<netadrBO1_t>(From, pMsg))
		return CL_DispatchConnectionlessPacket_Detour.CallOriginal(dwLocalClientNum, From, pMsg, Time);

	return true;
}

BOOL CL_DispatchConnectionlessPacketMW_Hook(DWORD dwLocalClientNum, netadrMW_t From, msg_t* pMsg, DWORD Time) {
	if (HandleConnectionlessPacket<netadrMW_t>(From, pMsg))
		return CL_DispatchConnectionlessPacket_Detour.CallOriginal(dwLocalClientNum, From, pMsg, Time);

	return true;
}

Detour<BOOL> Netchan_Process_Detour;
BOOL Netchan_Process_Hook(void* pChan, msg_t* pMsg) {
	NetchanIncomingData_s* IncomingData = (NetchanIncomingData_s*)((DWORD)pMsg->Data);

	if (IncomingData->OutgoingSequence & 0xC0000000 || IncomingData->OutgoingSequence & 0x80000000) {
		if (IncomingData->FragmentIndex > 0x7F ||
			IncomingData->FragmentOffset > SHRT_MAX || IncomingData->FragmentOffset < 0x0) {
			return 0;
		}

		if (IncomingData->FragmentSize < 0) {
			return 0;
		}
	}

	return Netchan_Process_Detour.CallOriginal(pChan, pMsg);
}

Detour<void> MSG_WriteString_Detour;
void MSG_WriteString_Hook(void* pMsg, const char* szString) {

	if (!szString) {
		return;
	}

	return MSG_WriteString_Detour.CallOriginal(pMsg, szString);
}

VOID HandleMW3() {
	Sleep(7000); // Wait a decent amount of time before we patch.
	if (IsGameUpdated((char*)0x8201678C)) {
		CurrentGame = MW3;

		LOGPRINT_NL("[Anti Crash Plugin] MW3 detected, attempting to patch.");

		CL_DispatchConnectionlessPacket_Detour.SetupDetour(0x82176A98, CL_DispatchConnectionlessPacketMW_Hook);

		*(DWORD*)0x821773F4 = 0x60000000;
		*(DWORD*)0x82362678 = 0x38A00004;

		PrintNotify();
	}
}

VOID HandleMW2() {
	Sleep(7000); // Wait a decent amount of time before we patch.
	if (IsGameUpdated((char*)0x820153C0)) {
		CurrentGame = MW2;

		LOGPRINT_NL("[Anti Crash Plugin] MW2 detected, attempting to patch.");

		CL_DispatchConnectionlessPacket_Detour.SetupDetour(0x82147398, CL_DispatchConnectionlessPacketMW_Hook);

		*(DWORD*)0x82147BA0 = 0x60000000;

		PrintNotify();
	}
} 

VOID HandleBO1() {
	Sleep(7000); // Wait a decent amount of time before we patch.
	if (IsGameUpdated((char*)0x820206E0)) {
		CurrentGame = BO1;

		LOGPRINT_NL("[Anti Crash Plugin] BO1 detected, attempting to patch.");

		Netchan_Process_Detour.SetupDetour(0x8234FE08, Netchan_Process_Hook);
		MSG_WriteString_Detour.SetupDetour(0x8234A380, MSG_WriteString_Hook);
		CL_DispatchConnectionlessPacket_Detour.SetupDetour(0x82246120, CL_DispatchConnectionlessPacketBO1_Hook);

		// Anti kick (Crash in tommy's mind...)
		*(DWORD*)0x8215C598 = 0x60000000;
		*(DWORD*)0x8215C530 = 0x60000000;
		*(DWORD*)0x8215C4C8 = 0x60000000;
		*(DWORD*)0x8215C460 = 0x60000000;
		*(DWORD*)0x8215C3F8 = 0x60000000;

		// Anti migration kick.
		*(DWORD*)0x822468CC = 0x60000000;

		PrintNotify();
	}
}

VOID HandleBO2() {
	Sleep(7000); // Wait a decent amount of time before we patch.
	if (IsGameUpdated((char*)0x82015E04)) {
		CurrentGame = BO2;

		LOGPRINT_NL("[Anti Crash Plugin] BO2 detected, attempting to patch.");

		DWORD* ServerCommandJumpTable = (DWORD*)0x8222A430;

		ServerCommandJumpTable[0x2B] = 0x8222B000;
		ServerCommandJumpTable[0x37] = 0x8222B000;
		ServerCommandJumpTable[0x6A] = 0x8222B000;

		*(DWORD*)0x822A06B0 = 0x60000000;
		*(DWORD*)0x822A067C = 0x60000000;
		*(DWORD*)0x822781D8 = 0x60000000;
		*(DWORD*)0x822781D8 = 0x60000000;

		*(DWORD*)0x82278020 = 0x60000000;
		*(DWORD*)0x823D16F8 = 0x4E800020;

		Netchan_Process_Detour.SetupDetour(0x8241E438, Netchan_Process_Hook);
		CL_DispatchConnectionlessPacket_Detour.SetupDetour(0x82277888, CL_DispatchConnectionlessPacketBO2_Hook);

		PrintNotify();
	}
}

VOID CleanupHooks() {
	if (CL_DispatchConnectionlessPacket_Detour.IsDetoured())
		CL_DispatchConnectionlessPacket_Detour.TakeDownDetour();

	if (Netchan_Process_Detour.IsDetoured())
		Netchan_Process_Detour.TakeDownDetour();

	if (MSG_WriteString_Detour.IsDetoured())
		MSG_WriteString_Detour.TakeDownDetour();
}