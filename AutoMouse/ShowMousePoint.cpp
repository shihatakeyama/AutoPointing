// *********************************************************************************
// ShowMousePoint.cpp
// 
//  2018/12/08	新規作成
//  
// *********************************************************************************


#include "stdafx.h"
#include "AutoPointing.h"
#include "AutoPointingDlg.h"
#include "afxdialogex.h"

#include "Windows.h"

#include "GnrlNumerical.h"
#include "GnrlThread.h"
#include "Define.h"
#include "extern.h"
#include "global.h"


//DWORD WINAPI MousePointThread(LPVOID Arg);

//HANDLE ghMouseThread = NULL;
//DWORD gdwThreadId;


// *********************************************************************************
// 現在のマウスポインタ位置を更新する。
// *********************************************************************************
//DWORD WINAPI MousePointThread(LPVOID Arg)
Uint32 MousePointThread(void *Arg)
{
	Sint32 ack;
	POINT point,oldpoint={0,0};
	CString strm,strw;
	RECT rect;

	while (gMouseThread.isLife())
	{
		Sleep(100);

		GetCursorPos(&point);

		if((point.x != oldpoint.x) || (point.y != oldpoint.y)){
			
			strm.Format(_T("x=%5d , y=%5d"), point.x, point.y);

			ack = getTargetWindowPos(rect);
			if(ack < 0){
				strw = _T("-------  -------");
			}else{
				strw.Format(_T("x=%5d , y=%5d"), point.x - rect.left , point.y - rect.top);
			}

			pAutoPointingDlg->OnMousePoint(strm, strw);

			oldpoint = point;
		}
	}


	return 0;
}

