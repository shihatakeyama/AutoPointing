// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// WorkBase.cpp
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"
#include "windows.h"

#include "define.h"
#include "extern.h"
#include "global.h"
#include "sub.h"
#include "GnrlThread.h"
#include "WorkBase.h"
#include "WorkTouch.h"
#include "WorkTouchs.h"
#include "WorkWait.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �N���X���̃��X�g
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
const TCHAR		*WorkBase::m_ProcNames[] = { _T("touch"), _T("touchs"), _T("wait") ,nullptr };


WorkBase::WorkBase(enum E_ProcType ProcType)
 : m_ProcType(ProcType),m_LoopNum(1)
{}
WorkBase::~WorkBase()
{}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �����I�u�W�F�N�g�𐶐�
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
WorkBase *WorkBase::newProc(enum E_ProcType ValTypeId)
{
	WorkBase *pbase = nullptr;

	switch (ValTypeId){
		case EPT_touch:
			pbase = new WorkTouch();
			break;
		case EPT_touchs:
			pbase = new WorkTouchs();
			break;
		case EPT_wait:
			pbase = new WorkWait();
			break;
		default:
			break;
	}

	return pbase;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �e�������ʌĂяo������
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::proc()
{
	int32_t ack=ERC_ok;
	int32_t i;

	for(i=0;m_LoopNum==0 ? true : i<m_LoopNum;i++){
		if (!isLife())		return ERC_ok;
		ack = procOne();
		if (ack < ERC_ok)	return ack;
	}

	return ack;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::procOne()
{
	return ERC_invalid_call;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �^�b�`����Ȃ�
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::touchPointAndDelay(const TouchPoint *Point)
{
	ASSERT(Point);
	int32_t ack = ERC_ng;

	// RUN���ł���΃^�b�`���삵�܂��B
	if(isLife()){
		ack = AP_pointingDesiredWindow(CPoint(Point->x,Point->y));
		if (ack < ERC_ok)	return ack;
	}

	delay(Point->delay);

	return ack;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �҂�
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void WorkBase::delay(int32_t Msec)
{	
	APD_Sleep(Msec);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// true:�ʏ�^�]��
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
bool WorkBase::isLife()
{
	return (g_Operation != 0);
//	return gOperationThread.isLife();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// **** �P����/������ ���ʓǂݏ��� �e�v���Z�X�̓��e��XML�I�u�W�F�N�g�� ****
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::loadXmlNode(const rapidxml::node_t* Node)
{
	return ERC_ng;
}
int32_t WorkBase::saveXmlNode(rapidxml::document_t &Doc ,rapidxml::node_t *&ProcList) const
{
	return ERC_ng;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// XML���烏�[�N���X�g��ǂݍ���
// List�F ���[�N�̐� �z��v�f���ɂȂ�܂��B
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::loadWorkList(std::vector<WorkBase*>	&List, rapidxml::node_t* ProcessXml, std::vector<std::wstring> &WorkNames)
{
	int32_t ack;
	rapidxml::node_t *node = ProcessXml->first_node();
	if (node == nullptr)	ERC_ng;
	std::wstring name;

	// ���[�N�̐��������[�v���܂��B
	while (node){
		rapidxml::attribute_t *attr = rapidxml::first_attribute(node, _T("name"), name);

		int32_t idx;
#if 0
		ack = rapidxml::find_node_name_index(node, m_ProcNames, idx);	// �m�[�h���� _T("works") �ł��鎖���m�F�B 
		if (ack < 0)	break;
#else
		ack = rapidxml::comp_node_name(node, _T("work"));
		if (ack < 0)	throw std::wstring(_T("not work"));

		idx = EPT_touchs;
#endif
		WorkBase *proc = newProc(static_cast<E_ProcType>(idx));
		ack = proc->loadXmlNode(node);
		if (ack < 0)	break;

		List.push_back(proc);
		WorkNames.push_back(name);

		node = rapidxml::next_sibling(node);
	}


	return ERC_ok;
}
int32_t WorkBase::saveWorkList(const std::vector<WorkBase*> &List, rapidxml::document_t &Doc, rapidxml::node_t* ProcessXml,const std::vector<std::wstring> &WorkNames)
{
	ASSERT(List.size() == WorkNames.size());

	rapidxml::node_t *node;

	for (uint32_t i = 0; i<List.size(); i++){
		List[i]->saveXmlNode(Doc ,node);
		rapidxml::name(node, _T("work"));
		rapidxml::attribute_t *attr = rapidxml::allocate_attribute(Doc ,_T("name"), WorkNames[i].c_str());
		rapidxml::insert_attribute(node ,rapidxml::first_attribute(node) ,attr);	// �m�[�h�̏��߂̕��ɃA�g���r���[�g��ǉ�
		rapidxml::append_node(ProcessXml, node);
	}

	return ERC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ���������
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::clearProcList(std::vector<WorkBase*> &List)
{
	for (auto *e : List)	delete e;
	List.clear();

	return ERC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �^�b�`�ʒu�ƒx���̓ǂݏ���
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::loadTouchPoint(const rapidxml::node_t* Node ,TouchPoint *Point)
{
	rapidxml::  attribute_t	*attr;

	attr = rapidxml::first_attribute(Node ,_T("x") ,Point->x);
	if(attr == nullptr)	throw _T("x");
	attr = rapidxml::first_attribute(Node ,_T("y") ,Point->y);
	if(attr == nullptr)	throw _T("y");
	attr = rapidxml::first_attribute(Node ,_T("delay") ,Point->delay);
	if(attr == nullptr)	throw _T("delay");

	return ERC_ok;
}
int32_t WorkBase::saveTouchPoint(rapidxml::document_t &Doc ,rapidxml::node_t *Node, const TouchPoint *Point)
{
	rapidxml::append_attribute(Doc, Node, _T("x"), Point->x);
	rapidxml::append_attribute(Doc, Node, _T("y"), Point->y);
	rapidxml::append_attribute(Doc, Node, _T("delay"), Point->delay);

	return ERC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ���[�v��
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::loadXmlLoop_n(const rapidxml::node_t* Node)
{
	int32_t n;
	rapidxml::attribute_t *attr;

	attr = rapidxml::first_attribute(Node, _T("loop_n"), n);
	if (attr != nullptr){
		m_LoopNum = n;
	}

	return ERC_ok;
}
int32_t WorkBase::saveXmlLoop_n(rapidxml::document_t &Doc ,rapidxml::node_t* Node) const
{
	// ���[�v1��͏o�͂��ȗ�����B
	if (m_LoopNum != 1){
		rapidxml::append_attribute(Doc, Node, _T("loop_n"), m_LoopNum);
	}

	return ERC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �R�����g
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkBase::loadXmlComment(const rapidxml::node_t* Node)
{
	rapidxml::string_t str;
	rapidxml::attribute_t *attr;

	attr = rapidxml::first_attribute(Node, _T("comment"), str);
	if (attr != nullptr){
		m_Comment = str;
	}

	return ERC_ok;
}
int32_t WorkBase::saveXmlComment(rapidxml::document_t &Doc ,rapidxml::node_t* Node) const
{
	if (!m_Comment.empty()){
		rapidxml::append_attribute(Doc, Node, _T("comment"), m_Comment.c_str());
	}

	return ERC_ok;
}
