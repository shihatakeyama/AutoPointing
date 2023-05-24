// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// WorkTouchs.cpp
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"

#include "define.h"
#include "WorkTouchs.h"



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
// 信号処理  各処理共通呼び出し処理
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkTouchs::proc()
{
	int32_t ack;
	int32_t i;

	for(i=0;m_LoopNum==0 ? true : i<m_LoopNum;i++){
		if (!isLife())		return ERC_ok;
		ack = procOne();
		if (ack < ERC_ok)	return ack;
	}

	return ERC_ok;
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
int32_t WorkTouchs::loadXmlNode(const rapidxml::node_t* Child)
{
	int32_t ack;
	int32_t val;

	rapidxml::attribute_t *attr = rapidxml::first_attribute(Child ,_T("type"));
	ack = rapidxml::find_attribute_val_index(attr ,m_ModeNames ,val);
	if(ack < ERC_ok){
	}else{
		m_Mode	= static_cast<E_WorkTouchsMode>(val);
	}

	loadXmlLoop_n(Child);

#if 1
	// プロセスの数読み込み
	const rapidxml::node_t* itr = rapidxml::first_node(Child);
	while(itr){
		int32_t idx;
		WorkBase *wb;

		ack = rapidxml::find_node_name_index(itr, m_ProcNames, idx);	// ノード名が _T("works") である事を確認。 
		wb = WorkBase::newProc(static_cast<WorkBase::E_ProcType>(idx));
		if (wb == nullptr)	throw std::wstring(_T("ワーク名が不一致"));

		wb->loadXmlNode(itr);
		m_TouchPoints.push_back(wb);
		
		itr = rapidxml::next_sibling(itr);
	}
#endif

	return ERC_ok;
}
int32_t WorkTouchs::saveXmlNode(rapidxml::node_t *Parent ,rapidxml::document_t &Doc) const
{
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



