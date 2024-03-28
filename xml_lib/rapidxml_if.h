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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// アロケート
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//--char_t* allocate_int(document_t &Doc, const int32_t &Val);
//--char_t* allocate_uint(document_t &Doc, const uint32_t &Val);
//--char_t* allocate_double(document_t &Doc, double Val);
//--char_t* allocate_hex(document_t &Doc, uint32_t Val);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ノードに数値を追加
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//--node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const char_t *Str=nullptr);
//--node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const string_t &Str);
//--node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const int32_t &Val);
//--node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const uint32_t &Val);
//--node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const double &Val);
//--node_t *append_node_hex(document_t &Doc ,node_t* Base ,const char_t *Name ,const uint32_t &Val);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 属性に数値を追加
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//--attribute_t *append_attribute(document_t &Doc ,node_t* Base ,const char_t *Name ,const char_t *Str);
//--attribute_t *append_attribute(document_t &Doc ,node_t* Base ,const char_t *Name ,const int32_t &Val);
//--attribute_t *append_attribute(document_t &Doc, node_t* Base, const char_t *Name, const uint32_t &Val);
//--attribute_t *append_attribute(document_t &Doc ,node_t* Base ,const char_t *Name ,const double &Val);
//--attribute_t *append_attribute_hex(document_t &Doc ,node_t* Base ,const char_t *Name ,const uint32_t &Val);
//--attribute_t *append_attribute_check(document_t &Doc ,node_t* Base ,const bool &Val);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ノードから ノード / 文字 / 数値 を取得
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//node_t* first_node(const node_t* Base ,const char_t *Name ,char_t **Str);
//node_t* first_node(const node_t* Base, const char_t *Name, string_t &Str);
//node_t* first_node(const node_t* Base ,const char_t *Name ,int32_t &Val);
//node_t* first_node(const node_t* Base ,const char_t *Name ,uint32_t &Val);
//node_t* first_node(const node_t* Base ,const char_t *Name ,double &Val);
//node_t* first_node_hex(const node_t* Base ,const char_t *Name ,uint32_t &Val);
//int32_t node_name_index(const node_t* Node ,const char_t **List, int32_t &Index);		// find_node_name_index
//int32_t first_node_name_index(const node_t* Node ,const char_t *name ,const char_t **List, int32_t &Index);
//int32_t comp_node_name(const node_t* Node, const char_t *Name);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 属性から ノード / 文字 / 数値 を取得
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//--attribute_t* first_attribute(const node_t* Base ,const char_t *Name ,char_t **Str);
//--attribute_t* first_attribute(const node_t* Base ,const char_t *Name ,int32_t &Val);
//--attribute_t* first_attribute(const node_t* Base ,const char_t *Name ,double &Val);
//--attribute_t* first_attribute_hex(const node_t* Base ,const char_t *Name ,uint32_t &Val);
//attribute_t* first_attribute(const node_t* Base, const char_t *Name, string_t &Str);
//attribute_t* first_attribute_check(const node_t* Base ,bool &Val);
//--int32_t attribute_val_index(const attribute_t* Attr, const char_t **List, int32_t &Index);		// find_attribute_val_index
//--int32_t first_attribute_val_index(const node_t* node, const char_t *AttrName ,const char_t **List, int32_t &Index);
//--int32_t comp_attribute_name(const attribute_t* Attr, const char_t *Name);

};

#endif // #ifndef RAPIDXML_IF_H

