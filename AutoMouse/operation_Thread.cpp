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
// 操作スレッド
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Uint32 OperationThread(void* Arg)
{
	uint32_t seed  = time( NULL );		// スレッド中でrand()する場合は当スレッド中でsrand()しないとsrandが効かない。？
	srand(seed);  

	while (gOperationThread.isLife()){
		if (g_Operation != 0){	// 稼働中だったら。
			std::lock_guard<std::mutex> lock(gWorkMutex);
			gWorks[gWorkIndex]->proc();
		}
		Sleep(100);
	}

	return 0;
}

