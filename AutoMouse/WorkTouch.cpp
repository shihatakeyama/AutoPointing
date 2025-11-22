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
#if 0
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ワーク処理
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkTouch::proc()
{
	int32_t ack;

	ack = touchPointAndDelay(m_TouchPoint);

	return ack;
}
#endif
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ワーク処理 1回
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t	WorkTouch::procOne()
{
	SendComment(m_Comment);

	if (!m_Comment.empty()){
		TRACE("WorkTouch::procOne() %ls\n" , m_Comment.c_str());
	}

	return touchPointAndDelay(m_TouchPoint);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 単数個/複数個 共通読み書き 各プロセスの内容をXMLオブジェクトへ
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkTouch::loadXmlNode(const rapidxml::node_t* Node)
{
	loadTouchPoint(Node, m_TouchPoint);
	loadXmlLoop_n(Node);
	loadXmlComment(Node);

	

	return ERC_ok;
}
int32_t WorkTouch::saveXmlNode(rapidxml::document_t &Doc ,rapidxml::node_t *&Node) const
{
	Node = Doc.allocate_node(rapidxml::node_element);

	saveTouchPoint(Doc ,Node, m_TouchPoint);
	saveXmlLoop_n(Doc, Node);
	saveXmlComment(Doc, Node);

	return ERC_ok;
}



