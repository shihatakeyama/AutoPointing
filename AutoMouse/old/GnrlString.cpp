// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// GnrlString.cpp
//
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"

#include "GnrlString.h"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// PurposeNameと一致する文字列を NameList[]の中から探します。
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t findStringListId(const char *PurposeName ,const char *NameList[])
{
	int32_t itr = 0;

	while(*NameList){
		if(strcmp(PurposeName ,*NameList)==0){
			return itr;
		}

		NameList++;
		itr++;
	}

	return -1;
}

int32_t findStringListId(const CString &PurposeName ,const LPCTSTR NameList[])
{
	int32_t itr = 0;

	while(*NameList){
		CString tmp = *NameList;
		if(PurposeName.CompareNoCase(tmp)==0){
			return itr;
		}

		NameList++;
		itr++;
	}

	return -1;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// PurposeNameと一致する文字列を NameList[]の中から探します。
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t findStringListId(const char *PurposeName ,const char *NameList[] ,bool case_sensitive)
{
	int32_t itr = 0;
	std::size_t	purname_size = rapidxml::internal::measure(PurposeName);
	std::size_t	listname_size;

	while(*NameList){
		listname_size	= rapidxml::internal::measure(*NameList);
		if(rapidxml::internal::compare(*NameList, listname_size, PurposeName, purname_size, case_sensitive)){
			return itr;
		}

		NameList++;
		itr++;
	}

	return -1;
}



// 型変換
// std::string std::to_string(int32_t)	// int→std::string変換

// int32_t _ttoi(CString);				// CString→int変換

#if 0
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// マルチバイト変換とアロケートします。
// 戻り値  成功するとnull以外
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
char *toToMultiByte(rapidxml::xml_document<> &Doc, CString &Str)
{
	int32_t len;
	const size_t textSize = 256;
	TCHAR tchrText[textSize];
	char *ret;

	char pszText[textSize];
	len = WideCharToMultiByte(CP_ACP, 0, Str.GetBuffer(0), -1, pszText, textSize, NULL, NULL);
	if (len == 0) { return nullptr; }

	ret = Doc.allocate_string(pszText, len);

	return ret;
}
char *toToMultiByte(rapidxml::xml_document<> &Doc, std::string &Str)
{
	char *ret = Doc.allocate_string(Str.c_str(), Str.length());

	return ret;
}
CString toWideChar()
{
    char *pSrc = "MultiByteToWideChar test";
    const unsigned int dataSize = 24;
    wchar_t wlocal[dataSize+1] = {0x00};

    int ret = MultiByteToWideChar(
        CP_ACP,
        MB_PRECOMPOSED,
        pSrc,
        dataSize,
        wlocal,
        dataSize+1);

	return CString(pSrc);
}
#endif

