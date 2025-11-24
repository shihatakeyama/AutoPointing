// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// defin.h
// 
// AutoPointing 定義
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include <stdint.h>

#include "GnrlNumerical.h"



#ifndef AM_DEFIN_H
#define AM_DEFIN_H


// ソフトバージョン
#define		SOFT_VERSION	0x0425C240

#define		SPY_MODE		0		// 1:外部から自ウインドウ名をキャプチャされるのを防ぐ


#define TRX_BUF_SIZE   	32
#define RX_TIMEOUT		400
#define CR				0x0D		// コマンド終端

// #define WINDOW_NAME	_T("AutoPointing")


enum E_Notification{
	EN_mouse		= 1,
	EN_keyboard		= 2,
	EN_digitizer	= 3,
	EN_system		= 0x99
};

enum E_WindowPosBit{
	EWP_none	= 0x00,
	EWP_top		= 0x01,
	EWP_vcenter	= 0x02,
	EWP_bottom	= 0x03,
	EWP_left	= 0x04,
	EWP_hcenter	= 0x08,
	EWP_right	= 0x0C,
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

// 稼働状態
enum E_OperationState{
	  EOS_non_operation	= 0
	, EOS_operation		= 1
	, EOS_stop_req		= 2
};

// リターンコード
enum E_ReturnCode {
	ERC_ok			=  0,
	ERC_open		= -1,
	ERC_baudrate	= -2,
	ERC_timeout		= -3,
	ERC_ng			= -5,
	ERC_invalid_call = -13,
	ERC_cmd_send	= -14,
	ERC_window_not_found	= -15,
	ERC_othererr	= -99
};

// ユーザー使用メッセージID
#define WM_UPDATEDATA	(WM_APP+100)
#define WM_SHOW_VERSION (WM_APP+101)
#define WM_COMMENT		(WM_APP+102)
#define WM_CNT			(WM_APP+103)



#endif // #ifndef AM_DEFIN_H
