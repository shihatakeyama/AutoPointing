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
// ���[�N����
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkTouch::proc()
{
	int32_t ack;

	ack = touchPointAndDelay(m_TouchPoint);

	return ack;
}
#endif
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ���[�N���� 1��
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t	WorkTouch::procOne()
{
//	setGuiComment(m_Comment);
	SendComment(m_Comment);

	if (!m_Comment.empty()){
		TRACE(m_Comment.c_str());
	}

	return touchPointAndDelay(m_TouchPoint);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �P����/������ ���ʓǂݏ��� �e�v���Z�X�̓��e��XML�I�u�W�F�N�g��
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
	Node = rapidxml::allocate_node(Doc);

	saveTouchPoint(Doc ,Node, m_TouchPoint);
	saveXmlLoop_n(Doc, Node);
	saveXmlComment(Doc, Node);

	return ERC_ok;
}



