#include "DevicesHandler.h"

#include "WinUtils.h"

#include <iostream>

#include <windows.h>
#include <initguid.h>
#include <devguid.h>
#include <cfgmgr32.h>
#include <setupapi.h>

DevicesHandler::DevicesHandler()
{
	handle = SetupDiGetClassDevs(&GUID_DEVCLASS_NET, nullptr, nullptr, DIGCF_PRESENT);

	if (handle == INVALID_HANDLE_VALUE) {
		std::cout << "DevicesHandler::DevicesHandler(): SetupDiGetClassDevs() failed with error: " << GetLastError() << std::endl;
	}
}

DevicesHandler::~DevicesHandler()
{
	if (handle == INVALID_HANDLE_VALUE) {
		return;
	}

	if (!SetupDiDestroyDeviceInfoList(handle)) {
		std::cout << "DevicesHandler::~DevicesHandler(): SetupDiDestroyDeviceInfoList() failed with error: " << GetLastError() << std::endl;
	}
}

bool DevicesHandler::enumerateNics()
{
	if (handle == INVALID_HANDLE_VALUE) {
		return false;
	}

	SP_DEVINFO_DATA deviceInfoData;
	deviceInfoData.cbSize = sizeof(deviceInfoData);

	for (DWORD i = 0; SetupDiEnumDeviceInfo(handle, i, &deviceInfoData); ++i) {
		DWORD requiredSize;

		if (!SetupDiGetDeviceRegistryProperty(handle, &deviceInfoData, SPDRP_FRIENDLYNAME, nullptr, nullptr, 0, &requiredSize)) {
			const DWORD err = GetLastError();
			if (err != ERROR_INSUFFICIENT_BUFFER) {
				std::cout << "DevicesHandler::enumerateNics(): SetupDiGetDeviceRegistryProperty() failed with error: " << err << std::endl;
				return false;
			}
		}

		std::wstring name(requiredSize, 0);

		if (!SetupDiGetDeviceRegistryProperty(handle, &deviceInfoData, SPDRP_FRIENDLYNAME, nullptr, reinterpret_cast<PBYTE>(&name[0]), name.size(), nullptr)) {
			std::cout << "DevicesHandler::enumerateNics(): SetupDiGetDeviceRegistryProperty() failed with error: " << GetLastError() << std::endl;
			return false;
		}

		std::cout << "Index: " << i << std::endl;
		std::wcout << "Name: " << name << std::endl << std::endl;
	}

	const DWORD err = GetLastError();

	if (err != ERROR_NO_MORE_ITEMS) {
		std::cout << "DevicesHandler::enumerateNics(): SetupDiEnumDeviceInfo() failed with error: " << err << std::endl;
		return false;
	}

	return true;
}

bool DevicesHandler::toggleNetDevice(const unsigned long &index, const Action &action)
{
	if (!WinUtils::isRunningAsAdmin()) {
		std::cout << "DevicesHandler::toggleNetDevice(): Administrator privileges are required in order to change a device's state.";
		return false;
	}

	if (handle == INVALID_HANDLE_VALUE) {
		return false;
	}

	SP_DEVINFO_DATA deviceInfoData;
	deviceInfoData.cbSize = sizeof(deviceInfoData);

	if (!SetupDiEnumDeviceInfo(handle, index, &deviceInfoData)) {
		std::cout << "DevicesHandler::toggleNetDevice(): SetupDiEnumDeviceInfo() failed with error: " << GetLastError() << std::endl;
		return false;
	}

	SP_PROPCHANGE_PARAMS propChangeParams;

	switch (action) {
		case Disable:
			propChangeParams.StateChange = DICS_DISABLE;
			break;
		case Enable:
			propChangeParams.StateChange = DICS_ENABLE;
			break;
		case Toggle: {
			ULONG status, problem;
			const CONFIGRET ret = CM_Get_DevNode_Status(&status, &problem, deviceInfoData.DevInst, 0);
			if (ret != CR_SUCCESS) {
				std::cout << "DevicesHandler::toggleNetDevice(): CM_Get_DevNode_Status() failed with error: " << ret << std::endl;
				return false;
			}

			propChangeParams.StateChange = problem == CM_PROB_DISABLED ? DICS_ENABLE : DICS_DISABLE;
			break;
		}
		default:
			std::cout << "DevicesHandler::toggleNetDevice(): Unrecognized action: " << action << std::endl;
			return false;
	}

	propChangeParams.HwProfile = 0;
	propChangeParams.Scope = DICS_FLAG_CONFIGSPECIFIC;
	propChangeParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
	propChangeParams.ClassInstallHeader.cbSize = sizeof(propChangeParams.ClassInstallHeader);

	if (!SetupDiSetClassInstallParams(handle, &deviceInfoData, &propChangeParams.ClassInstallHeader, sizeof(propChangeParams))) {
		std::cout << "DevicesHandler::toggleNetDevice(): SetupDiSetClassInstallParams() failed with error: " << GetLastError() << std::endl;
		return false;
	}

	if (!SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, handle, &deviceInfoData)) {
		const DWORD err = GetLastError();

		std::cout << "DevicesHandler::toggleNetDevice(): SetupDiCallClassInstaller() failed with error: " << err << std::endl;

		// Clear parameters, otherwise the device remains in an inconsistent state (e.g. with the enabled icon even if disabled).
		SetupDiSetClassInstallParams(handle, &deviceInfoData, nullptr, 0);

		return false;
	}

	return true;
}
