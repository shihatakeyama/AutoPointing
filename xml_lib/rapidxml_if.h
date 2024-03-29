// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// rapidxml_if.h
//
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


#ifndef RAPIDXML_IF_H
#define RAPIDXML_IF_H


#include <stdint.h>

#include "stdafx.h"


namespace rapidxml{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 各種型定義
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
typedef	TCHAR					char_t;		// XMLタグ名、データで使用するキャラクタ型
typedef	TCHAR					Ch;			// XMLタグ名、データで使用するキャラクタ型
typedef	xml_document<char_t>	document_t;
typedef	xml_node<char_t>		node_t;
typedef	xml_attribute<char_t>	attribute_t;
//typedef	std::basic_string<char_t> string_t;
//typedef std::wstring			string_t;
typedef std::wofstream			ofstream_t;
typedef std::wifstream			ifstream_t;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 本ライブラリのバージョン
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
extern const uint32_t			vresion;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ファイル load / save
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t load_document(document_t &Doc ,const TCHAR *Path ,string_t &DocBuf);
int32_t save_document(document_t &Doc ,const TCHAR *Path);

};

#endif // #ifndef RAPIDXML_IF_H

