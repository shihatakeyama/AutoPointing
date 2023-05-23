// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// WorkWait.cpp
//
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"

#include "define.h"

#include "WorkWait.h"



WorkWait::WorkWait()
: WorkBase(EPT_wait)
, m_WaitMsec(400)
{}

WorkWait::~WorkWait()
{}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// **** �M������  �e�������ʌĂяo������ ****
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkWait::proc()
{
	delay(m_WaitMsec);

	return ERC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// **** �P����/������ ���ʓǂݏ��� �e�v���Z�X�̓��e��XML�I�u�W�F�N�g�� ****
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkWait::loadXmlNode(const rapidxml::node_t* Child/*-- ,sheet_t *Db*/)
{
	int32_t val;
	rapidxml::attribute_t *attr;

	loadLoop_n(Child);

	attr = rapidxml::first_attribute(Child, _T("wait"), val);
	if (attr != nullptr){
		m_WaitMsec = val;
	}

	return ERC_ok;
}
int32_t WorkWait::saveXmlNode(rapidxml::node_t *Parent, rapidxml::document_t &Doc) const
{
	return ERC_ok;
}


