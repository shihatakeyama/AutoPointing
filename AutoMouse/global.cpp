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
// �A�v���P�[�V������
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
std::wstring	gApplicatonName = _T("AutoPointing");

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �e��X���b�h
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
GnrlThread	gMouseThread;
GnrlThread	gOperationThread;
GnrlThread	gRecvThread;

volatile int32_t	gDelayRemine = 0;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �V���A���ʐM
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
GnrlCom		gCom;

int32_t		g_Operation = 0;		// �ғ����
int32_t		gActivePauseTime = 5000;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ���샊�X�g�B
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
std::vector<AM_Point>		gPointVector;		/// �^�[�Q�b�g�E�C���h�E���W�

std::wstring				gTitle;
std::wstring				gTargetWindowName;
bool						gInsideCheck = true;			// �|�C���g�ʒu���E�C���h�E�g���Ɏ��܂��Ă��邩�`�F�b�N
int32_t						gWindowDenominator=100;
CPoint						gWindowPos;
CPoint						gBasePoint;		// Pointing �I�t�Z�b�g
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
int32_t						gWorkIndex = 0;	//  ���ݎ��s���̃��[�N
