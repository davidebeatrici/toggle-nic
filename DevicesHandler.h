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

		bool enumerateNics();
		bool toggleNic(const unsigned long &index, const Action &action);

		DevicesHandler();
		~DevicesHandler();
	private:
		void *handle;
};
