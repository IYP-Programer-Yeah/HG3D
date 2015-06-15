#include "..\Windows Handler\Windows Handler.h"
#include "..\Windows Handler\SystemInformation.h"
#include "..\HG3D Engine\HG3D Engine.h"
#include "..\Physics\PhysicsWorld.h"

#include <time.h>
#define Mouse_Left_Stat    1
#define Mouse_Right_Stat   2
#define Mouse_Middle_Stat  3

using namespace Windows_Handler;
bool init_main_window(HINSTANCE hInstance); //initialize main window


void CheckSystemInformation(); 
void DestroyCachedSystemInformation();

MSG* GetMSG(); //get main window's msg
HDC* GetHDC(); //get main window's hdc

int GetW();//get width
int GetH();//get height
bool Get_Wind_Update_Stat();//get if needs update
bool Get_Mouse_Stat(int Stat_ID);//get mouse stats
int Get_Mouse_X();//get x
int Get_Mouse_Y();//get y
int Get_Mouse_Last_X();//get last x
int Get_Mouse_Last_Y();//get last y
int Get_Mouse_Wheel_Delta();//get mouse wheel's movment
bool Update_Wnd();