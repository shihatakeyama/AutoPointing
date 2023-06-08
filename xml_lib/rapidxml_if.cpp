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

#include "rapidxml_print.hpp"	// std::ofstream << rapidxml::xml_document<>
#include "rapidxml_if.h"


namespace rapidxml{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 本ライブラリのバージョン
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
const static uint32_t vresion		= 0x05236080;

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
	DocBuf.resize(static_cast<uint32_t>(size + 1));
    ifs.read(&DocBuf[0] , size);
	DocBuf[static_cast<uint32_t>(ifs.gcount())] = '\0';
	Doc.parse<parse_validate_closing_tags>(&DocBuf[0]);	// parse_validate_closing_tags

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
char_t* allocate_string(document_t &Doc, const char_t *Str, size_t StrSize)	// StrSizeはnull文字は含まないので注意
{
	return Doc.allocate_string(Str ,StrSize);
}
char_t* allocate_int(document_t &Doc, const int32_t &Val)
{
	std::wstring valueString = std::to_wstring(Val);
	return allocate_string(Doc, valueString.c_str(), valueString.length() +1);
}
char_t* allocate_uint(document_t &Doc, const uint32_t &Val)
{
	std::wstring valueString = std::to_wstring(Val);
	return allocate_string(Doc, valueString.c_str(), valueString.length() +1);
}
char_t* allocate_double(document_t &Doc, double Val)
{
	std::wstring valueString = std::to_wstring(Val);
	return allocate_string(Doc ,valueString.c_str(), valueString.length() +1);
}
char_t* allocate_hex(document_t &Doc, uint32_t Val)
{
	const size_t malloc_size = 8 + 1;
	char_t *valoc = allocate_string(Doc, nullptr, malloc_size);
	std::swprintf(valoc, malloc_size, L"%08X", Val);
	return valoc;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 名前 設定/取得
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void name(node_t *Node ,const char_t *Name)
{
	Node->name(Name);
}
void name(attribute_t *Attr ,const char_t *Name)
{
	Attr->name(Name);
}
char_t * name(const node_t *Node)
{
	return Node->name();
}
char_t * name(const attribute_t *Attr)
{
	return Attr->name();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 値 設定/取得
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void value(node_t *Node, const char_t *Value)
{
	Node->value(Value);
}
void value(attribute_t *Attr, const char_t *Value)
{
	Attr->value(Value);
}
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
node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const char_t *Str)	// Str はアロケートされないので注意
{
	node_t* node = allocate_node(Doc,Name, Str);
	append_node(Base ,node);
	return node;
}
node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const string_t &Str)
{
	char_t *aloc = allocate_string(Doc ,Str.c_str(), Str.length()+1);
	node_t* node = allocate_node(Doc ,Name, aloc);
	append_node(Base ,node);
	return node;
}
node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const int32_t &Val)
{
//	std::wstring valueString = std::to_wstring(Val);
//	char_t *valoc = allocate_string(Doc ,valueString.c_str(), valueString.length()+1);
	char_t *valoc = allocate_int(Doc ,Val);
	node_t* node = allocate_node(Doc ,Name, valoc);
	append_node(Base ,node);
	return node;
}
node_t *append_node(document_t &Doc, node_t* Base, const char_t *Name, const uint32_t &Val)
{
	char_t *valoc = allocate_uint(Doc, Val);
	node_t* node = allocate_node(Doc ,Name, valoc);
	append_node(Base ,node);
	return node;
}


node_t *append_node(document_t &Doc ,node_t* Base ,const char_t *Name ,const double &Val)
{
	char_t *valoc = allocate_double(Doc, Val);
	node_t* node = allocate_node(Doc ,Name, valoc);
	append_node(Base ,node);
	return node;
}
node_t *append_node_hex(document_t &Doc ,node_t* Base ,const char_t *Name ,const uint32_t &Val)
{
	char_t *valoc = allocate_hex(Doc, Val);
	node_t* node = allocate_node(Doc ,Name, valoc);
	append_node(Base ,node);
	return node;
}

node_t *insert_node(node_t* Base, node_t* Where, node_t* Child)
{
	Base->insert_node(Where, Child);
	return Child;
}

attribute_t *append_attribute(node_t* Base ,attribute_t* Attr)
{
	Base->append_attribute(Attr);
	return Attr;
}

attribute_t *append_attribute(document_t &Doc ,node_t* Base ,const char_t *Name ,const char_t *Str)
{
	attribute_t* attr = allocate_attribute(Doc ,Name ,Str);
	append_attribute(Base ,attr);
	return attr;
}
attribute_t *append_attribute(document_t &Doc ,node_t* Base ,const char_t *Name ,const int32_t &Val)
{
	char_t *valoc = allocate_int(Doc, Val);
	attribute_t* attr = allocate_attribute(Doc ,Name, valoc);
	append_attribute(Base ,attr);
	return attr;
}
attribute_t *append_attribute(document_t &Doc, node_t* Base, const char_t *Name, const uint32_t &Val)
{
	char_t *valoc = allocate_uint(Doc, Val);
	attribute_t* attr = allocate_attribute(Doc, Name, valoc);
	append_attribute(Base, attr);
	return attr;
}
attribute_t *append_attribute(document_t &Doc ,node_t* Base ,const char_t *Name ,const double &Val)
{
	char_t *valoc = allocate_double(Doc, Val);
	attribute_t* attr = allocate_attribute(Doc ,Name, valoc);
	append_attribute(Base ,attr);
	return attr;
}
attribute_t *append_attribute_hex(document_t &Doc ,node_t* Base ,const char_t *Name ,const uint32_t &Val)
{
	char_t *valoc = allocate_hex(Doc, Val);
	attribute_t* attr = allocate_attribute(Doc ,Name, valoc);
	append_attribute(Base ,attr);
	return attr;
}
// ノードの check 属性の値 Disable/Enable を出力します。
attribute_t *append_attribute_check(document_t &Doc ,node_t* Base ,const bool &Val)
{
	attribute_t* attr = allocate_attribute(Doc, CheckText, DisaEnaText[Val]);
	append_attribute(Base ,attr);
	return attr;
}

