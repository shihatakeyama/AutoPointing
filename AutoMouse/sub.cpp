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
// �����_��
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
// �X���[�v
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#define TIC		200
void APD_Sleep(int32_t msc)
{
	int32_t slp;

	gBPMutex.lock();
	gDelayRemine = msc + random(gBureTime);
	gBPMutex.unlock();

	while (g_Operation != 0){
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
// �߂��X���[�v�I������߂���
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void APD_SleepAppend(int32_t msc)
{
	if (g_Operation == 0) return;

	std::lock_guard<std::mutex> lock(gBPMutex);

	if (gDelayRemine < msc){
		gDelayRemine = msc;
	}
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Val�̒l��Min��Max�͈͓̔��ɖ�����΁A�͈͓��Ɏ��߂܂��B
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void inside(int32_t &Val ,const int32_t &Min ,const int32_t &Max)
{
	if(Val < Min)	Val = Min;
	if(Val > Max)	Val = Max;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �ړI�̃E�C���h�E������΁A�ړI�̃E�C���h�E�Ƀ|�C���e�B���O���܂��B
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
		// �ړI�̃E�C���h�E���ݒ肳��Ă����
		ack = getTargetWindowPos(rec);
		if(ack >= ERC_ok){
			// �E�C���h�E�ʒu�ϊ� �����_��
			new_x = randScatter(Point.x + rec.left ,gBurePoint.x);
			new_y = randScatter(Point.y + rec.top  ,gBurePoint.y);

			// �̈���`�F�b�N
			if (gInsideCheck){
				inside(new_x, rec.left, rec.right);
				inside(new_y, rec.top, rec.bottom);
			}

			ack = AM_click(new_x ,new_y);
		}
	}

	return ack;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// now�{�^���̃e�L�X�g���쐬���܂�
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
std::size_t  toNowText(TCHAR *Buf, std::size_t Size, int32_t Time)
{
	return Tsprintf_s(Buf, Size, _T("now+%d.%dh"), Time / 100, Time % 100);
}

