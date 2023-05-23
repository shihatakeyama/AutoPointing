// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// WorkBase.cpp
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"
#include "windows.h"

#include "define.h"
#include "extern.h"
#include "WorkBase.h"
#include "WorkTouch.h"
#include "WorkTouchs.h"
#include "WorkWait.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// クラス名のリスト
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
const TCHAR		*WorkBase::m_ProcNames[] = { _T("touch") ,_T("touchs") ,_T("wait") ,nullptr	};


WorkBase::WorkBase(enum E_ProcType ProcType)
 : m_ProcType(ProcType),m_LoopNum(1)
{}
WorkBase::~WorkBase()
{}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 処理オブジェクトを生成
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
WorkBase *WorkBase::newProc(enum E_ProcType ValTypeId)
{
	WorkBase *pbase = nullptr;

	switch (ValTypeId){
		case EPT_touch:
			pbase = new WorkTouch();
			break;
		case EPT_touchs:
			pbase = new WorkTouchs();
			break;
		case EPT_wait:
			pbase = new WorkWait();
			break;
		default:
			ASSERT(0);
			break;
	}

	return pbase;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// **** 信号処理  各処理共通呼び出し処理 ****
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::proc()
{
	return ERC_ng;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// タッチ操作など
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::touchPoint(const TouchPoint *Point)
{
	ASSERT(Point);

	delay(Point->delay);

	// RUN中であればタッチ操作します。
	if(isLife()){
		AM_click(Point->x, Point->y);
	}

	return ERC_ok;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 待ち
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void WorkBase::delay(int32_t Msec)
{
	Sleep(Msec);	// 仮
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// true:通常運転中
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
bool WorkBase::isLife() const
{
	return true;	// ※※※※※
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// **** 単数個/複数個 共通読み書き 各プロセスの内容をXMLオブジェクトへ ****
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::loadXmlNode(const rapidxml::node_t* Child/*-- ,sheet_t *Db*/)
{
	return ERC_ng;
}
int32_t WorkBase::saveXmlNode(rapidxml::node_t *ProcList ,rapidxml::document_t &Doc) const
{
	return ERC_ng;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// XMLからワークリストを読み込み
// List： ワークの数 配列要素数になります。
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::loadProcList(std::vector<WorkBase*>	&List, rapidxml::node_t* ProcessXml, std::vector<std::wstring> &WorkNames)
{
	int32_t ack;
	rapidxml::node_t *node = ProcessXml->first_node();
	if (node == nullptr)	ERC_ng;
	std::wstring name;

	// ワークの数だけループします。
	while (node){
		rapidxml::attribute_t *attr = rapidxml::first_attribute(node, _T("name"), name);
		int32_t idx;
		ack = rapidxml::find_node_index(node, m_ProcNames, idx);	// ノード名が _T("works") である事を確認。 
		if (ack < 0)	break;

		WorkBase *proc = newProc(static_cast<E_ProcType>(idx));
		ack = proc->loadXmlNode(node);
		if (ack < 0)	break;

		List.push_back(proc);
		WorkNames.push_back(name);

		node = rapidxml::next_sibling(node);
	}


	return ERC_ok;
}
int32_t WorkBase::saveProcList(const std::vector<WorkBase*> &List ,rapidxml::document_t &Doc ,rapidxml::node_t* ProcessXml)
{
	return ERC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// メモリ解放
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::clearProcList(std::vector<WorkBase*> &List)
{
	for (auto *e : List)	delete e;
	List.clear();

	return ERC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// タッチ位置と遅延の読み書き
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::loadTouchPoint(const rapidxml::node_t* Node ,TouchPoint *Point)
{
//	int32_t ack;
	rapidxml::  attribute_t	*attr;

	attr = rapidxml::first_attribute(Node ,_T("x") ,Point->x);
	if(attr == nullptr)	throw _T("x");
	attr = rapidxml::first_attribute(Node ,_T("y") ,Point->y);
	if(attr == nullptr)	throw _T("y");
	attr = rapidxml::first_attribute(Node ,_T("delay") ,Point->delay);
	if(attr == nullptr)	throw _T("delay");

	return ERC_ok;
}
int32_t WorkBase::saveTouchPoint(rapidxml::node_t *Parent ,rapidxml::document_t &Doc ,const TouchPoint *Point)
{
	return ERC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 回数読み
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::loadLoop_n(const rapidxml::node_t* Node)
{
	int32_t n;
	rapidxml::attribute_t *attr;

	attr = rapidxml::first_attribute(Node, _T("loop_n"), n);
	if (attr == nullptr){
		return ERC_ng;
	}
	m_LoopNum = n;

	return ERC_ok;
}
