// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// WorkTouchs.cpp
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"

#include "define.h"
#include "WorkTouchs.h"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 文字列テーブル
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
// 1週するのみ
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t	WorkTouchs::procOne()
{
	uint32_t i;
	int32_t ack = ERC_ok;
	if(m_TouchPoints.size() == 0)	return ERC_ng;

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
		int32_t rdm = rand() % m_TouchPoints.size();
		ack = m_TouchPoints[rdm]->proc();
		}
		break;
	default:
		assert(false);
		break;
	}

	return ERC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 単数個/複数個 共通読み書き 各プロセスの内容をXMLオブジェクトへ
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkTouchs::loadXmlNode(const rapidxml::node_t* Node)
{
	int32_t ack;
	int32_t val;

	ack = rapidxml::first_attribute_val_index(Node, _T("type"), m_ModeNames, val);
	if(ack < ERC_ok){
	}else{
		m_Mode	= static_cast<E_WorkTouchsMode>(val);
	}

#if 1
	// プロセスの数読み込み
	const rapidxml::node_t* itr = rapidxml::first_node(Node);
	while(itr){
		int32_t idx;
		WorkBase *wb;

		ack = rapidxml::node_name_index(itr, m_ProcNames, idx);	// ノード名が _T("works") である事を確認。 
		wb = WorkBase::newProc(static_cast<WorkBase::E_ProcType>(idx));
		if (wb == nullptr)	throw std::wstring(_T("ワーク名が不一致"));

		wb->loadXmlNode(itr);
		m_TouchPoints.push_back(wb);
		
		itr = rapidxml::next_sibling(itr);
	}
#endif
	loadXmlLoop_n(Node);

	return ERC_ok;
}
int32_t WorkTouchs::saveXmlNode(rapidxml::document_t &Doc ,rapidxml::node_t *&Node) const
{
	Node = rapidxml::allocate_node(Doc);
	rapidxml::append_attribute(Doc, Node, _T("type"), m_ModeNames[m_Mode]);

	for (uint32_t i = 0; i<m_TouchPoints.size(); i++){
		const WorkBase *wb = m_TouchPoints[i];
		rapidxml::node_t *child;	//  = rapidxml::append_node(Doc, Node, wb->getProcName());
		wb->saveXmlNode(Doc, child);
		rapidxml::name(child, wb->getProcName());
		rapidxml::append_node(Node, child);
	}

	saveXmlLoop_n(Doc, Node);

	return ERC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// タッチリストクリア
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void WorkTouchs::clearTouchPoints()
{
	for(auto *e:m_TouchPoints){
		delete e;
	}

	m_TouchPoints.clear();
}



