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

#include "AutoMouse.h"
#include "AutoMouseDlg.h"
#include "afxdialogex.h"
#include "Windows.h"

#include "GnrlNumerical.h"
#include "GnrlThread.h"
#include "Define.h"
#include "GnrlComList.h"
#include "GnrlCom.h"

#include "define.h"
#include "extern.h"
#include "global.h"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// デジタイザ通信
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
static uint8_t gTxBuf[TRX_BUF_SIZE*2];		// ASCII
static uint8_t gRxBuf[TRX_BUF_SIZE*2];		// バイナリ
static int16_t gRxCo = 0;
static int16_t gRxTimer = 0;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// デジタイザへコマンドを送信します。
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t sendCommand(uint8_t Comand1 ,uint8_t Comand2 ,const uint8_t *Data ,uint8_t Len)
{
	int32_t len,ack;

//	len = intArrowToDump16((Sint8*)gTxBuf ,(uint8_t*)Paran ,Paran->Len+2);

	intArrowToDump16((Sint8*)&gTxBuf[0] ,&Comand1 ,sizeof(Comand1));
	intArrowToDump16((Sint8*)&gTxBuf[2] ,&Comand2 ,sizeof(Comand2));
	len = 4;

	intArrowToDump16((Sint8*)&gTxBuf[4] ,Data ,Len);
	len += Len * 2;

	gTxBuf[len] = CR;
	len++;

	ack = gCom.write(gTxBuf ,len);
	if(ack < 0){
		return -4;
	}

	return 0;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 画面の解像度を設定
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 int32_t AM_setDisplayResolution(int resolution_x,int resolution_y)
 {
	uint8_t buf[8];

	Uint16ToUint8Arrow(&buf[0] ,resolution_x);
	Uint16ToUint8Arrow(&buf[2] ,resolution_y);

	return sendCommand(EN_digitizer ,0x90 ,buf ,4);
 }
  
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 画面押して離す
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t AM_click(int x ,int y)
{
	uint8_t buf[8];

	Uint16ToUint8Arrow(&buf[0] ,x);
	Uint16ToUint8Arrow(&buf[2] ,y);

	return sendCommand(EN_digitizer ,0x07 ,buf ,4);
}

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
int32_t RotatingHorse2(const AM_Point *Point, int32_t Len)
{
	int32_t i,ack;
	int32_t new_x,new_y;
	RECT rec;

	for (i = 0; i<Len; i++){

		while (gAddSleep){
			Sleep(1000);	// 追加でSleepさせたい場合
		}

		if ((gOperationThread.isLife() == FALSE) || (g_Operation == 0) || !gCom.isOpened()){
			return 0;
		}

		ack = getDoaxWindowPos(rec);
		if(ack >= 0){
		
			new_x = randScatter(Point[i].x + rec.left ,Point[i].randx);
			new_y = randScatter(Point[i].y + rec.top  ,Point[i].randy);

			AM_click(new_x ,new_y);
		}

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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 操作メイン
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

// 操作スレッド ループ
Uint32 OperationThread(void* Arg)
{

	while (gOperationThread.isLife()){
		if (g_Operation != 0){	// 稼働中だったら。
//			RotatingHorse1(gPointList, sizeof(gPointList) / sizeof(AM_Point));
			RotatingHorse2(&gPointVector[0], gPointVector.size());

		}
		Sleep(1000);
	}

///	ghOperationThread = NULL;

	return 0;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// デジタイザのバージョン をメインスレッドへ通知
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void recvVersion(const uint8_t *Data ,uint8_t Len)
{
	Uint32 ver = Uint8ArrowToUint32(&Data[2]);

	pCAutoMouseDlg->PostMessage(WM_SHOW_VERSION, ver);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// システム系のコマンド受けた。
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void cmdSystem(const uint8_t *Data ,uint8_t Len)
{
	switch(Data[1]){
		case 0x99:
			recvVersion(Data ,Len);
			break;
	}
}

char gDebugBuf[64];

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 受信をポーリングされながら
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Uint32 Recv_chk()
{
	return 0;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 受信スレッド ループ
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Uint32 RecvThread(void* Arg)
{
	int32_t	ack;
	uint8_t	rxcc,rxco_h;

	while(gRecvThread.isLife()){

		ack = gCom.read(&rxcc ,sizeof(rxcc));
		if(ack > 0){
		    rxco_h = gRxCo >> 1;		
			if(rxcc != CR){
			  // 0.5バイトづつバッファにいれる。

			  if((gRxCo & 0x01) == 0){
				gRxBuf[rxco_h] = asciiToInt((uint8_t)rxcc) << 4;
			  }else{
				gRxBuf[rxco_h] |= asciiToInt((uint8_t)rxcc);        
			  }
#if 1
				// 通信デバグ用
				gDebugBuf[gRxCo] = rxcc;
#endif

			  gRxCo++;
			  if(gRxCo > TRX_BUF_SIZE){
				gRxCo = 0;
			  }
       
			}else{
				  // フレーム受信完了
				  if((gRxCo & 0x01) == 0){
					switch(gRxBuf[0]){
					  case EN_mouse:  // マウス
						break;
					  case 0xD0:	// デバグ
						
						break;
					  case 0x99:	// システム、メンテナンス
						cmdSystem(&gRxBuf[0] ,rxco_h);
					  default:
						break;
					}
				 }
				 gRxCo = 0;
			}
			gRxTimer = RX_TIMEOUT;
		}else{
			if(gRxTimer>0){
				gRxTimer--;
			}else{
				gRxCo = 0;	// RX_TIMEOUTの期間Byt取得できなかったら、バッファ位置0クリア。
			}

			Sleep(10);
		}
	}
	return 0;
}


