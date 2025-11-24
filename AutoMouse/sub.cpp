// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  sub.cpp
// 
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include <stdafx.h>

#include <mutex>

#include "define.h"
#include "global.h"
#include "GnrlThread.h"
#include "extern.h"
#include "GnrlString.h"

#include "sub.h"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ランダム
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int random(int32_t MaxVal)
{
	return rand() % MaxVal;
}
int randScatter(int32_t CenterVal, int32_t Width)
{
	int32_t ret = random(Width);
	return CenterVal + ret - (Width >> 1);
}

std::mutex gBPMutex;	// brief period
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// スリープ
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#define TIC		200
void APD_Sleep(int32_t msc)
{
	int32_t slp;

	gBPMutex.lock();
	gDelayRemine = msc + random(gBureTime);
	gBPMutex.unlock();

	while (g_Operation == EOS_operation){
		{
			std::lock_guard<std::mutex> lock(gBPMutex);
			if (gDelayRemine == 0){
				break;
			}
			if (gDelayRemine > TIC){
				slp = TIC;
				gDelayRemine -= TIC;
			}
			else{
				slp = gDelayRemine;
				gDelayRemine = 0;
			}
		}
		Sleep(slp);
	}
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 近いスリープ終了をやめたい
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void APD_SleepAppend(int32_t msc)
{
	if (g_Operation == EOS_non_operation) return;

	std::lock_guard<std::mutex> lock(gBPMutex);

	if (gDelayRemine < msc){
		gDelayRemine = msc;
	}
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Valの値がMinとMaxの範囲内に無ければ、範囲内に収めます。
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void limit(int32_t &Val ,const int32_t &Min ,const int32_t &Max)
{
	if(Val < Min)	Val = Min;
	if(Val > Max)	Val = Max;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 目的のウインドウがあれば、目的のウインドウにポインティングします。
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t AP_pointingDesiredWindow(const CPoint &Point)
{
	int32_t ack=ERC_ng;
	RECT rec;
	int32_t new_x,new_y;

	if(gTargetWindowName.empty()){
		new_x = randScatter(Point.x ,gBurePoint.x);
		new_y = randScatter(Point.y ,gBurePoint.y);

		ack = AM_click(new_x ,new_y);
				
	}else{
		// 目的のウインドウが設定されていれば
		ack = getTargetWindowPos(rec);
		if(ack < ERC_ok){
			// ウインドウが見つからない。
			return ERC_window_not_found;

		}else{
			// ウインドウ位置変換 ランダム
			new_x = randScatter(Point.x + rec.left ,gBurePoint.x);
			new_y = randScatter(Point.y + rec.top  ,gBurePoint.y);

			// 領域内チェック
			if (gInsideCheck){
				limit(new_x, rec.left, rec.right + gWindowMargin.x - gInsideMargin.x);
				limit(new_y, rec.top, rec.bottom + gWindowMargin.y - gInsideMargin.y);
			}

			ack = AM_click(new_x ,new_y);
		}
	}

	return ack;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// nowボタンのテキストを作成します
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
std::size_t  toNowText(TCHAR *Buf, std::size_t Size, int32_t Time)
{
	return Tsprintf_s(Buf, Size, _T("now+%d.%dh"), Time / 100, Time % 100);
}

