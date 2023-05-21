/***********************************************************************************
* Gnrl�ŕK�v�ȃf�t�@�C��
* Gnrl���R���p�C������\�[�X�́A�K���{�t�@�C�����C���N���[�h���ĉ������B
* 
* GnrlDefine.h
*  (�� GlobalDefine.h)
* 
* 2013/06/31  Createed
***********************************************************************************/

#ifndef GNRLDEFINE_H
#define GNRLDEFINE_H

// **** �A�v���P�[�V�����Ŏg�p����̋@�\�ɉ����ăf�t�@�C�����������ĉ������B ****
//#define USE_GNRLLIST
//#define USE_GNRLTABLE
#define USE_GNRLCONDITION
#define USE_GNRLBYTEORDER
//#define USE_GNRLEVENT
#define USE_STREAM_BUF


// **** �g�p����PC�̊����`���Ă������� ****
#define HAVE_CXX_VOLATILE	1		// �R���p�C����volatile�ɑΉ����Ă���ꍇ
#define WINDOWS_INTERFACE	1		// WINDOWS�Ŏg�p����ꍇ

#undef MAX_PATH
#define MAX_PATH	260				// ���ɉ����Ēl�𑝌����ĉ�����

#define SIZE_OF_BUF	(1024*4)		// �����񑀍�p�o�b�t�@�[�T�C�Y

#if WINDOWS_INTERFACE==1
//#include "stdafx.h"				// Microsoft VisualC++�g�p�̏ꍇ
#include <windows.h>
#endif

//#define _CRT_SECURE_NO_WARNINGS 	// CRT�̌x�����o�͂��Ȃ�
#pragma warning ( disable : 4996 ) 


#if 1
// **** �g�p����^���v���g�^�C�v ****
typedef	char			Sint8;
typedef	unsigned char	Uint8;
typedef	bool			Bool8;
typedef	bool			Bool;
typedef	int				Bool32;
typedef	void*			pVoid;		// ���@�ɔp�~�\��
typedef	void*			Pvoid;		// 32bit-64bit �|�C���^�݊��̈�
typedef short			Sint16;
typedef unsigned short	Uint16;
typedef	int				int32_t;
typedef unsigned int	Uint32;
typedef __int64			Sint64;
typedef unsigned __int64 Uint64;
typedef float			Float32;
typedef double			Float64;
#endif






// �������C�ӂ̏ꍇ�̃}�N��
#define GNRL_VA_LIST		va_list
#define GNRL_VA_START(a,b)	va_start(a,b)
#define GNRL_VA_ARG(a,b)	va_arg(a,b)
#define GNRL_VA_END(a)		va_end (a)

// �������s��̌Ăяo��
//#define GNRL_VSPRINTF	_vstprintf	// TCHAR.H �̃��[�`��
#define GNRL_VSPRINTF	vsprintf	// _UNICODE & _MBCS ������`�܂���_MBCS ����`����Ă���ꍇ
//#define GNRL_VSPRINTF	vswprintf	// _UNICODE ����`����Ă���ꍇ


#if 0
#define ZeroMemory(x,y) memset((x),0,(y))
#endif


// �L���X�g�͕p�ɂɌĂяo���ꍇ�́A
// �N���X���ɐ�p�����o�֐����쐬���������悢��������Ȃ��B

// �_�E���L���X�g�Ŏg�p���Ă��������B
#define GNRL_DOWN_CAST(To ,from){ assert(dynamic_cast<To>(from) == static_cast<To>(from) && "GNRL_STATIC_CAST ���s");	return static_cast<To>(from);}

// �t���[����ǂȂǁA�|�C���^�̌^�ϊ��Ŏg�p���Ă��������B
#define GNRL_REINTERPRET_CAST(t ,a) 	static_cast<t>((pVoid)a)


#endif // GNRLDEFINE


