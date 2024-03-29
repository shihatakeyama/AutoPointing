// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  rapidxml_if.cpp
//
// �L�����N�^�̓��C�h�����ɑΉ����Ă��܂��B
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"


#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

//#include "rapidxml.h"
//--#include "rapidxml_define.h"
#include "rapidxml.h"
#include "rapidxml_print.hpp"	// std::ofstream << rapidxml::xml_document<>
#include "rapidxml_if.h"



namespace rapidxml{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �{���C�u�����̃o�[�W����
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
const static uint32_t vresion		= 0x05243290;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �t�@�C�� load / save
// �� DocBuf�́Aload�����e�L�X�g�f�[�^���i�[����Adocument_t��node_t�ŎQ�Ƃ���܂��B
//    DocBuf��p���������document_t�܂���node_t���Q�Ƃ��Ȃ��ŉ������B
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t load_document(document_t &Doc ,const TCHAR *Path ,string_t &DocBuf)
{
	ifstream_t ifs;

	ifs.imbue(std::locale("Japanese", LC_CTYPE));	// �S�p����UTF8�o�͂���ꍇ
	ifs.open(Path , std::ios::in | std::ios::binary );		// �o�C�i�����[�h�łɂ��Ȃ��ƁA�ǂݍ��񂾃t�@�C���T�C�Y������Ȃ��Ȃ�B
    if (ifs.fail()){
        std::cerr << "���s" << std::endl;
		return -1;
    }
    auto begin = ifs.tellg();
    ifs.seekg(0, ifs.end);

    auto end = ifs.tellg();		// �ꉞ�͈̓`�F�b�N���ׂ������ǁc�c
    auto size = (end - begin);

    ifs.clear();				// ������clear����EOF�t���O������
    ifs.seekg(0, ifs.beg);
	DocBuf.resize(static_cast<uint32_t>(size + 1));
    ifs.read(&DocBuf[0] , size);
	DocBuf[static_cast<uint32_t>(ifs.gcount())] = '\0';
	Doc.parse(&DocBuf[0], parse_validate_closing_tags);	// parse_validate_closing_tags

	return 0;
}
int32_t save_document(document_t &Doc ,const TCHAR *Path)
{
	ofstream_t outFile;

	outFile.imbue(std::locale("Japanese", LC_CTYPE));	// �S�p����UTF8�o�͂���ꍇ
	outFile.open(Path , std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);

	if(outFile.fail()){
		std::string str = "file not open ";
		throw std::runtime_error(str);
	}

	outFile << Doc;

	return 0;
}

};	// namespace rapidxml

