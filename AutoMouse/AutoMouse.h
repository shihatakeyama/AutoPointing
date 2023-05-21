
// AutoMouse.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CAutoMouseApp:
// このクラスの実装については、AutoMouse.cpp を参照してください。
//

class CAutoMouseApp : public CWinApp
{
public:
	CAutoMouseApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CAutoMouseApp theApp;