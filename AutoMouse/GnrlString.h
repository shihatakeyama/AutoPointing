// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  GnrlString.h
//
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


#ifndef GNRL_NCHAR_H
#define GNRL_NCHAR_H



//  プロジェクトで設定する文字の扱いを吸収する。
#if 0
	
	// マルチバイト
typedef	char			Nchar;
#define Nstrlen(a)		strlen(a)

#else
	// プロジェクトで設定する文字の扱いがワイド文字(2Byte文字)の場合
	// 参考 http://www.t-net.ne.jp/~cyfis/c/string/TCHAR.html

//typedef wchar_t				Nchar;				// プロジェクトで設定する文字の扱いに合わせて下さい。Unicode,マルチバイト
typedef wchar_t				TCHAR;				// Microsoft定義キャラクタ 旧Nchar
#define Tstrlen(a)			wcslen(a)
#define Titoa(v,b,l)		_itow(v,b,l)
#define Tatof(a)			_wtof(a)

#define Tstrchr(a,b)		wcschr(a,b)
#define Tstrrchr(a,b)		wcsrchr(a,b)		// strrchr()
#define Tstrcpy(a,b)		wcscpy(a,b)			// strcpy()
#define Tstrcpy_s(a,b,c)	wcscpy_s(a,b,c)	
#define	Tstrcat(a,b)		wcscat(a,b)			// strcat()
#define Tstrcat_s(a,b,c)	wcscat_s(a,b,c)
#define	Tstrcmp(a,b)		wcscmp(a,b)
#define	Tstrncmp(a,b,c)		wcsncmp(a,b,c)
#define	Tstrncpy(a,b,c)		wcsncpy(a,b,c)
#define	Tstrncpy_s(a,b,c,d)	wcsncpy_s(a,b,c,d)
//#define	N_T(a)				_T(a)				// const char set
#define Tmkdir(a)			_wmkdir(a)


#define Tfopen_s(a,b,c)		_wfopen_s(a,b,c)	// fopen

#endif

#define	Tsprintf(a,b,...)		wsprintf(a,b,__VA_ARGS__)
#define	Tsprintf_s(a,b,c,...)	swprintf_s(a,b,c,__VA_ARGS__)		// int swprintf_s(wchar_t *buffer,size_t sizeOfBuffer,const wchar_t *format,...);


#endif	// GNRL_Nchar_H