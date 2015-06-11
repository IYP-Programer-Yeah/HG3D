#include <Windows.h>
#include <assert.h>
#include <iostream>


#define NT_IS_DEBUGGING

namespace Windows_Handler
{
	//We only need a singleton

	class SystemInformation
	{
	public:
		void __declspec(dllexport) Initialize();
		void __declspec(dllexport) Destroy();

		static __declspec(dllexport) SystemInformation* Instance();

		BYTE __declspec(dllexport) GetNumCPUCores()const;
	private:
		//Prevent copying
		SystemInformation() {}
		SystemInformation(const SystemInformation&) {}
		~SystemInformation() {}
		void operator=(const SystemInformation&) {}


		BYTE m_CPUCores;

		static SystemInformation* _instance;
	};


}