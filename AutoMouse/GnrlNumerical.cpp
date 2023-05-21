// *********************************************************************************
// GnrlNumerical.cpp
//  整数を変換します。
// *********************************************************************************


#include "stdafx.h"
#include "GnrlNumerical.h"


// *********************************************************************************
// 配列/整数 変換
// 引数はリトルエンディアン
// *********************************************************************************
Uint16 Uint8ArrowToUint16(const Uint8 * data)
{
	return  ((Uint16)data[0] << 8) | (Uint16)data[1];
}
Uint32 Uint8ArrowToUint32(const Uint8 * data)
{
	return  ((Uint32)data[0] << 24) | (Uint32)data[1] << 16 | ((Uint32)data[2] << 8) | (Uint32)data[3];
}

void Uint16ToUint8Arrow(Uint8 * Result ,Uint16 Src)
{
	Result[0] = (Uint8)(Src >> 8);
	Result[1] = (Uint8)(Src);
}

// *********************************************************************************
// AsciiをIntに変換します。
// *********************************************************************************
Uint8 asciiToInt(Sint8 c)
{
  if('0' <= c && c <= '9') return (c-'0');
  if('A' <= c && c <= 'F') return (c-'A'+10);
  if('a' <= c && c <= 'f') return (c-'a'+10);
  return 0;
}

// *********************************************************************************
// 数値を16進ダンプ変換します。
// 戻り値：OutAscii出力した長さ
// *********************************************************************************
Sint32 intArrowToDump16(Sint8 *OutAscii ,const Uint8 *Src ,Sint32 Len)
{
	const static Sint8 ictbl[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	Sint32 i,ii=0;

	for(i=0;i<Len;i++){
		OutAscii[ii] = ictbl[Src[i] >> 4];
		ii++;
		OutAscii[ii] = ictbl[Src[i] & 0x0F];	
		ii++;
	}

	return ii;
}

