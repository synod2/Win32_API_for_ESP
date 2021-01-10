#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <tchar.h>
//���μ��� �̸��� ���ڷ� �Ͽ� pid�� ��ȯ�ϴ� �Լ� 
bool process_name_to_pid(
	__out DWORD& pid,
	__in const std::wstring& process_name
);
//�ش� PID�� ���� ���μ����� ������ �̸��� ���� dll�� ��������. 
bool dll_injection(
	__in DWORD pid,
	__in const std::wstring& dll_name
);
int main()
{
	DWORD pid = 0;
	std::wstring process_name = L"winmine_(�ѱ�).exe";
	std::wstring dll_name = L"G:\\dll_injection\\testDll2.dll";
	//���μ��� �̸��� ���� pid�� ���ؿ���, �ش� pid�� ������ �̸��� ���� dll�� ������ �ϴ� ����. 
	if (process_name_to_pid(pid, process_name)) {
		dll_injection(pid, dll_name);
	}
	return 0;
}
bool process_name_to_pid(
	__out DWORD& pid,
	__in const std::wstring& process_name
) {
	bool result = false; //����� �⺻������ false, ���μ����� ������ ��� true�� ����.  
	HANDLE snapshot = nullptr;
	PROCESSENTRY32 entry = {};
	entry.dwSize = sizeof(PROCESSENTRY32);

	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	//������ ���μ����� ��, ��⿡���� ������ ���� �������� �������� �Լ�. 

	if (snapshot != INVALID_HANDLE_VALUE) {
		//do-while �� �ȿ��� ���μ��� ����� Ž���ϸ� ����ڰ� ������ ���μ��� �̸��� �ִ��� Ž���Ѵ�.
		Process32First(snapshot, &entry);
		// ���������� �߻��� ù���� ���μ��� ������ �˻� 
		do {
			if (!_tcsicmp(process_name.c_str(), entry.szExeFile)) {
				//������ ���μ��� �̸��� entry�� ���μ��� �̸��� ��ġ(_tcsicmp�Լ�-��ҹ��� ����X) �ϴ��� ��. 
				pid = entry.th32ProcessID;
				result = true;
				break;
				//��ġ�ϴ� ��쿡 result ���� �� pid ��ȯ. 
			}
		} while (Process32Next(snapshot, &entry)); //�������� ��ϵ� ���� ���μ����� ������ Ž�� 
		CloseHandle(snapshot);
	}
	return result;
}
bool dll_injection(
	__in DWORD pid,
	__in const std::wstring& dll_name
) {
	bool result = false;
	HANDLE process_handle = nullptr;
	HANDLE thread_handle = nullptr;
	LPVOID remote_buffer = nullptr;
	HMODULE module = {};
	LPTHREAD_START_ROUTINE thread_start_routine = nullptr;
	do {
		process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		//�ش� PID�� ���� ���μ����� ���� �ڵ��� �����´�
		if (process_handle == nullptr) {
			break;
		}
		remote_buffer = VirtualAllocEx(
			process_handle,
			nullptr,
			dll_name.size(),
			MEM_COMMIT,
			PAGE_READWRITE
		);
		//�ش� ���μ����� ���� �޸� ������ �޸𸮸� �Ҵ���. 
		if (!remote_buffer) {
			break;
		}
		if (!WriteProcessMemory(
			process_handle,
			remote_buffer,
			dll_name.c_str(),
			dll_name.size() * sizeof(wchar_t),
			nullptr)
			) {
			break;
		}
		// remote_buffer�� ��� �Ҵ�� �޸� ������ �ε�� DLL�� ��� ��Ʈ���� wrtie �Ѵ�. 
		module = GetModuleHandle(L"kernel32.dll");
		//kernel32.dll ����� �ڵ��� ������
		thread_start_routine = (LPTHREAD_START_ROUTINE)GetProcAddress(module, "LoadLibraryW");
		//��⿡ ��� Kernel32.dll ���� LoadLibrary�Լ� �����͸� ������ �����Ѵ�. 
		thread_handle = CreateRemoteThread(
			process_handle,
			nullptr,
			0,
			thread_start_routine,
			remote_buffer,
			0,
			nullptr
		);
		//LoadLibrary �Լ��� ������ ���� CreateRemoteThread�Լ��� �̿��Ͽ� Thread�� �����ϴ°� ó�� �ش� �Լ��� ������.
		//�̶� remote_buffer �޸� �������� �����ߴ� dll�� ��� ���ڿ��� ����ִ�.
		//���� LoadLibrary �Լ��� ���� ���� ������ dll�� load�� �� �ְ� �Ǵ°�. 
		WaitForSingleObject(thread_handle, INFINITE);
		result = true;
	} while (false);
	CloseHandle(process_handle);
	CloseHandle(thread_handle);
	return result;
}