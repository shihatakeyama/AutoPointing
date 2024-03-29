// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  rapidxml_if.cpp
//
// キャラクタはワイド文字に対応しています。
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
// 本ライブラリのバージョン
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
const static uint32_t vresion		= 0x05243290;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ファイル load / save
// ※ DocBufは、loadしたテキストデータが格納され、document_tとnode_tで参照されます。
//    DocBufを廃棄した後にdocument_tまたはnode_tを参照しないで下さい。
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t load_document(document_t &Doc ,const TCHAR *Path ,string_t &DocBuf)
{
	ifstream_t ifs;

	ifs.imbue(std::locale("Japanese", LC_CTYPE));	// 全角文字UTF8出力する場合
	ifs.open(Path , std::ios::in | std::ios::binary );		// バイナリモードでにしないと、読み込んだファイルサイズが合わなくなる。
    if (ifs.fail()){
        std::cerr << "失敗" << std::endl;
		return -1;
    }
    auto begin = ifs.tellg();
    ifs.seekg(0, ifs.end);

    auto end = ifs.tellg();		// 一応範囲チェックすべきだけど……
    auto size = (end - begin);

    ifs.clear();				// ここでclearしてEOFフラグを消す
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

	outFile.imbue(std::locale("Japanese", LC_CTYPE));	// 全角文字UTF8出力する場合
	outFile.open(Path , std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);

	if(outFile.fail()){
		std::string str = "file not open ";
		throw std::runtime_error(str);
	}

	outFile << Doc;

	return 0;
}

};	// namespace rapidxml

