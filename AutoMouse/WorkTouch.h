// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ProcTouch.h
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


#ifndef PROCTOUCHS_H
#define PROCTOUCHS_H


#include "WorkBase.h"


class WorkTouch
	: public WorkBase{

public:
	
	WorkTouch();
	virtual ~WorkTouch();

	// **** ���[�N����  �e�������ʌĂяo������ ****
//	virtual int32_t proc();
	virtual int32_t	procOne();		// 1��

	// **** �P����/������ ���ʓǂݏ��� �e�v���Z�X�̓��e��XML�I�u�W�F�N�g�� ****
	virtual int32_t loadXmlNode(const rapidxml::node_t* Node);
	virtual int32_t saveXmlNode(rapidxml::document_t &Doc, rapidxml::node_t *&Node) const;

private:
	TouchPoint	*m_TouchPoint;
};

#endif // PROCTOUCHS_H

