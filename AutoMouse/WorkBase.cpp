// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// WorkBase.cpp
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"
#include "windows.h"

#include "define.h"
#include "extern.h"
#include "global.h"
#include "sub.h"
#include "GnrlThread.h"
#include "WorkTouch.h"
#include "WorkTouchs.h"
#include "WorkWait.h"

#include "Resource.h"
#include "AutoPointingDlg.h"

#include "WorkBase.h"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// クラス名のリスト
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
const TCHAR		*WorkBase::m_ProcNames[] = { _T("touch"), _T("touchs"), _T("wait") ,nullptr };


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
			break;
	}

	return pbase;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 各処理共通呼び出し処理
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::proc()
{
	int32_t ack=ERC_ok;
	int32_t i;

	for(i=0;m_LoopNum==0 ? true : i<m_LoopNum;i++){
		if (!isLife())		return ERC_ok;

		setGuiComment(m_Comment);
		setGuiCnt(i);

		ack = procOne();
		if (ack < ERC_ok)	return ack;
	}

	return ack;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::procOne()
{
	return ERC_invalid_call;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// タッチ操作など
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::touchPointAndDelay(const TouchPoint &Point)
{
	int32_t ack;

	if(!isLife())	ERC_ok;

	// RUN中であればタッチ操作します。
	ack = AP_pointingDesiredWindow(CPoint(Point.x,Point.y));
	if(ack < ERC_ok)	return ack;

	delay(Point.delay);

	return ERC_ok;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 待ち
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void WorkBase::delay(int32_t Msec)
{	
	APD_Sleep(Msec);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// true:通常運転中
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
bool WorkBase::isLife()
{
	return (g_Operation == EOS_operation);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// **** 単数個/複数個 共通読み書き 各プロセスの内容をXMLオブジェクトへ ****
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::loadXmlNode(const rapidxml::node_t* Node)
{
	return ERC_ng;
}
int32_t WorkBase::saveXmlNode(rapidxml::document_t &Doc ,rapidxml::node_t *&ProcList) const
{
	return ERC_ng;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// XMLからワークリストを読み込み
// List： ワークの数 配列要素数になります。
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::loadWorkList(std::vector<WorkBase*>	&List, rapidxml::node_t* ProcessXml, std::vector<std::wstring> &WorkNames)
{
	int32_t ack;
	rapidxml::node_t *node = ProcessXml->first_node();
	if (node == nullptr)	ERC_ng;
	std::wstring name;

	// ワークの数だけループします。
	while (node){
		rapidxml::attribute_t *attr = node->first_attribute(_T("name"), name);

		int32_t idx;
#if 0
		ack = rapidxml::find_node_name_index(node, m_ProcNames, idx);	// ノード名が _T("works") である事を確認。 
		if (ack < 0)	break;
#else
		ack = node->comp_node_name(_T("work"));
		if (ack < 0)	throw std::wstring(_T("not work"));

		idx = EPT_touchs;
#endif
		WorkBase *proc = newProc(static_cast<E_ProcType>(idx));
		ack = proc->loadXmlNode(node);
		if (ack < 0)	break;

		List.push_back(proc);
		WorkNames.push_back(name);

		node = node->next_sibling();
	}


	return ERC_ok;
}
int32_t WorkBase::saveWorkList(const std::vector<WorkBase*> &List, rapidxml::document_t &Doc, rapidxml::node_t* ProcessXml,const std::vector<std::wstring> &WorkNames)
{
	ASSERT(List.size() == WorkNames.size());

	rapidxml::node_t *node;

	for (uint32_t i = 0; i<List.size(); i++){
		List[i]->saveXmlNode(Doc ,node);
		node->name(_T("work"));
		rapidxml::attribute_t *attr = Doc.allocate_attribute(_T("name"), WorkNames[i].c_str());
		node->insert_attribute(node->first_attribute(), attr);	// ノードの初めの方にアトリビュートを追加
		ProcessXml->append_node(node);
	}

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
	rapidxml::  attribute_t	*attr;

	attr = Node->first_attribute(_T("x"), Point->x);
	if(attr == nullptr)	throw _T("x");
	attr = Node->first_attribute(_T("y"), Point->y);
	if(attr == nullptr)	throw _T("y");
	attr = Node->first_attribute(_T("delay"), Point->delay);
	if(attr == nullptr)	throw _T("delay");

	return ERC_ok;
}
int32_t WorkBase::saveTouchPoint(rapidxml::document_t &Doc ,rapidxml::node_t *Node, const TouchPoint *Point)
{
	Node->append_attribute(Doc, _T("x"), Point->x);
	Node->append_attribute(Doc, _T("y"), Point->y);
	Node->append_attribute(Doc, _T("delay"), Point->delay);

	return ERC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ループ回数
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::loadXmlLoop_n(const rapidxml::node_t* Node)
{
	int32_t n;
	rapidxml::attribute_t *attr;

	attr = Node->first_attribute(_T("loop_n"), n);
	if (attr != nullptr){
		m_LoopNum = n;
	}

	return ERC_ok;
}
int32_t WorkBase::saveXmlLoop_n(rapidxml::document_t &Doc ,rapidxml::node_t* Node) const
{
	// ループ1回は出力を省略する。
	if (m_LoopNum != 1){
		Node->append_attribute(Doc, _T("loop_n"), m_LoopNum);
	}

	return ERC_ok;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// コメント
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::loadXmlComment(const rapidxml::node_t* Node)
{
	rapidxml::string_t str;
	rapidxml::attribute_t *attr;

	attr = Node->first_attribute(_T("comment"), str);
	if (attr != nullptr){
		m_Comment = str;
	}

	return ERC_ok;
}
int32_t WorkBase::saveXmlComment(rapidxml::document_t &Doc ,rapidxml::node_t* Node) const
{
	if (!m_Comment.empty()){
		Node->append_attribute(Doc, _T("comment"), m_Comment.c_str());
	}

	return ERC_ok;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// コメント表示
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void WorkBase::setGuiComment(const std::wstring &Txt)
{
	const WCHAR *pc = Txt.c_str();

	SendMessageA(pAutoPointingDlg->m_hWnd, WM_COMMENT, reinterpret_cast<WPARAM>(pc), 0);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 残り回数を表示    引数は試行回数 0:1回目 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void WorkBase::setGuiCnt(int32_t Cnt)
{
	std::wstring str;

	if (m_LoopNum != 0){
		str = std::to_wstring(m_LoopNum - (Cnt+1));
	}

	SendMessageA(pAutoPointingDlg->m_hWnd, WM_CNT, reinterpret_cast<WPARAM>(str.c_str()), 0);
}

