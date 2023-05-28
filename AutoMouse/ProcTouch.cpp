// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ProcTouch.cpp
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


#include "stdafx.h"

#include "AM_define.h"
#include "ProcTouch.h"



ProcTouch::ProcTouch()
:ProcBase(EPT_touch)
,m_TouchPoint(new TouchPoint())
{}

ProcTouch::~ProcTouch()
{
	delete m_TouchPoint;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// **** 信号処理  各処理共通呼び出し処理 ****
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t ProcTouch::proc()
{
//--	delay(m_TouchPoint->delay);
	touchPoint(m_TouchPoint);

	return ERC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// **** 単数個/複数個 共通読み書き 各プロセスの内容をXMLオブジェクトへ ****
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t ProcTouch::loadXmlNode(const rapidxml::node_t* Child/*-- ,sheet_t *Db*/)
{
	loadTouchPoint(Child ,m_TouchPoint);

	return ERC_ok;
}
int32_t ProcTouch::saveXmlNode(rapidxml::node_t *Parent ,rapidxml::document_t &Doc) const
{
	return ERC_ok;
}



