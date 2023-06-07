// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  operation_Thread.cpp
// 
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


#include "stdafx.h"

#include <vector>
#include <mutex>
#include <string>

#include "WorkBase.h"

#include "AutoPointing.h"
#include "AutoPointingDlg.h"
#include "afxdialogex.h"
#include "Windows.h"

#include "GnrlNumerical.h"
#include "GnrlThread.h"
#include "Define.h"
#include "global.h"
//#define USE_MFC
#include "GnrlComList.h"
#include "GnrlCom.h"

#include "define.h"
#include "extern.h"
#include "global.h"
#include "sub.h"


#if 0
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ワーク一周のトランザクション
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t RotatingHorse1(const AM_Point *Point, int32_t Len)
{
	int32_t i;
	int32_t new_x,new_y;

	for (i = 0; i<Len; i++){
		if ((gOperationThread.isLife() == FALSE) || (g_Operation == 0) || !gCom.isOpened()){
			return 0;
		}

	    new_x = randScatter(Point[i].x ,Point[i].randx);
		new_y = randScatter(Point[i].y ,Point[i].randy);

		AM_click(new_x ,new_y);

	    Sleep(Point[i].wait + random(Point[i].wait>>2));
	}

	return 0;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Point List
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#if 1 // テンションバースト有
// オフセット 626 ,280
AM_Point gPointList[] = { // クリック座標
  { 1220 ,  520 , 48 , 310 , 2000 },	// フェス選択 通常ランダム
  { 1019 ,  728	, 16 ,  16 , 2500 },    // スキップ(開始と共通)  // 上方がフェス選択に干渉する・
  {  602 ,  503	, 20 ,  20 ,  500 },	// FP回復
  {  745 ,  619	, 12 ,  10 ,  500 },	// メッセージボックスOK
//  {  724 ,  619	, 40 ,  40 , 9000 },	// メッセージボックスOK
//  {  724 ,  620	, 40 ,  40 , 3000 },	// 待ち1
  {  922 ,  731 , 16 ,  16 , 1000 },	// 待ち1
  {  922 ,  747 , 16 ,  16 , 1000 },	// 待ち1
  {  922 ,  731 , 16 ,  16 , 1000 },	// 待ち1
  {  922 ,  747 , 16 ,  16 , 1000 },	// 待ち1
  {  922 ,  731 , 16 ,  16 , 1000 },	// 待ち1
  {  922 ,  747 , 16 ,  16 , 1000 },	// 待ち1
  {  922 ,  731 , 16 ,  16 , 1000 },	// 待ち1
  {  922 ,  747 , 16 ,  16 , 1000 },	// 待ち1
  {  646 ,  747 ,  8 ,   4 , 2500 },	// 待ち3
  {  646 ,  747 ,  8 ,   4 , 7000 },	// 待ち3
  { 1238 ,   82 ,  8,    8 , 1000 },	// お知らせ ×
};
#endif

#if 1 // 10連ガチャ
AM_Point gPointListGatya[] = {
  { 1076 , 556 , 40 , 40 , 1000 },  
  {  720 , 619 , 40 , 40 , 5000 }, // メッセージボックスOK
};
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// vector ポイントに値を入れます。
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t setPointVector(int32_t OperationNo ,int32_t Offset_x ,int32_t Offset_y)
{
	int32_t i,end=0;
	AM_Point amp,*pamp=NULL;

	gPointVector.clear();
	
	if(OperationNo == 0){
		// 通常フェス
		pamp = gPointList;
		end = sizeof(gPointList)/sizeof(AM_Point);
		for(i=0;i<end;i++){
			amp = pamp[i];
			amp.x -= Offset_x;
			amp.y -= Offset_y;
			gPointVector.push_back(amp);
		}
	}else if(OperationNo == 1){
		AM_Point fes = { 1220 , 393, 48, 40, 2000 };
		pamp = gPointList;
		end = sizeof(gPointList)/sizeof(AM_Point);
		for(i=0;i<end;i++){
			if(i==0){
				amp = fes;
			}else{
				amp = pamp[i];
				amp.x -= Offset_x;
				amp.y -= Offset_y;
			}
			gPointVector.push_back(amp);
		}
	}else if(OperationNo == 2){
		AM_Point fes = { 1220 , 465, 48, 40, 2000 };
		pamp = gPointList;
		end = sizeof(gPointList)/sizeof(AM_Point);
		for(i=0;i<end;i++){
			if(i==0){
				amp = fes;
			}else{
				amp = pamp[i];
				amp.x -= Offset_x;
				amp.y -= Offset_y;
			}
			gPointVector.push_back(amp);
		}

	}else if(OperationNo == 3){
		pamp = gPointListGatya;
		end = sizeof(gPointListGatya)/sizeof(AM_Point);
		for(i=0;i<end;i++){
			amp = pamp[i];
			amp.x -= Offset_x;
			amp.y -= Offset_y;
			gPointVector.push_back(amp);
		}
	}

	return 0;
}
#endif


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 操作スレッド
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Uint32 OperationThread(void* Arg)
{
	while (gOperationThread.isLife()){
		if (g_Operation != 0){	// 稼働中だったら。
			std::lock_guard<std::mutex> lock(gWorkMutex);
			gWorks[gWorkIndex]->proc();
		}
		Sleep(100);
	}

	return 0;
}

