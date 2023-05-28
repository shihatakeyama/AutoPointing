// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// WorkTouchs.h
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


#include <vector>

#include "WorkBase.h"

#ifndef PROCTOUCH_H
#define PROCTOUCH_H

class WorkTouchs
	: public WorkBase{

public:
	enum E_WorkTouchsMode{
		 EPTM_each		= 0
		,EPTM_anyone	= 1
		,EPTM_num		= 2
	};


	WorkTouchs();
	virtual ~WorkTouchs();

	// **** �M������  �e�������ʌĂяo������ ****
//	virtual int32_t proc();
	virtual int32_t	procOne();		// 1�T����̂�

	// **** �P����/������ ���ʓǂݏ��� �e�v���Z�X�̓��e��XML�I�u�W�F�N�g�� ****
	virtual int32_t loadXmlNode(const rapidxml::node_t* Child);
	virtual int32_t saveXmlNode(rapidxml::node_t *Parent ,rapidxml::document_t &Doc) const;

private:
	enum E_WorkTouchsMode		m_Mode;
	void clearTouchPoints();
	std::vector<WorkBase*>		m_TouchPoints;

	const static TCHAR			*m_ModeNames[];
};

#endif // PROCTOUCH_H
