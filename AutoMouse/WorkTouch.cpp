// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// WorkTouch.cpp
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


#include "stdafx.h"

#include "define.h"
#include "WorkTouch.h"



WorkTouch::WorkTouch()
:WorkBase(EPT_touch)
,m_TouchPoint(new TouchPoint())
{}

WorkTouch::~WorkTouch()
{
	delete m_TouchPoint;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ワーク処理  各処理共通呼び出し処理
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkTouch::proc()
{
	int32_t ack;

	ack = touchPoint(m_TouchPoint);

	return ack;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 単数個/複数個 共通読み書き 各プロセスの内容をXMLオブジェクトへ
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkTouch::loadXmlNode(const rapidxml::node_t* Child)
{
	loadXmlLoop_n(Child);
	loadTouchPoint(Child ,m_TouchPoint);

	return ERC_ok;
}
int32_t WorkTouch::saveXmlNode(rapidxml::node_t *Parent ,rapidxml::document_t &Doc) const
{
	return ERC_ok;
}



