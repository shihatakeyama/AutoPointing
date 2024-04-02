// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// defin.h
// 
// AutoPointing ��`
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include <stdint.h>

#include "GnrlNumerical.h"



#ifndef AM_DEFIN_H
#define AM_DEFIN_H


// �\�t�g�o�[�W����
#define		SOFT_VERSION	0x04244020

#define		SPY_MODE		0		// 1:�O�����玩�E�C���h�E�����L���v�`�������̂�h��


#define TRX_BUF_SIZE   	32
#define RX_TIMEOUT		400
#define CR				0x0D		// �R�}���h�I�[

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

struct AM_Point{	// ���d�l
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
	Uint8 Len;					// Data��
	Uint8 Data[TRX_BUF_SIZE-3];
};

// ���^�[���R�[�h
enum E_ReturnCode {
	ERC_ok			=  0,
	ERC_open		= -1,
	ERC_baudrate	= -2,
	ERC_timeout		= -3,
	ERC_ng			= -5,
	 ERC_invalid_call = -13,
	ERC_othererr	= -99
};

// ���[�U�[�g�p���b�Z�[�WID
#define WM_UPDATEDATA	(WM_APP+100)
#define WM_SHOW_VERSION (WM_APP+101)
#define WM_COMMENT		(WM_APP+102)


#endif // #ifndef AM_DEFIN_H
