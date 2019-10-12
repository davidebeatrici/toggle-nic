#include "WinUtils.h"

#include <iostream>

#include <windows.h>

bool WinUtils::isRunningAsAdmin()
{
	bool asAdmin = false;
	HANDLE hToken;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
		std::cout << "WinUtils::isRunningAsAdmin(): OpenProcessToken() failed with error: " << GetLastError();
		CloseHandle(hToken);
		return false;
	}

	DWORD requiredSize;
	TOKEN_ELEVATION elevation;

	if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &requiredSize)) {
		asAdmin = elevation.TokenIsElevated;
	} else {
		const DWORD err = GetLastError();
		std::cout << "WinUtils::isRunningAsAdmin(): GetTokenInformation() failed with error: " << err << std::endl;

		if (err == ERROR_INVALID_PARAMETER) {
			std::cout << "WinUtils::isRunningAsAdmin(): Windows versions older than Vista are not supported." << std::endl;
		}
	}

	CloseHandle(hToken);

	return asAdmin;
}
