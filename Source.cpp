#include <iostream>
#include <Windows.h>

using namespace std;




class Bypass {



public:
	Bypass();
	~Bypass();


	bool Attach(DWORD dwPid);
	bool Read(uintptr_t lpBaseAddress, void* lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesRead = NULL);
	bool Write(uintptr_t lpBaseAddress, void* lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten = NULL);

private:
	HANDLE m_hProcess = NULL;

};

Bypass::Bypass() {

}
Bypass::~Bypass() {
	if (m_hProcess != NULL) CloseHandle(m_hProcess);
}

bool Bypass::Attach(DWORD dwPid) {
	m_hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, dwPid);
	if (m_hProcess != NULL) return true; return false;
}
bool Bypass::Read(uintptr_t lpBaseAddress, void* lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesRead) {
	if (ReadProcessMemory(m_hProcess, (LPCVOID)lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead)) return true; return false;
}
bool Bypass::Write(uintptr_t lpBaseAddress, void* lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten) {
	if (WriteProcessMemory(m_hProcess, (LPVOID)lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten)) return true; return false;
}

int main() {
	
	DWORD pid = 0;
	cout << "PID: ";
	cin >> dec >> pid;

	Bypass* bypass = new Bypass();
	//Si ca marche pas
	if (!bypass->Attach(pid)) {
		cout << "OpenProcess failed. GetLastError = " << dec << GetLastError() << endl;
		system("pause");
		return EXIT_FAILURE;
	}

	//
	// Demande a l'utilisateur l'adresse

	uintptr_t memoryAddress = 0x0;
	cout << "The Adress (in hexadecimal): 0x";
	cin >> hex >> memoryAddress;
	cout << "Reading 0x" << hex << uppercase << memoryAddress << " ..." << endl;
	bool choice;
	cout << "Write or Read || 0 or 1 : ";
	cin >> choice;
	// Reading the integer from other process
	if (choice == 1) {

	int intRead = 0;
	if (!bypass->Read(memoryAddress, &intRead, sizeof(int))) {
		cout << "ReadProcessMemory failed. GetLastError = " << dec << GetLastError() << endl;
		system("pause");
		return EXIT_FAILURE;

	}
	cout << dec << intRead << endl;
	system("pause");

	}
	else {
		int intWrite = 0;
		int intRead = 0;
		if (!bypass->Write(memoryAddress, &intWrite, sizeof(int))) {
			cout << "ReadProcessMemory failed. GetLastError = " << dec << GetLastError() << endl;
			system("pause");
			return EXIT_FAILURE;
		}
		cout << "Writing successfuly accomplished" << endl;
		system("pause");

	}
	
}