attribute_t *insert_attribute(node_t* Base, attribute_t* Where, attribute_t* Attr)
{
	Base->insert_attribute(Where, Attr);
	return Attr;
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

	*Str = value(node);

	return node;
}
node_t* first_node(const node_t* Base ,const char_t *Name ,std::wstring &Str)
{
	node_t* node = first_node(Base ,Name);
	if(node == nullptr){
		Str.clear();
		return nullptr;
	}

	Str = value(node);

	return node;
}

node_t* first_node(const node_t* Base ,const char_t *Name ,int32_t &Val)
{
	node_t* node = first_node(Base ,Name);
	if(node == nullptr)	return nullptr;

	Val = std::stoi(value(node));

	return node;
}
node_t* first_node(const node_t* Base ,const char_t *Name ,uint32_t &Val)
{
	node_t* node = first_node(Base ,Name);
	if(node == nullptr)	return nullptr;

	Val = std::stoul(value(node), nullptr, 10);

	return node;
}

node_t* first_node(const node_t* Base ,const char_t *Name ,double &Val)
{
	node_t* node = first_node(Base ,Name);
	if(node == nullptr)	return nullptr;

	Val = std::stof(value(node));

	return node;
}
node_t* first_node_hex(const node_t* Base ,const char_t *Name ,uint32_t &Val)
{
	node_t* node = first_node(Base ,Name);
	if(node == nullptr)	return nullptr;

	Val = std::stoul(value(node), nullptr, 16);

	return node;
}


node_t* next_sibling(const node_t* Node ,const char_t *name, std::size_t name_size)
{
	return Node->next_sibling(name ,name_size ,true);
}
// Node(名)はListの何番目にありますか？
int32_t node_name_index(const node_t* Node , const char_t **List, int32_t &Index)
{
	if(Node == nullptr)	return -1;

	auto *node = Node;

	int32_t i = 0;
	char_t *purname = name(node);
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
	const node_t* node = first_node(Node ,name);
	return node_name_index(node ,List,Index);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Node(名)がNameと一致しているか？
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t comp_node_name(const node_t* Node, const char_t *Name)
{
	return rapidxml::internal::compare(name(Node), Node->name_size(), Name, 0, false);
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
	attribute_t* attr = first_attribute(Base ,Name);
	if(attr == nullptr){
		*Str = nullptr;
		return nullptr;
	}

	*Str = value(attr);

	return attr;
}

attribute_t* first_attribute(const node_t* Base, const char_t *Name, int32_t &Val)
{
	attribute_t* attr = first_attribute(Base ,Name);
	if(attr == nullptr){
		return nullptr;
	}

	Val = std::stoi(value(attr));

	return attr;
}

attribute_t* first_attribute(const node_t* Base, const char_t *Name, double &Val)
{
	attribute_t* attr = first_attribute(Base ,Name);
	if(attr == nullptr){
		return nullptr;
	}

	Val = std::stod(value(attr));

	return attr;
}

attribute_t* first_attribute_hex(const node_t* Base ,const char_t *Name ,uint32_t &Val)
{
	attribute_t* attr = first_attribute(Base ,Name);
	if(attr == nullptr){
		return nullptr;
	}

	Val = std::stoul(value(attr), nullptr, 16);

	return attr;
}

attribute_t* first_attribute(const node_t* Base ,const char_t *Name ,string_t &Str)
{
	attribute_t* attr = first_attribute(Base ,Name);
	if(attr == nullptr){
		Str.clear();
		return nullptr;
	}

	Str = value(attr);

	return attr;
}

attribute_t* first_attribute_check(const node_t* Base ,bool &Val)
{
	int32_t index;
	attribute_t* attr = first_attribute(Base ,CheckText);
		
	attribute_val_index(attr, DisaEnaText, index);

	Val = (index != 0);

	return attr;
}

attribute_t* next_attribute(const attribute_t* Attr, const char_t *name, std::size_t name_size)
{
	return Attr->next_attribute(name ,name_size ,true);
}
// Attr(値)はListの何番目にありますか？
int32_t attribute_val_index(const attribute_t* Attr, const char_t **List, int32_t &Index)
{
	if(Attr == nullptr)	return -1;

	int32_t i=0;
	char_t *purname = value(Attr);
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
	const attribute_t* attr = first_attribute(Node ,AttrName);

	return attribute_val_index(attr ,List, Index);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Attr(名)がNameと一致しているか？
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t comp_attribute_name(const attribute_t* Attr, const char_t *Name)
{
	return rapidxml::internal::compare(name(Attr), Attr->name_size(), Name, 0, false);
}

};	// namespace rapidxml

