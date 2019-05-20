#include "DevicesHandler.h"

#include <cstring>
#include <iostream>
#include <memory>

int main(int argc, char *argv[])
{
	DevicesHandler devicesHandler;

	switch (argc) {
		case 2:
			if (strcmp(argv[1], "--list") == 0) {
				devicesHandler.enumerateNics();
				return 0;
			}

			break;
		case 3:
			const uint32_t index = strtoul(argv[1], nullptr, 10);

			if (index == 0 || errno == ERANGE) {
				std::cout << "Invalid index: " << argv[1] << std::endl << std::endl;
				break;
			}

			if (strcmp(argv[2], "--disable") == 0) {
				if (devicesHandler.toggleNetDevice(index, DevicesHandler::Disable)) {
					std::cout << "Network interface #" << index << " successfully disabled!" << std::endl;
					return 0;
				}

				return 1;
			} else if (strcmp(argv[2], "--enable") == 0) {
				if (devicesHandler.toggleNetDevice(index, DevicesHandler::Enable)) {
					std::cout << "Network interface #" << index << " successfully enabled!" << std::endl;
					return 0;
				}

				return 1;
			} else if (strcmp(argv[2], "--toggle") == 0) {
				if (devicesHandler.toggleNetDevice(index, DevicesHandler::Toggle)) {
					std::cout << "Network interface #" << index << " successfully toggled!" << std::endl;
					return 0;
				}

				return 1;
			} else {
				std::cout << "Invalid argument: " << argv[2] << std::endl << std::endl;
			}
	}

	std::cout << "----- Usage -----" << std::endl << std::endl;
	std::cout << "List interfaces: \"toggle_nic.exe --list\"" << std::endl;
	std::cout << "Disable interface #11: \"toggle_nic.exe 11 --disable\"" << std::endl;
	std::cout << "Enable interface #11: \"toggle_nic.exe 11 --enable\"" << std::endl;

	return 0;
}
