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


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ����X���b�h
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Uint32 OperationThread(void* Arg)
{
	uint32_t seed  = time( NULL );		// �X���b�h����rand()����ꍇ�͓��X���b�h����srand()���Ȃ���srand�������Ȃ��B�H
	srand(seed);  

	while (gOperationThread.isLife()){
		if (g_Operation != 0){	// �ғ�����������B
			std::lock_guard<std::mutex> lock(gWorkMutex);
			gWorks[gWorkIndex]->proc();
		}
		Sleep(100);
	}

	return 0;
}

