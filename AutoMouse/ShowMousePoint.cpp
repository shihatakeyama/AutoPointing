// *********************************************************************************
// ShowMousePoint.cpp
// 
//  2018/12/08	�V�K�쐬
//  
// *********************************************************************************


#include "stdafx.h"
#include "AutoMouse.h"
#include "AutoMouseDlg.h"
#include "afxdialogex.h"

#include "Windows.h"

#include "GnrlNumerical.h"
#include "GnrlThread.h"
#include "AM_Define.h"
#include "AM_extern.h"


//DWORD WINAPI MousePointThread(LPVOID Arg);

//HANDLE ghMouseThread = NULL;
//DWORD gdwThreadId;


// *********************************************************************************
// ���݂̃}�E�X�|�C���^�ʒu���X�V����B
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

			ack = getDoaxWindowPos(rect);
			if(ack < 0){
				strw = _T("-------  -------");
			}else{
				strw.Format(_T("x=%5d , y=%5d"), point.x - rect.left , point.y - rect.top);
			}

			pCAutoMouseDlg->OnMousePoint(strm ,strw);

			oldpoint = point;
		}
	}


	return 0;
}

