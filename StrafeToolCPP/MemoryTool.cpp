#include "MemoryTool.h"
#include <iostream>
#include "Windows.h"
#include "tlhelp32.h"
#include "psapi.h"

DWORD FindProcessId(const std::wstring& processName) {
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE) {
		return 0;
	}

	Process32First(processesSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile))
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processesSnapshot);
	return 0;
}
HANDLE ProcessByName(const std::wstring& processName) {
	DWORD processId = FindProcessId(processName);
	return OpenProcess(PROCESS_ALL_ACCESS, TRUE, processId);
}
MemoryTool::MemoryTool()
{
	baseClient = 0;
	HMODULE hModules[1024];
	hProcess = ProcessByName(L"csgo.exe");
	const int max_path = 256;
	char filePath[max_path];
	MODULEINFO moduleInfo;
	DWORD bytes_required;
	if (!EnumProcessModules(hProcess, hModules, sizeof(hModules), &bytes_required)) throw std::runtime_error("EnumProcessModules Failed!");
	int num_modules = bytes_required / sizeof(HMODULE);
	for (int i = 0; i < num_modules; i++) {
		if (!filePath)
			throw std::runtime_error("Could not create memory for file path buffer!");
		if (GetModuleFileNameExA(hProcess, hModules[i], filePath, max_path) > max_path)
			throw std::runtime_error("File Path too long!");
		if (std::string(filePath).find("client_panorama.dll") != std::string::npos) {
			GetModuleInformation(hProcess, hModules[i], &moduleInfo, sizeof(moduleInfo));
			baseClient = (int)moduleInfo.lpBaseOfDll;
			moduleSize = moduleInfo.SizeOfImage;
		}
	}
	if (baseClient == 0) throw std::runtime_error("Could not find base address of client!");
	playerPtr = baseClient + addresses::dwLocalPlayer;
	aPlayer = readInt(playerPtr);
	aVelocity = aPlayer + addresses::m_vecVelocity;
	aOrientation = aPlayer + addresses::m_vecViewOffset;
	oFlags = addresses::m_fFlags;
}
inline void* MemoryTool::readData(const int address, int numBytes) const {
	byte* data = new byte[numBytes];
	SIZE_T bytes_read;
	try {
		if (!ReadProcessMemory(hProcess, (LPCVOID)address, data, numBytes, &bytes_read))
			throw std::exception("Could not read from process memory!");
		else if (bytes_read != numBytes)
			throw std::exception("Could not read appropriate number of bytes from process memory!");
		return data;
	}
	catch (std::exception& e) {
		std::cerr << e.what() << '\n';
		std::cin.get();
	}
}
int MemoryTool::readInt(const int address) const {
	int result;
	std::memcpy(&result, readData(address, sizeof(result)), sizeof(result));
	return result;
}
float MemoryTool::readFloat(const int address) const {
	float result;
	std::memcpy(&result, readData(address, sizeof(result)), sizeof(result));
	return result;
}
byte MemoryTool::readByte(const int address) const {
	byte result;
	std::memcpy(&result, readData(address, sizeof(result)), sizeof(result));
	return result;
}
void MemoryTool::writeData(const int address, int numBytes, void* data) const{
	if (!WriteProcessMemory(hProcess, (LPVOID)address, data, numBytes, NULL))
		throw std::runtime_error("Could not write to process memory!");
}
void MemoryTool::writeInt(const int address, int input) const {
	writeData(address, sizeof(int), (void*)&input);
}
void MemoryTool::writeFloat(const int address, float input) const {
	writeData(address, sizeof(float), (void*)&input);
}