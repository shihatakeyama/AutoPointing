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
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
std::wstring	gApplicatonName = _T("AutoPointingDigitizer");

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
std::vector<AM_Point>		gPointVector;		/// ターゲットウインドウ座標基準

std::wstring				gTitle;
std::wstring				gTargetWindowName;
CPoint						gBasePoint;
CPoint						gBurePoint = CPoint(4,4);
int32_t						gBureTime = 4;
int32_t						gNowTime[3] = { 280, 280, 20 };

class WorkBase;
std::vector<WorkBase*>		gWorks;
std::vector<std::wstring>	gWorkNames;
std::mutex					gWorkMutex;
int32_t						gInitWorkNo=0;
int32_t						gWorkNo = 0;	//  現在実行中のワーク
