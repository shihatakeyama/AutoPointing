// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  rapidxml_suport.cpp
//
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"

#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

#include "rapidxml_print.hpp"	// std::ofstream << rapidxml::xml_document<>
#include "rapidxml_support.h"


namespace rapidxml{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 本ライブラリのバージョン
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
const static uint32_t vresion		= 0x05235220;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 拒否/許可 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
const static char_t	CheckText[]		=  _T("check");
const static char_t	*DisaEnaText[]	= {_T("disable") ,_T("enable")};

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
	DocBuf.resize(size + 1);
    ifs.read(&DocBuf[0] , size);
	DocBuf[ifs.gcount()] = '\0';
	Doc.parse<0>(&DocBuf[0]);

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
node_t* allocate_node(document_t &Doc ,const char_t *Name ,const char_t *Val ,size_t NameSize ,size_t ValSize)
{
	return Doc.allocate_node(rapidxml::node_element ,Name, Val ,NameSize ,ValSize);
}
attribute_t* allocate_attribute(document_t &Doc ,const char_t *Name ,const char_t *Val ,size_t NameSize ,size_t ValSize)
{
	return Doc.allocate_attribute(Name, Val ,NameSize ,ValSize);
}
char_t* allocate_string(document_t &Doc, const char_t *Str, size_t StrSize)	// null文字は含まないでよいか？？？
{
	return Doc.allocate_string(Str ,StrSize);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 値取得
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
char_t *value(const node_t *Node)
{
	return Node->value();
}
char_t *value(const attribute_t *Attr)
{
	return Attr->value();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ノードに数値を追加
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
node_t *append_node(node_t* Base ,node_t* Child)
{
	Base->append_node(Child);
	return Child;
}
node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const char_t *Str)
{
	node_t* node = Doc.allocate_node(rapidxml::node_element ,Name, Str);
	Base->append_node(node);
	return node;
}
node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const string_t &Str)
{
	char_t *aloc = Doc.allocate_string(Str.c_str(), Str.length()+1);
	node_t* node = Doc.allocate_node(rapidxml::node_element ,Name, aloc);	// _T("123")
	Base->append_node(node);
	return node;
}
node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const int32_t &Val)
{
	std::wstring valueString = std::to_wstring(Val);

	char_t *valoc = Doc.allocate_string(valueString.c_str(), valueString.length()+1);
	node_t* node = Doc.allocate_node(rapidxml::node_element ,Name, valoc);
	Base->append_node(node);
	return node;
}
node_t *append_node(document_t &Doc, node_t* Base, const char_t *Name, const uint32_t &Val)
{
	std::wstring valueString = std::to_wstring(Val);

	char_t *valoc = Doc.allocate_string(valueString.c_str(), valueString.length()+1);
	node_t* node = Doc.allocate_node(rapidxml::node_element ,Name, valoc);
	Base->append_node(node);
	return node;
}


node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const double &Val)
{
	std::wstring valueString = std::to_wstring(Val);
	char_t *valoc = Doc.allocate_string(valueString.c_str(), valueString.length()+1);
	node_t* node = Doc.allocate_node(rapidxml::node_element ,Name, valoc);
	Base->append_node(node);
	return node;
}
node_t *append_node_hex(document_t &Doc ,node_t* Base ,const char_t *Name ,const uint32_t &Val)
{
	const size_t malloc_size = 8+1;
	char_t *valoc = Doc.allocate_string(nullptr, malloc_size);
	std::swprintf(valoc, malloc_size, L"%08X", Val);

	node_t* node = Doc.allocate_node(rapidxml::node_element ,Name, valoc);
	Base->append_node(node);
	return node;
}

