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
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
std::wstring	gApplicatonName = _T("AutoPointing");

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �e��X���b�h
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
GnrlThread	gMouseThread;
GnrlThread	gOperationThread;
GnrlThread	gRecvThread;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �V���A���ʐM
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
GnrlCom		gCom;

int32_t		g_Operation = 0;		// �ғ����
int32_t		gAddSleep = 0;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ���샊�X�g�B
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
std::vector<AM_Point>		gPointVector;		/// �^�[�Q�b�g�E�C���h�E���W�

std::wstring				gTitle;
std::wstring				gTargetWindowName;
enum E_WindowPosBit			gEWindowPosBit=EWP_none;		// �E�C���h�E�ʒu
int32_t						gWindowDenominator=100;
CPoint						gWindowPos;
CPoint						gBasePoint;		// Pointing �I�t�Z�b�g
CPoint						gBurePoint = CPoint(4,4);
int32_t						gBureTime = 4;
int32_t						gNowTime[3] = { 280, 280, 20 };

class WorkBase;
std::vector<WorkBase*>		gWorks;
std::vector<std::wstring>	gWorkNames;
std::mutex					gWorkMutex;
int32_t						gInitWorkNo=0;
int32_t						gWorkNo = 0;	//  ���ݎ��s���̃��[�N
