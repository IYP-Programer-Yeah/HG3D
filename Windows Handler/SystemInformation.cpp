#include "SystemInformation.h"

namespace Windows_Handler
{
	SystemInformation* SystemInformation::_instance = nullptr;

	void SystemInformation::Initialize()
	{
		SYSTEM_INFO sysInfo;

		GetSystemInfo(&sysInfo);

		m_CPUCores = static_cast<BYTE>(sysInfo.dwNumberOfProcessors);

#ifdef NT_IS_DEBUGGING
		char s[50];
		sprintf_s(s, "Number of CPU Cores: %d\n", m_CPUCores);

		MessageBox(NULL, s, NULL, NULL);
#endif

	}

	void SystemInformation::Destroy()
	{
		if (_instance != nullptr)
		{
			delete _instance;
			_instance = nullptr;
		}
	}

	BYTE SystemInformation::GetNumCPUCores()const
	{
#ifdef NT_IS_DEBUGGING
		assert(m_CPUCores > 0);
#endif

		return m_CPUCores;
	}

	SystemInformation* SystemInformation::Instance()
	{
		if (_instance == nullptr)
		{
			_instance = new SystemInformation();
		}

		return _instance;
	}

}