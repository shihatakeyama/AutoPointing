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

#define  USE_MFC
#include "GnrlComList.h"
#include "GnrlCom.h"



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
std::vector<AM_Point>			gPointVector;		/// �^�[�Q�b�g�E�C���h�E���W�

std::wstring					gTitle;
CPoint							gBurePoint;
int32_t							gBureTime = 0;

class WorkBase;
std::vector<WorkBase*>			gWorks;
std::vector<std::wstring>		gWorkNames;
std::mutex						gWorkMutex;


