// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  sub.h
// 
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include <stdafx.h>



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �����_��
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int random(int32_t MaxVal);
int randScatter(int32_t CenterVal, int32_t Width);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �X���[�v
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void APD_Sleep(int32_t msc);
// �߂��X���[�v�I������߂���
void APD_SleepAppend(int32_t msc);

void inside(int32_t &Val ,const int32_t &Min ,const int32_t &Max);

int32_t AP_pointingDesiredWindow(const CPoint &Point);

std::size_t  toNowText(TCHAR *Buf, std::size_t Size, int32_t Time);



