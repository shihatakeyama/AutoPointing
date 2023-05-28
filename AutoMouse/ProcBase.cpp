// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ProcBase.cpp
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"

#include "AM_define.h"
#include "ProcBase.h"

//#include "rapidxml_support.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �N���X���̃��X�g
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
const TCHAR		*ProcBase::m_ProcNames[] = { _T("touch") ,_T("touchs") ,_T("wait")	};


ProcBase::ProcBase(enum E_ProcType ProcType)
 : m_ProcType(ProcType),m_LoopNum(1)
{}
ProcBase::~ProcBase()
{}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �����I�u�W�F�N�g�𐶐�
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
// **** �M������  �e�������ʌĂяo������ ****
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t ProcBase::proc()
{
	return ERC_ng;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �^�b�`����Ȃ�
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t ProcBase::touchPoint(const TouchPoint *Point)
{
	delay(Point->delay);

	// RUN���ł���΃^�b�`���삵�܂��B
	if(1){
		
	}

	return ERC_ok;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �҂�
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ProcBase::delay(int32_t Msec)
{
	Sleep(Msec);	// ��
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// true:�ʏ�^�]��
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
bool ProcBase::isLife() const
{
	return true;	// ����������
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// **** �P����/������ ���ʓǂݏ��� �e�v���Z�X�̓��e��XML�I�u�W�F�N�g�� ****
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
// XML����v���Z�X���X�g��ǂݍ���
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
// �^�b�`�ʒu�ƒx���̓ǂݏ���
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

