#include "FuckSelfUpdater.h"

int main()
{
	DWORD processId, moduleAddress;
	HANDLE hProcess;
	byte jmp = 0xEB, nop = 0x90;
	byte HashBypassCode[] = { 0xE9, 0x52, 0x01, 0x00, 0x00 };

	printf("Searching for SelfUpdater.exe..\n");
	while (!(processId = FindProcessId("SelfUpdater.exe")))
	{
		Sleep(10);
	}
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if (!hProcess)
	{
		printf("process error..\n");
		return 0;
	}
	else
	{
		//Module Address
		moduleAddress = GetModuleBase("SelfUpdater.exe", processId);
		//UpdateInfo.CheckDownloaderHash
		WriteProcessMemory(hProcess, (LPVOID)(moduleAddress + 0x24786), &HashBypassCode, sizeof(HashBypassCode), NULL);
		WriteProcessMemory(hProcess, (LPVOID)(moduleAddress + 0x2478B), &nop, sizeof(nop), NULL);
		//UpdateInfo.CheckUpdaterHash
		WriteProcessMemory(hProcess, (LPVOID)(moduleAddress + 0x24AF6), &HashBypassCode, sizeof(HashBypassCode), NULL);
		WriteProcessMemory(hProcess, (LPVOID)(moduleAddress + 0x24AFB), &nop, sizeof(nop), NULL);
		//CheckUpdateInfo
		WriteProcessMemory(hProcess, (LPVOID)(moduleAddress + 0x24A33), &jmp, sizeof(jmp), NULL);
	}
	CloseHandle(hProcess);

	return 0;
}