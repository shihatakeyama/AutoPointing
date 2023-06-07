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
// ���[�N����̃g�����U�N�V����
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

#if 1 // �e���V�����o�[�X�g�L
// �I�t�Z�b�g 626 ,280
AM_Point gPointList[] = { // �N���b�N���W
  { 1220 ,  520 , 48 , 310 , 2000 },	// �t�F�X�I�� �ʏ탉���_��
  { 1019 ,  728	, 16 ,  16 , 2500 },    // �X�L�b�v(�J�n�Ƌ���)  // ������t�F�X�I���Ɋ�����E
  {  602 ,  503	, 20 ,  20 ,  500 },	// FP��
  {  745 ,  619	, 12 ,  10 ,  500 },	// ���b�Z�[�W�{�b�N�XOK
//  {  724 ,  619	, 40 ,  40 , 9000 },	// ���b�Z�[�W�{�b�N�XOK
//  {  724 ,  620	, 40 ,  40 , 3000 },	// �҂�1
  {  922 ,  731 , 16 ,  16 , 1000 },	// �҂�1
  {  922 ,  747 , 16 ,  16 , 1000 },	// �҂�1
  {  922 ,  731 , 16 ,  16 , 1000 },	// �҂�1
  {  922 ,  747 , 16 ,  16 , 1000 },	// �҂�1
  {  922 ,  731 , 16 ,  16 , 1000 },	// �҂�1
  {  922 ,  747 , 16 ,  16 , 1000 },	// �҂�1
  {  922 ,  731 , 16 ,  16 , 1000 },	// �҂�1
  {  922 ,  747 , 16 ,  16 , 1000 },	// �҂�1
  {  646 ,  747 ,  8 ,   4 , 2500 },	// �҂�3
  {  646 ,  747 ,  8 ,   4 , 7000 },	// �҂�3
  { 1238 ,   82 ,  8,    8 , 1000 },	// ���m�点 �~
};
#endif

#if 1 // 10�A�K�`��
AM_Point gPointListGatya[] = {
  { 1076 , 556 , 40 , 40 , 1000 },  
  {  720 , 619 , 40 , 40 , 5000 }, // ���b�Z�[�W�{�b�N�XOK
};
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// vector �|�C���g�ɒl�����܂��B
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t setPointVector(int32_t OperationNo ,int32_t Offset_x ,int32_t Offset_y)
{
	int32_t i,end=0;
	AM_Point amp,*pamp=NULL;

	gPointVector.clear();
	
	if(OperationNo == 0){
		// �ʏ�t�F�X
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
// ����X���b�h
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Uint32 OperationThread(void* Arg)
{
	while (gOperationThread.isLife()){
		if (g_Operation != 0){	// �ғ�����������B
			std::lock_guard<std::mutex> lock(gWorkMutex);
			gWorks[gWorkIndex]->proc();
		}
		Sleep(100);
	}

	return 0;
}

