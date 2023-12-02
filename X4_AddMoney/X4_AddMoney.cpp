#include <iostream>
#include <Windows.h>
#include <WtsApi32.h>
#include <psapi.h>

#pragma comment(lib, "Wtsapi32.lib")

DWORD GetX4ProcessPID()
{
	std::wstring x4ProcessName = L"X4.exe";
	//wchar_t x4ProcessName[] = L"X4";

	DWORD info_level = 1;
	PWTS_PROCESS_INFO_EX proc_info = nullptr;
	DWORD pid_count = 0;

	if (!WTSEnumerateProcessesEx(WTS_CURRENT_SERVER_HANDLE, &info_level, WTS_ANY_SESSION, (PWSTR*)&proc_info, &pid_count))
	{
		std::cout << "Could not enumerate processes.\n" << "Error code: " << ::GetLastError << "\n";
		return 0;
	}

	DWORD result = 0;
	for (DWORD i = 0; i <= pid_count; i++)
	{
		auto& current_proc = proc_info[i];

		if (x4ProcessName == current_proc.pProcessName)
		{
			std::cout << "Found X4 process!\n";
			result = current_proc.ProcessId;
			break;
		}
	}

	WTSFreeMemoryEx(WTS_TYPE_CLASS::WTSTypeProcessInfoLevel1, proc_info, pid_count);
	return result;
}

int main()
{
	uint64_t remote_check;
	SIZE_T num_ofbytes_read;
	wchar_t x4_filepath[MAX_PATH] = {};
	uint64_t money_amount = 1; // Any value will max out player money.

	HANDLE x4_proc_address = OpenProcess(PROCESS_ALL_ACCESS, 0, GetX4ProcessPID());
	GetModuleFileNameEx(x4_proc_address, NULL, x4_filepath, MAX_PATH); // Get exe filepath

	HMODULE x4_module = LoadLibraryEx(x4_filepath, NULL, NULL);
	FARPROC money_add_proc = GetProcAddress(x4_module, "AddPlayerMoney");

	// Allocate and reserve a memory location in the remote process,
	// Write a parameter to memory location (money_amount)
	LPVOID money_remote_param = nullptr;
	try
	{
		money_remote_param = VirtualAllocEx(x4_proc_address, NULL, sizeof(money_amount), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		SIZE_T num_ofbytes_written = 0;
		if (!WriteProcessMemory(x4_proc_address, money_remote_param, &money_amount, sizeof(money_amount), &num_ofbytes_written))
		{
			std::cout << "[-] Trouble writing memory to process!! Error code: " << ::GetLastError() << "\n";
		}

		if (!ReadProcessMemory(x4_proc_address, money_remote_param, &remote_check, sizeof(money_amount), &num_ofbytes_read))
		{
			std::cout << "[-] Could not read memory at " << (char*)money_remote_param << " in remote process. Error code: " << ::GetLastError() << "\n";
		}

		if (money_amount == remote_check)
		{
			std::cout << "[+] Successfully wrote bytes\n";
		}

		DWORD thread_id;

		std::cout << "[.] Attempting to execute AddPlayerMoney\n";

		HANDLE dothething = CreateRemoteThreadEx(x4_proc_address, 0, 0, (LPTHREAD_START_ROUTINE)money_add_proc, money_remote_param, 0, NULL, &thread_id);
		std::cout << "[+] Success! Cleaning up." << std::endl;
		VirtualFreeEx(x4_proc_address, money_remote_param, 0, MEM_RELEASE);

	}
	catch (std::exception ex)
	{
		std::cout << "[-] Error\n" << ex.what();
	}
	
	return 0;
}

