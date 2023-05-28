// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// GnrlString.cpp
//
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"

#include "GnrlString.h"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// PurposeName�ƈ�v���镶����� NameList[]�̒�����T���܂��B
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
// PurposeName�ƈ�v���镶����� NameList[]�̒�����T���܂��B
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



// �^�ϊ�
// std::string std::to_string(int32_t)	// int��std::string�ϊ�

// int32_t _ttoi(CString);				// CString��int�ϊ�

#if 0
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �}���`�o�C�g�ϊ��ƃA���P�[�g���܂��B
// �߂�l  ���������null�ȊO
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

