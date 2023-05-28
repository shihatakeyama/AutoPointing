// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// rapidxml_support.h
//
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


#ifndef RAPIDXML_SUPPORT_H
#define RAPIDXML_SUPPORT_H


#include <stdint.h>

#include "stdafx.h"


namespace rapidxml{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 各種型定義
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
typedef	TCHAR					char_t;		// XMLタグ名、データで使用するキャラクタ型
typedef	xml_document<char_t>	document_t;
typedef	xml_node<char_t>		node_t;
typedef	xml_attribute<char_t>	attribute_t;
//typedef	std::basic_string<char_t> string_t;
typedef std::wstring			string_t;
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
node_t* allocate_node(document_t &Doc ,const char_t *Name ,const char_t *Val=nullptr ,size_t NameSize=0 ,size_t ValSize=0);
attribute_t* allocate_attribute(document_t &Doc ,const char_t *Name ,const char_t *Val ,size_t NameSize=0 ,size_t ValSize=0);
char_t* allocate_string(document_t &Doc, const char_t *Str = nullptr, size_t StrSize = 0);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 値取得
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
char_t *value(const node_t *Node);
char_t *value(const attribute_t *Attr);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ノードに数値を追加
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
node_t *append_node(node_t* Base ,node_t* Child);
node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const char_t *Str=nullptr);
node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const string_t &Str);
node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const int32_t &Val);
node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const uint32_t &Val);
node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const double &Val);
node_t *append_node_hex(document_t &Doc ,node_t* Base ,const char_t *Name ,const uint32_t &Val);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 属性に数値を追加
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
attribute_t *append_attribute(node_t* Base ,attribute_t* Attr);
attribute_t *append_attribute(document_t &Doc ,node_t* Base ,const char_t *Name ,const char_t *Str);
attribute_t *append_attribute(document_t &Doc ,node_t* Base ,const char_t *Name ,const int32_t &Val);
attribute_t *append_attribute(document_t &Doc ,node_t* Base ,const char_t *Name ,const double &Val);
attribute_t *append_attribute_hex(document_t &Doc ,node_t* Base ,const char_t *Name ,const uint32_t &Val);
attribute_t *append_attribute_check(document_t &Doc ,node_t* Base ,const bool &Val);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ノードから ノード / 文字 / 数値 を取得
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
node_t* first_node(const node_t* Base);
node_t* last_node(const node_t* Base);
node_t* first_node(const node_t* Base ,const char_t *Name);
node_t* first_node(const node_t* Base ,const char_t *Name ,char_t **Str);
node_t* first_node(const node_t* Base ,const char_t *Name ,std::wstring &Str);
node_t* first_node(const node_t* Base ,const char_t *Name ,int32_t &Val);
node_t* first_node(const node_t* Base ,const char_t *Name ,uint32_t &Val);
node_t* first_node(const node_t* Base ,const char_t *Name ,double &Val);
node_t* first_node_hex(const node_t* Base ,const char_t *Name ,uint32_t &Val);
node_t* next_sibling(const node_t* Node ,const char_t *name = 0, std::size_t name_size = 0);
int32_t find_node_index(const node_t* Node /*,const char_t *Name */,const char_t **List ,int32_t &Index);
int32_t namecmp(const node_t* Node, const char_t *Name);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 属性から ノード / 文字 / 数値 を取得
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
attribute_t* first_attribute(const node_t* Base);
attribute_t* last_attribute(const node_t* Base);
attribute_t* first_attribute(const node_t* Base ,const char_t *Name);
attribute_t* first_attribute(const node_t* Base ,const char_t *Name ,char_t **Str);
attribute_t* first_attribute(const node_t* Base ,const char_t *Name ,int32_t &Val);
attribute_t* first_attribute(const node_t* Base ,const char_t *Name ,double &Val);
attribute_t* first_attribute_hex(const node_t* Base ,const char_t *Name ,uint32_t &Val);
attribute_t* first_attribute(const node_t* Base, const char_t *Name, string_t &Str);
attribute_t* first_attribute_check(const node_t* Base ,bool &Val);	// ,const char_t *Name
attribute_t* next_sibling(const attribute_t* Attr ,const char_t *name = 0, std::size_t name_size = 0);
attribute_t* find_attribute_index(const node_t* Node ,const char_t *Name ,const char_t **List ,int32_t &Index);

};

#endif // #ifndef RAPIDXML_SUPPORT_H

