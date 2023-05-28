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
// **** �M������  �e�������ʌĂяo������ ****
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t ProcTouch::proc()
{
//--	delay(m_TouchPoint->delay);
	touchPoint(m_TouchPoint);

	return ERC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// **** �P����/������ ���ʓǂݏ��� �e�v���Z�X�̓��e��XML�I�u�W�F�N�g�� ****
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



