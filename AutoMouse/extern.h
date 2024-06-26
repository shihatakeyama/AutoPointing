// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// extern.h
//
// AutoPointing 定義
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#ifndef AM_EXTERN_H
#define AM_EXTERN_H


#include <mutex>

#include "GnrlNumerical.h"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// パラメーター
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t initParam();
int32_t clearParam();

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int randScatter(int CenterVal ,int Width);

// **** operation_Thread.cpp ****
uint32_t OperationThread(void* Arg);
uint32_t RecvRootine(void* Arg);


int32_t APD_sendCommand(Uint8 Comand1 ,Uint8 Comand2 ,const Uint8 *Data ,Uint8 Len);

//extern int32_t g_Life;
extern int32_t	g_Operation;
extern int32_t	gActivePauseTime;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// デジタイザコマンド
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t AM_setDisplayResolution(int resolution_x,int resolution_y);
int32_t AM_click(int x ,int y);



#endif // #ifndef AM_EXTERN_H