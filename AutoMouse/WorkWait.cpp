// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// WorkWait.cpp
//
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"

#include "rapidxml_if.h"

#include "define.h"

#include "WorkWait.h"



WorkWait::WorkWait()
: WorkBase(EPT_wait)
, m_WaitMsec(400)
{}

WorkWait::~WorkWait()
{}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �M������  �e�������ʌĂяo������
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkWait::procOne()
{
	delay(m_WaitMsec);

	return ERC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �P����/������ ���ʓǂݏ��� �e�v���Z�X�̓��e��XML�I�u�W�F�N�g��
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkWait::loadXmlNode(const rapidxml::node_t* Child)
{
	int32_t val;
	rapidxml::attribute_t *attr;

	attr = rapidxml::first_attribute(Child, _T("delay"), val);
	if (attr != nullptr){
		m_WaitMsec = val;
	}
	loadXmlLoop_n(Child);

	return ERC_ok;
}
int32_t WorkWait::saveXmlNode(rapidxml::document_t &Doc ,rapidxml::node_t *&Node) const
{
	Node = rapidxml::allocate_node(Doc);

	rapidxml::append_attribute(Doc, Node, _T("delay"), m_WaitMsec);
	saveXmlLoop_n(Doc, Node);

	return ERC_ok;
}



