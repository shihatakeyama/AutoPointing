// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  GnrlString.h
//
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


#ifndef GNRL_NCHAR_H
#define GNRL_NCHAR_H



//  �v���W�F�N�g�Őݒ肷�镶���̈������z������B
#if 0
	
	// �}���`�o�C�g
typedef	char			Nchar;
#define Nstrlen(a)		strlen(a)

#else
	// �v���W�F�N�g�Őݒ肷�镶���̈��������C�h����(2Byte����)�̏ꍇ
	// �Q�l http://www.t-net.ne.jp/~cyfis/c/string/TCHAR.html

//typedef wchar_t				Nchar;				// �v���W�F�N�g�Őݒ肷�镶���̈����ɍ��킹�ĉ������BUnicode,�}���`�o�C�g
typedef wchar_t				TCHAR;				// Microsoft��`�L�����N�^ ��Nchar
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