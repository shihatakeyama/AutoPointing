// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ProcTouchs.cpp
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"

#include "AM_define.h"
#include "ProcTouchs.h"



const TCHAR		*ProcTouchs::m_ModeNames[]	= { _T("each") ,_T("anyone") };

ProcTouchs::ProcTouchs()
:ProcBase(EPT_touchs)
,m_Mode(EPTM_each)
{}

ProcTouchs::~ProcTouchs()
{
	clearTouchPoints();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// **** �M������  �e�������ʌĂяo������ ****
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t ProcTouchs::proc()
{
	for(int32_t i=0;i<m_LoopNum;i++){
		procOne();
		if(!isLife())	return ERC_ok;
	}

	return ERC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 1�T����̂�
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t	ProcTouchs::procOne()
{
	int32_t ack = ERC_ok;
	if(m_TouchPoints.size() == 0)	return ERC_ng;

	switch(m_Mode){ //(EPTM_each)
	case EPTM_each:
		for(int32_t i=0;i<m_TouchPoints.size();i++){
			ack = touchPoint(m_TouchPoints[i]);
			if(!isLife())	return ERC_ok;
		}
		break;
	case EPTM_anyone:
		{
		int32_t rdm = rand() % m_TouchPoints.size();
		ack = touchPoint(m_TouchPoints[rdm]);
		}
		break;
	default:
		assert(false);
		break;
	}
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
// **** �P����/������ ���ʓǂݏ��� �e�v���Z�X�̓��e��XML�I�u�W�F�N�g�� ****
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t ProcTouchs::loadXmlNode(const rapidxml::node_t* Child/*-- ,sheet_t *Db*/)
{
#if 0
	// �v���Z�X�̐��ǂݍ���			for ( auto &itr : rapidxml::node_iterator<TCHAR>(ProcessXmlaa)) {
	std::for_each(rapidxml::node_iterator<TCHAR>(Child->first_node()) ,rapidxml::node_iterator<TCHAR>(Child->last_node()) ,[this](const rapidxml::node_t *itr){
		TouchPoint	*point = new TouchPoint();
		loadTouchPoint(itr ,point);
		m_TouchPoints.push_back(point);
	});
#endif

#if 1
	// �v���Z�X�̐��ǂݍ���
	const rapidxml::node_t* itr = rapidxml::first_node(Child);		// ���������� �����H
	while(itr){
		TouchPoint	*point = new TouchPoint();
		loadTouchPoint(itr ,point);
		m_TouchPoints.push_back(point);
		
		itr = rapidxml::next_sibling(itr);
	}
#endif

#if 0
	const rapidxml::node_t *nn = nullptr;
	rapidxml::const_node_iterator<TCHAR>	aaa(nn);
	children<TCHAR>	bbb(nn);
	for(auto *itr:aaa){

	
	}

#endif

	return ERC_ok;
}
int32_t ProcTouchs::saveXmlNode(rapidxml::node_t *Parent ,rapidxml::document_t &Doc) const
{
	return ERC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �^�b�`���X�g�N���A
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ProcTouchs::clearTouchPoints()
{
	for(auto *e:m_TouchPoints){
		delete e;
	}

	m_TouchPoints.clear();
}


