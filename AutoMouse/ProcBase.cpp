// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ProcBase.cpp
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"

#include "AM_define.h"
#include "ProcBase.h"

//#include "rapidxml_support.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// クラス名のリスト
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
const TCHAR		*ProcBase::m_ProcNames[] = { _T("touch") ,_T("touchs") ,_T("wait")	};


ProcBase::ProcBase(enum E_ProcType ProcType)
 : m_ProcType(ProcType),m_LoopNum(1)
{}
ProcBase::~ProcBase()
{}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 処理オブジェクトを生成
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
ProcBase *ProcBase::newProc(enum E_ProcType ValTypeId)
{
	ProcBase *pbase = nullptr;

	switch (ValTypeId){
		case EPT_touch:
//++			pbase = new ProcTouch();
			break;
		case EPT_touchs:
//++			pbase = new ProcTouch();
			break;
		case EPT_wait:
//++			pbase = new ProcWait();
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
int32_t ProcBase::proc()
{
	return ERC_ng;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// タッチ操作など
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t ProcBase::touchPoint(const TouchPoint *Point)
{
	delay(Point->delay);

	// RUN中であればタッチ操作します。
	if(1){
		
	}

	return ERC_ok;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 待ち
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ProcBase::delay(int32_t Msec)
{
	Sleep(Msec);	// 仮
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// true:通常運転中
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
bool ProcBase::isLife() const
{
	return true;	// ※※※※※
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// **** 単数個/複数個 共通読み書き 各プロセスの内容をXMLオブジェクトへ ****
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t ProcBase::loadXmlNode(const rapidxml::node_t* Child/*-- ,sheet_t *Db*/)
{
	return ERC_ng;
}
int32_t ProcBase::saveXmlNode(rapidxml::node_t *ProcList ,rapidxml::document_t &Doc) const
{
	return ERC_ng;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// XMLからプロセスリストを読み込み
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t ProcBase::loadProcList(std::vector<ProcBase*>	&List ,rapidxml::node_t* ProcessXml/*-- ,sheet_t *Db*/)
{
	return ERC_ng;
}
int32_t ProcBase::saveProcList(const std::vector<ProcBase*> &List ,rapidxml::document_t &Doc ,rapidxml::node_t* ProcessXml)
{
	return ERC_ng;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// タッチ位置と遅延の読み書き
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t ProcBase::loadTouchPoint(const rapidxml::node_t* Node ,TouchPoint *Point)
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
int32_t ProcBase::saveTouchPoint(rapidxml::node_t *Parent ,rapidxml::document_t &Doc ,const TouchPoint *Point)
{
	return ERC_ok;
}

