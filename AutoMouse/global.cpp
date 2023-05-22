// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// global.cpp
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"

#include <vector>
#include <mutex>
#include <string>

#include "GnrlThread.h"
#include "define.h"

#define  USE_MFC
#include "GnrlComList.h"
#include "GnrlCom.h"



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 各種スレッド
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
GnrlThread	gMouseThread;
GnrlThread	gOperationThread;
GnrlThread	gRecvThread;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// シリアル通信
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
GnrlCom		gCom;

int32_t		g_Operation = 0;		// 稼働状態
int32_t		gAddSleep = 0;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 操作リスト。
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
std::vector<AM_Point>			gPointVector;		/// ターゲットウインドウ座標基準

std::wstring					gTitle;
CPoint							gBurePoint;
int32_t							gBureTime = 0;

class WorkBase;
std::vector<WorkBase*>			gWorks;
std::vector<std::wstring>		gWorkNames;
std::mutex						gWorkMutex;


