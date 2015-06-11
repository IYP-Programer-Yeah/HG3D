#include "..\Windows Handler\Windows Handler.h"
#include "..\HG3D Engine\HG3D Engine.h"
using namespace Windows_Handler;
bool init_main_window(HINSTANCE hInstance); //initialize main window


void CheckSystemInformation(); 
void DestroyCachedSystemInformation();

MSG* GetMSG(); //get main window's msg
HDC* GetHDC(); //get main window's hdc

int GetW();//get width
int GetH();//get height
bool Get_Wind_Update_Stat();//get if needs update