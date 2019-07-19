// StrafeToolCPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Windows.h"
#include "tlhelp32.h"
#include "psapi.h"
#include "MemoryTool.h"
#include "Stopwatch.h"
#include "StrafeInfo.h"
#include <vector>
#include <string>

int getIntAt(const byte* result, const MemoryTool& mem, const int& x) {
	const int i = x - mem.baseClient;
	if (i < mem.moduleSize)
		return result[i + 3] << 24 | result[i + 2] << 16 | result[i + 1] << 8 | result[i];
	else {
		printf("i = %d but result.length = %d\n", i, mem.moduleSize);
		return -1;
	}
}

int getIntAt2(const byte* result, const int& i) {
	return result[i] << 24 | result[i + 1] << 16 | result[i + 2] << 8 | result[i + 3];
}

inline bool data_matches_pattern(const MemoryTool& mem, const std::vector<byte>& pattern, const byte* game_data, int base_address) {
	static int sz;
	if (pattern[0] != 0x00 && game_data[base_address] != pattern[0]) return false;
	sz = pattern.size();
	const byte* data_shifted = game_data + base_address;
	for (int i = 1; i < sz; i++)
		if (pattern[i] != 0x00 && pattern[i] != data_shifted[i]) return false;
	return true;
}

int indexOfPattern(std::vector<byte> pattern, const MemoryTool& mem, int offset) {
	byte* game_data = (byte*)mem.readData(mem.baseClient, mem.moduleSize);
	if (!game_data) throw std::runtime_error("Error or readData");
	bool match = true;
	for (int i = 0; i < mem.moduleSize - pattern.size(); i++) {
		if (data_matches_pattern(mem, pattern, game_data, i))
			return mem.baseClient + i + offset;
	}
	return -1;
}

int main()
{
	MemoryTool mem;
	StrafeInfo strafeInfo{ mem };
	while (true) {
		strafeInfo.update();
	}
	return 0;
}