attribute_t *append_attribute(node_t* Base ,attribute_t* Attr)
{
	Base->append_attribute(Attr);
	return Attr;
}
attribute_t *append_attribute(document_t &Doc ,node_t* Base ,const char_t *Name ,const char_t *Str)
{
	attribute_t* attr = Doc.allocate_attribute(Name ,Str);
	Base->append_attribute(attr);
	return attr;
}
attribute_t *append_attribute(document_t &Doc ,node_t* Base ,const char_t *Name ,const int32_t &Val)
{
	std::wstring valueString = std::to_wstring(Val);
	char_t *valoc = Doc.allocate_string(valueString.c_str(), valueString.length()+1);
	attribute_t* attr = Doc.allocate_attribute(Name, valoc);
	Base->append_attribute(attr);
	return attr;
}
attribute_t *append_attribute(document_t &Doc ,node_t* Base ,const char_t *Name ,const double &Val)
{
	std::wstring valueString = std::to_wstring(Val);
	char_t *valoc = Doc.allocate_string(valueString.c_str(), valueString.length()+1);
	attribute_t* attr = Doc.allocate_attribute(Name, valoc);
	Base->append_attribute(attr);
	return attr;
}
attribute_t *append_attribute_hex(document_t &Doc ,node_t* Base ,const char_t *Name ,const uint32_t &Val)
{
	const size_t malloc_size = 8+1;
	char_t *valoc = Doc.allocate_string(nullptr, malloc_size);
	std::swprintf(valoc, malloc_size, _T("%08X"), Val);
	attribute_t* attr = Doc.allocate_attribute(Name, valoc);
	Base->append_attribute(attr);
	return attr;
}
// ノードの Disable/Enable を出力します。
attribute_t *append_attribute_check(document_t &Doc ,node_t* Base ,const bool &Val)
{
	const size_t malloc_size = 14+1;
	char_t *valoc = Doc.allocate_string(DisaEnaText[Val], malloc_size );
	attribute_t* attr = Doc.allocate_attribute(CheckText, valoc);
	Base->append_attribute(attr);
	return attr;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 先頭/後 ノード取得
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
rapidxml::node_t* first_node(const node_t* Base)
{
	return Base->first_node();
}
rapidxml::node_t* last_node(const node_t* Base)
{
	return Base->last_node();
}

rapidxml::node_t* first_node(const node_t* Base ,const char_t *Name)
{
	return Base->first_node(Name);
}
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
node_t* first_node(const node_t* Base ,const char_t *Name ,std::wstring &Str)
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

//--	Val = std::stoi(node->value());
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

//	std::swscanf(node->value() ,_T("%08X") ,&Val);
	Val = std::stoul(node->value(), nullptr, 16);

	return node;
}


node_t* next_sibling(const node_t* Node ,const char_t *name, std::size_t name_size)
{
	return Node->next_sibling(name ,name_size ,true);
}

int32_t find_node_index(const node_t* Node /*,const char_t *Name */,const char_t **List ,int32_t &Index)
{
//	node_t* node = Node->first_node(Name);
//	if(node == nullptr)	return nullptr;
	auto *node = Node;

	Index = 0;
	std::size_t	purname_size = rapidxml::internal::measure(node->value());

	while(*List){
			std::size_t	listname_size = rapidxml::internal::measure(*List);
		if(rapidxml::internal::compare(*List, listname_size, node->value(), purname_size, false)){
			return 0;
		}

		List++;
		Index++;
	}

	return -1;	
}
int32_t namecmp(const node_t* Node, const char_t *Name)
{
	return Tstrcmp(Node->name(), Name);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 属性から ノード / 文字 / 数値 を取得
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
attribute_t* first_attribute(const node_t* Base)
{
	return Base->first_attribute();
}
attribute_t* last_attribute(const node_t* Base)
{
	return Base->last_attribute();
}

attribute_t* first_attribute(const node_t* Base ,const char_t *Name)
{
	return Base->first_attribute(Name);
}
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
		Val = 0;
		return nullptr;
	}
//	std::swscanf(attr->value() ,_T("%d") ,&Val);
	Val = std::stoi(attr->value());

	return attr;
}

attribute_t* first_attribute(const node_t* Base, const char_t *Name, double &Val)
{
	attribute_t* attr = Base->first_attribute(Name);
	if(attr == nullptr){
		Val = 0;
		return nullptr;
	}
//	std::swscanf(attr->value() ,_T("%f") ,&Val);
	Val = std::stod(attr->value());

	return attr;
}

attribute_t* first_attribute_hex(const node_t* Base ,const char_t *Name ,uint32_t &Val)
{
	attribute_t* attr = Base->first_attribute(Name);
	if(attr == nullptr){
		Val = 0;
		return nullptr;
	}
//	std::swscanf(attr->value() ,_T("%08X") ,&Val);
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
	attribute_t* attr = find_attribute_index(Base ,CheckText ,DisaEnaText ,index);

	Val = (index != 0);

	return attr;
}

attribute_t* next_sibling(const attribute_t* Attr ,const char_t *name, std::size_t name_size)
{
	return Attr->next_attribute(name ,name_size ,true);
}

attribute_t* find_attribute_index(const node_t* Node ,const char_t *Name ,const char_t **List ,int32_t &Index)
{
	attribute_t* attr = Node->first_attribute(Name);
	if(attr == nullptr)	return nullptr;

	Index = 0;
	std::size_t	purname_size = rapidxml::internal::measure(attr->value());

	while(*List){
			std::size_t	listname_size = rapidxml::internal::measure(*List);
		if(rapidxml::internal::compare(*List, listname_size, attr->value(), purname_size, false)){
			return attr;
		}

		List++;
		Index++;
	}

	return nullptr;	
}


};

