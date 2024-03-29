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
const static uint32_t vresion		= 0x0523C120;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 拒否/許可 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//const static char_t	CheckText[]		=  _T("check");
//const static char_t	*DisaEnaText[]	= {_T("disable") ,_T("enable")};

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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// アロケート
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#if 1
#endif
char_t* allocate_int(document_t &Doc, const int32_t &Val)
{
	std::wstring valueString = std::to_wstring(Val);
	return Doc.allocate_string(valueString.c_str(), valueString.length() + 1);
}
char_t* allocate_uint(document_t &Doc, const uint32_t &Val)
{
	std::wstring valueString = std::to_wstring(Val);
	return Doc.allocate_string(valueString.c_str(), valueString.length() + 1);
}

char_t* allocate_double(document_t &Doc, double Val)
{
	std::wstring valueString = std::to_wstring(Val);
	return Doc.allocate_string(valueString.c_str(), valueString.length() + 1);
}
char_t* allocate_hex(document_t &Doc, uint32_t Val)
{
	const size_t malloc_size = 8 + 1;
	char_t *valoc = Doc.allocate_string(nullptr, malloc_size);
	std::swprintf(valoc, malloc_size, L"%08X", Val);
	return valoc;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ノードに数値を追加
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#if 0
node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const char_t *Str)	// Str はアロケートされないので注意
{
	node_t* node = Doc.allocate_node(rapidxml::node_element ,Name, Str);
	Base->append_node(node);
	return node;
}
node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const string_t &Str)
{
	char_t *aloc = Doc.allocate_string(Str.c_str(), Str.length() + 1);
	node_t* node = Doc.allocate_node(rapidxml::node_element, Name, aloc);
	Base->append_node(node);
	return node;
}
node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const int32_t &Val)
{
	char_t *valoc = allocate_int(Doc ,Val);
	node_t* node = Doc.allocate_node(rapidxml::node_element, Name, valoc);
	Base->append_node(node);
	return node;
}
node_t *append_node(document_t &Doc, node_t* Base, const char_t *Name, const uint32_t &Val)
{
	char_t *valoc = allocate_uint(Doc, Val);
	node_t* node = Doc.allocate_node(rapidxml::node_element, Name, valoc);
	Base->append_node(node);
	return node;
}
node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const double &Val)
{
	char_t *valoc = allocate_double(Doc, Val);
	node_t* node = Doc.allocate_node(rapidxml::node_element, Name, valoc);
	Base->append_node(node);
	return node;
}
node_t *append_node_hex(document_t &Doc ,node_t* Base ,const char_t *Name ,const uint32_t &Val)
{
	char_t *valoc = allocate_hex(Doc, Val);
	node_t* node = Doc.allocate_node(rapidxml::node_element, Name, valoc);
	Base->append_node(node);
	return node;
}

attribute_t *append_attribute(document_t &Doc ,node_t* Base ,const char_t *Name ,const char_t *Str)
{
	attribute_t* attr = Doc.allocate_attribute(Name, Str);
	Base->append_attribute(attr);
	return attr;
}
attribute_t *append_attribute(document_t &Doc ,node_t* Base ,const char_t *Name ,const int32_t &Val)
{
	char_t *valoc = allocate_int(Doc, Val);
	attribute_t* attr = Doc.allocate_attribute(Name, valoc);
	Base->append_attribute(attr);
	return attr;
}
attribute_t *append_attribute(document_t &Doc, node_t* Base, const char_t *Name, const uint32_t &Val)
{
	char_t *valoc = allocate_uint(Doc, Val);
	attribute_t* attr = Doc.allocate_attribute(Name, valoc);
	Base->append_attribute(attr);
	return attr;
}
attribute_t *append_attribute(document_t &Doc ,node_t* Base ,const char_t *Name ,const double &Val)
{
	char_t *valoc = allocate_double(Doc, Val);
	attribute_t* attr = Doc.allocate_attribute(Name, valoc);
	Base->append_attribute(attr);
	return attr;
}

attribute_t *append_attribute_hex(document_t &Doc ,node_t* Base ,const char_t *Name ,const uint32_t &Val)
{
	char_t *valoc = allocate_hex(Doc, Val);
	attribute_t* attr = Doc.allocate_attribute(Name, valoc);
	Base->append_attribute(attr);
	return attr;
}
// ノードの check 属性の値 Disable/Enable を出力します。
attribute_t *append_attribute_check(document_t &Doc ,node_t* Base ,const bool &Val)
{
	attribute_t* attr = Doc.allocate_attribute(CheckText, DisaEnaText[Val]);
	Base->append_attribute(attr);
	return attr;
}
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 先頭/後 ノード取得
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#if 0
rapidxml::node_t* first_node(const node_t* Base ,const char_t *Name ,char_t **Str)
{
	node_t* node = Base->first_node(Name);
	if(node == nullptr){
		*Str = nullptr;
		return nullptr;
	}

	*Str = node->value();

	return node;
}
node_t* first_node(const node_t* Base ,const char_t *Name ,string_t &Str)
{
	node_t* node = Base->first_node(Name);
	if(node == nullptr){
		Str.clear();
		return nullptr;
	}

	Str = node->value();

	return node;
}

