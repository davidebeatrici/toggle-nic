#pragma once

class DevicesHandler
{
	public:
		enum Action
		{
			Disable,
			Enable,
			Toggle
		};

		static bool isRunningAsAdmin();
		bool enumerateNics();
		bool toggleNetDevice(const unsigned long &index, const Action &action);

		DevicesHandler();
		~DevicesHandler();
	private:
		void *handle;
};
