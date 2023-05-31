
// AutoPointing.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CAutoPointingApp:
// このクラスの実装については、AutoPointing.cpp を参照してください。
//

class CAutoPointing : public CWinApp
{
public:
	CAutoPointing();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CAutoPointing theApp;