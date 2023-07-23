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

#include "GnrlCom.h"
#include "GnrlComList.h"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// アプリケーション名
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
std::wstring	gApplicatonName = _T("AutoPointing");

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 各種スレッド
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
GnrlThread	gMouseThread;
GnrlThread	gOperationThread;
GnrlThread	gRecvThread;

volatile int32_t	gDelayRemine = 0;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// シリアル通信
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
GnrlCom		gCom;

int32_t		g_Operation = 0;		// 稼働状態
int32_t		gActivePauseTime = 5000;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 操作リスト。
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
std::vector<AM_Point>		gPointVector;		/// ターゲットウインドウ座標基準

std::wstring				gTitle;
std::wstring				gTargetWindowName;
bool						gInsideCheck = true;			// ポイント位置がウインドウ枠内に収まっているかチェック
int32_t						gWindowDenominator=100;
CPoint						gWindowPos;
CPoint						gBasePoint;		// Pointing オフセット
CPoint						gBurePoint = CPoint(4,4);
int32_t						gBureTime = 4;
int32_t						gNowTime[3] = { 280, 280, 20 };
int32_t						gSpinTime = 10;

class CAutoPointingDlg;
CAutoPointingDlg			*pAutoPointingDlg;

class WorkBase;
std::vector<WorkBase*>		gWorks;
std::vector<std::wstring>	gWorkNames;
std::mutex					gWorkMutex;
int32_t						gInitWorkNo= 0;
int32_t						gWorkIndex = 0;	//  現在実行中のワーク