node_t* first_node(const node_t* Base ,const char_t *Name ,int32_t &Val)
{
	node_t* node = Base->first_node(Name);
	if(node == nullptr)	return nullptr;

	Val = std::stoi(node->value());

	return node;
}
node_t* first_node(const node_t* Base ,const char_t *Name ,uint32_t &Val)
{
	node_t* node = Base->first_node(Name);
	if(node == nullptr)	return nullptr;

	Val = std::stoul(node->value(), nullptr, 10);

	return node;
}
node_t* first_node(const node_t* Base ,const char_t *Name ,double &Val)
{
	node_t* node = Base->first_node(Name);
	if(node == nullptr)	return nullptr;

	Val = std::stof(node->value());

	return node;
}
node_t* first_node_hex(const node_t* Base ,const char_t *Name ,uint32_t &Val)
{
	node_t* node = Base->first_node(Name);
	if(node == nullptr)	return nullptr;

	Val = std::stoul(node->value(), nullptr, 16);

	return node;
}

// Node(名)はListの何番目にありますか？
int32_t node_name_index(const node_t* Node , const char_t **List, int32_t &Index)
{
	if(Node == nullptr)	return -1;

	auto *node = Node;

	int32_t i = 0;
	char_t *purname = node->name();
//	std::size_t	purname_size = rapidxml::internal::measure(purname);
	std::size_t	purname_size = node->name_size();

	while(*List){
		std::size_t	listname_size = rapidxml::internal::measure(*List);
		if (rapidxml::internal::compare(*List, listname_size, purname, purname_size, false)){
			Index = i;
			return 0;
		}

		List++;
		i++;
	}

	return -1;	
}
int32_t first_node_name_index(const node_t* Node ,const char_t *name ,const char_t **List, int32_t &Index)
{
	const node_t* node = Node->first_node(name);
	return node_name_index(node ,List,Index);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Node(名)がNameと一致しているか？
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t comp_node_name(const node_t* Node, const char_t *Name)
{
	return rapidxml::internal::compare(Node->name(), Node->name_size(), Name, 0, false);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 属性から ノード / 文字 / 数値 を取得
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
attribute_t* first_attribute(const node_t* Base ,const char_t *Name ,char_t **Str)
{
	attribute_t* attr = Base->first_attribute(Name);
	if(attr == nullptr){
		*Str = nullptr;
		return nullptr;
	}

	*Str = attr->value();

	return attr;
}

attribute_t* first_attribute(const node_t* Base, const char_t *Name, int32_t &Val)
{
	attribute_t* attr = Base->first_attribute(Name);
	if(attr == nullptr){
		return nullptr;
	}

	Val = std::stoi(attr->value());

	return attr;
}

attribute_t* first_attribute(const node_t* Base, const char_t *Name, double &Val)
{
	attribute_t* attr = Base->first_attribute(Name);
	if(attr == nullptr){
		return nullptr;
	}

	Val = std::stod(attr->value());

	return attr;
}

attribute_t* first_attribute_hex(const node_t* Base ,const char_t *Name ,uint32_t &Val)
{
	attribute_t* attr = Base->first_attribute(Name);
	if(attr == nullptr){
		return nullptr;
	}

	Val = std::stoul(attr->value(), nullptr, 16);

	return attr;
}

attribute_t* first_attribute(const node_t* Base ,const char_t *Name ,string_t &Str)
{
	attribute_t* attr = Base->first_attribute(Name);
	if(attr == nullptr){
		Str.clear();
		return nullptr;
	}

	Str = attr->value();

	return attr;
}

attribute_t* first_attribute_check(const node_t* Base ,bool &Val)
{
	int32_t index;
	attribute_t* attr = Base->first_attribute(CheckText);
		
	attribute_val_index(attr, DisaEnaText, index);

	Val = (index != 0);

	return attr;
}

// Attr(値)はListの何番目にありますか？
int32_t attribute_val_index(const attribute_t* Attr, const char_t **List, int32_t &Index)
{
	if(Attr == nullptr)	return -1;

	int32_t i=0;
	char_t *purname = Attr->value();
//	std::size_t	purname_size = rapidxml::internal::measure(purname);
	std::size_t	purname_size = Attr->value_size();

	while(*List){
		std::size_t	listname_size = rapidxml::internal::measure(*List);
		if (rapidxml::internal::compare(*List, listname_size, purname, purname_size, false)){
			Index = i;
			return 0;
		}

		List++;
		i++;
	}

	return -1;	
}
int32_t first_attribute_val_index(const node_t* Node ,const char_t *AttrName ,const char_t **List, int32_t &Index)
{
	const attribute_t* attr = Node->first_attribute(AttrName);

	return attribute_val_index(attr ,List, Index);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Attr(名)がNameと一致しているか？
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t comp_attribute_name(const attribute_t* Attr, const char_t *Name)
{
	return rapidxml::internal::compare(Attr->name(), Attr->name_size(), Name, 0, false);
}
#endif

};	// namespace rapidxml

