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

	ack = touchPoint(m_TouchPoint);

	return ack;
}
#endif
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ���[�N���� 1��
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t	WorkTouch::procOne()
{
	return touchPoint(m_TouchPoint);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �P����/������ ���ʓǂݏ��� �e�v���Z�X�̓��e��XML�I�u�W�F�N�g��
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



