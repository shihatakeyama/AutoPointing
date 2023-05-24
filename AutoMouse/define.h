// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// AM_defin.h
// 
// AutoMouse 定義
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include <stdint.h>

#include "GnrlNumerical.h"



#ifndef AM_DEFIN_H
#define AM_DEFIN_H


// ソフトバージョン
#define		SOFT_VERSION	0x0423514


#define TRX_BUF_SIZE   	32
#define RX_TIMEOUT		400
#define CR				0x0D	// コマンド終端

#define WINDOW_NAME	_T("AutoPointingDigitizer")


enum E_Notification{
  EN_mouse      = 1,
  EN_keyboard   = 2,
  EN_digitizer  = 3,
  EN_system		= 0x99
};

struct AM_Point{	// 旧仕様
	Sint32 x, y;
	Sint32 randx, randy;
	Sint32 wait;
};

struct TouchPoint{
	int32_t x, y;
	int32_t	delay;
};


// operation_Thread.cpp
struct AM_CommandFrame{
	Uint8 Command1;
	Uint8 Command2;
	Uint8 Len;					// Data長
	Uint8 Data[TRX_BUF_SIZE-3];
};

// リターンコード
enum E_ReturnCode {
	ERC_ok			=  0,
	ERC_open		= -1,
	ERC_baudrate	= -2,
	ERC_timeout		= -3,
	ERC_ng			= -5,
	ERC_othererr	= -99
};


#define WM_UPDATEDATA	(WM_APP+100)
#define WM_SHOW_VERSION (WM_APP+101)


#endif // #ifndef AM_DEFIN_H
