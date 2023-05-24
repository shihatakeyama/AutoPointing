// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  sub.cpp
// 
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include <stdafx.h>

#include "define.h"
#include "global.h"
#include "GnrlThread.h"
#include "sub.h"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ランダム
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int random(int MaxVal)
{
	return rand() % MaxVal;
}
int randScatter(int CenterVal, int Width)
{
	int32_t ret = random(Width);
	return CenterVal + ret - (Width >> 1);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// スリープ
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#define TIC		200
static int32_t sscdn;
void APD_Sleep(int32_t msc)
{
	int32_t slp;
	sscdn = msc + random(gBureTime);

	while (gOperationThread.isLife()){
		if (sscdn > TIC){
			slp = TIC;
			sscdn -= TIC;
		}
		else{
			slp		= sscdn;
			sscdn = 0;
		}
		Sleep(slp);
		if (sscdn == 0){
			break;
		}
	}
}
// 近いスリープ終了をやめたい
void APD_SleepAppend(int32_t msc)
{
	if (sscdn < msc){
		sscdn = msc;
	}
}

