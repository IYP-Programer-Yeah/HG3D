#ifdef _DLLEXPORT
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif

//Should be removed while shipping
#define NT_IS_DEBUGGING