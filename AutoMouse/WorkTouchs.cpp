// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// WorkTouchs.cpp
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"

#include "define.h"
#include "WorkTouchs.h"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ������e�[�u��
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
const TCHAR		*WorkTouchs::m_ModeNames[]	= { _T("each") ,_T("anyone") };

WorkTouchs::WorkTouchs()
:WorkBase(EPT_touchs)
,m_Mode(EPTM_each)
{}

WorkTouchs::~WorkTouchs()
{
	clearTouchPoints();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 1�T����̂�
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t	WorkTouchs::procOne()
{
	uint32_t i;
	int32_t ack = ERC_ok;
	if(m_TouchPoints.size() == 0)	return ERC_ng;

//	SendComment(m_Comment);

	switch(m_Mode){
	case EPTM_each:
		for(i=0;i<m_TouchPoints.size();i++){
			if (!isLife())	return ERC_ok;
			ack = m_TouchPoints[i]->proc();
		}
		break;
	case EPTM_anyone:
		{
		if (!isLife())	return ERC_ok;
		int32_t rdm = rand();
		int32_t idx = rdm  % m_TouchPoints.size();
		ack = m_TouchPoints[idx]->proc();
		}
		break;
	default:
		assert(false);
		break;
	}

	return ERC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �P����/������ ���ʓǂݏ��� �e�v���Z�X�̓��e��XML�I�u�W�F�N�g��
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkTouchs::loadXmlNode(const rapidxml::node_t* Node)
{
	int32_t ack;
	int32_t val;

	ack = Node->first_attribute_val_index(_T("type"), m_ModeNames, val);
	if(ack < ERC_ok){
	}else{
		m_Mode	= static_cast<E_WorkTouchsMode>(val);
	}

#if 1
	// �v���Z�X�̐��ǂݍ���
	const rapidxml::node_t* itr = Node->first_node();	//-- rapidxml::first_node(Node);
	while(itr){
		int32_t idx;
		WorkBase *wb;

		ack = itr->node_name_index(m_ProcNames, idx);	// �m�[�h���� _T("works") �ł��鎖���m�F�B 
		wb = WorkBase::newProc(static_cast<WorkBase::E_ProcType>(idx));
		if (wb == nullptr)	throw std::wstring(_T("���[�N�����s��v"));

		wb->loadXmlNode(itr);
		m_TouchPoints.push_back(wb);
		
		itr = itr->next_sibling();
	}
#endif
	loadXmlLoop_n(Node);
	loadXmlComment(Node);

	return ERC_ok;
}
int32_t WorkTouchs::saveXmlNode(rapidxml::document_t &Doc ,rapidxml::node_t *&Node) const
{
	Node = Doc.allocate_node(rapidxml::node_element);
	Node->append_attribute(Doc, _T("type"), m_ModeNames[m_Mode]);

	for (uint32_t i = 0; i<m_TouchPoints.size(); i++){
		const WorkBase *wb = m_TouchPoints[i];
		rapidxml::node_t *child;	//  = rapidxml::append_node(Doc, Node, wb->getProcName());
		wb->saveXmlNode(Doc, child);
		child->name(wb->getProcName());
//--		rapidxml::append_node(Node, child);
		Node->append_node(child);
	}

	saveXmlLoop_n(Doc, Node);
	saveXmlComment(Doc, Node);

	return ERC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �^�b�`���X�g�N���A
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void WorkTouchs::clearTouchPoints()
{
	for(auto *e:m_TouchPoints){
		delete e;
	}

	m_TouchPoints.clear();
}



