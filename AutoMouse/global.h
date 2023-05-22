// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// global.h
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#ifndef GLOBAL_H
#define GLOBAL_H


#include "stdafx.h"

#include <thread>
#include <mutex>
#include <vector>

#include "GnrlNumerical.h"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 各種スレッド
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
class GnrlThread;
extern GnrlThread gMouseThread;
extern GnrlThread gOperationThread;
extern GnrlThread gRecvThread;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// シリアル通信
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
class GnrlCom;
extern GnrlCom gCom;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 操作リスト。
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
extern std::vector<AM_Point>			gPointVector;		/// ターゲットウインドウ座標基準

extern std::wstring						gTitle;
extern CPoint							gBurePoint;
extern int32_t							gBureTime;

class WorkBase;
extern std::vector<WorkBase*>			gWorks;
extern std::vector<std::wstring>		gWorkNames;
extern std::mutex						gWorkMutex;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 各種設定
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
extern CPoint							gBurePoint;
extern int32_t							gBureTime;


class CAutoMouseDlg;
extern CAutoMouseDlg *pCAutoMouseDlg;
int32_t callOperationThread();
int32_t getDoaxWindowPos(RECT &Rect);


#endif // #ifndef GLOBAL_H

