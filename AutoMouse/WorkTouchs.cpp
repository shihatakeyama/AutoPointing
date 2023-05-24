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
// **** 信号処理  各処理共通呼び出し処理 ****
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkTouchs::proc()
{
	int32_t ack;

	for(int32_t i=0;i<m_LoopNum;i++){
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

	switch(m_Mode){ //(EPTM_each)
	case EPTM_each:
		for(i=0;i<m_TouchPoints.size();i++){
			if (!isLife())	return ERC_ok;
//			ack = touchPoint(m_TouchPoints[i].);
			ack = m_TouchPoints[i]->proc();
		}
		break;
	case EPTM_anyone:
		{
		if (!isLife())	return ERC_ok;
		int32_t rdm = rand() % m_TouchPoints.size();
//		ack = touchPoint(m_TouchPoints[rdm]);
		ack = m_TouchPoints[rdm]->proc();
		}
		break;
	default:
		assert(false);
		break;
	}

	return ERC_ok;
}

namespace rapidxml
{

    //! Iterator of child nodes of xml_node
    template<class Ch>
    class const_node_iterator
    {
    
    public:

        typedef typename xml_node<Ch> value_type;
        typedef typename xml_node<Ch> &reference;
        typedef typename xml_node<Ch> *pointer;
        typedef std::ptrdiff_t difference_type;
        typedef std::bidirectional_iterator_tag iterator_category;
        
        const_node_iterator()
            : m_node(0)
        {
        }

        const_node_iterator(const xml_node<Ch> *node)
            : m_node(node->first_node())
        {
        }
        
        reference operator *() const
        {
            assert(m_node);
            return *m_node;
        }

        pointer operator->() const
        {
            assert(m_node);
            return m_node;
        }

        const_node_iterator& operator++()
        {
            assert(m_node);
            m_node = m_node->next_sibling();
            return *this;
        }

        const_node_iterator operator++(int)
        {
            node_iterator tmp = *this;
            ++this;
            return tmp;
        }

        const_node_iterator& operator--()
        {
            assert(m_node && m_node->previous_sibling());
            m_node = m_node->previous_sibling();
            return *this;
        }

        const_node_iterator operator--(int)
        {
            node_iterator tmp = *this;
            ++this;
            return tmp;
        }

        bool operator ==(const const_node_iterator<Ch> &rhs)
        {
            return m_node == rhs.m_node;
        }

        bool operator !=(const const_node_iterator<Ch> &rhs)
        {
            return m_node != rhs.m_node;
        }

    private:

        const xml_node<Ch> *m_node;

    };
}
#if 1
#include <iostream>
#include <vector>
#include <algorithm>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"	// rapidxml::file
#include "rapidxml_print.hpp"	// std::ofstream << rapidxml::xml_document<>
#include "rapidxml_std.hpp"
#include "rapidxml_iterators.hpp"
#include "rapidxml_std.hpp"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// **** 単数個/複数個 共通読み書き 各プロセスの内容をXMLオブジェクトへ ****
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t WorkTouchs::loadXmlNode(const rapidxml::node_t* Child)
{
	int32_t ack;

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



