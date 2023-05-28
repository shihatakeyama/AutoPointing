// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// AM_extern.h
//
// AutoMouse 定義
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#ifndef AM_EXTERN_H
#define AM_EXTERN_H


#include <mutex>

#include "GnrlNumerical.h"

// 各種設定
extern CPoint							gBurePoint;
extern int32_t							gBureTime;

extern std::wstring						gTitle;
class ProcBase;
extern std::vector<ProcBase*>			gWorks;
extern std::vector<std::wstring>		gWorkNames;
extern std::mutex						gWorkMutex;


// 各種スレッド
class GnrlThread;
extern GnrlThread gMouseThread;
extern GnrlThread gOperationThread;
extern GnrlThread gRecvThread;

class CAutoMouseDlg;
extern CAutoMouseDlg *pCAutoMouseDlg;
int32_t callOperationThread();
int32_t getDoaxWindowPos(RECT &Rect);

// 
int randScatter(int CenterVal ,int Width);

// **** ShowMousePoint.cpp ****
int32_t callMousePointThread();
uint32_t MousePointThread(void *Arg);
//DWORD WINAPI MousePointThread(LPVOID Arg);

// **** ShowMousePoint.cpp ****
uint32_t OperationThread(void* Arg);
uint32_t RecvThread(void* Arg);
int32_t RotatingHorse(const AM_Point *Point, int32_t Len);


//int32_t sendCommand(const AM_CommandFrame *Paran);
int32_t sendCommand(Uint8 Comand1 ,Uint8 Comand2 ,const Uint8 *Data ,Uint8 Len);

//extern int32_t g_Life;
extern int32_t	g_Operation;
extern int32_t	gAddSleep;

int random(int MaxVal);

// USBから出力するコマンド
int32_t AM_setDisplayResolution(int resolution_x,int resolution_y);
int32_t AM_click(int x ,int y);



#endif // #ifndef AM_EXTERN_H