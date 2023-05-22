// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  sub.cpp
// 
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include <stdafx.h>


#include "sub.h"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ƒ‰ƒ“ƒ_ƒ€
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

