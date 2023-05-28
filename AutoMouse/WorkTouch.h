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
	virtual int32_t loadXmlNode(const rapidxml::node_t* Child);
	virtual int32_t saveXmlNode(rapidxml::node_t *Parent ,rapidxml::document_t &Doc) const;

private:
	TouchPoint	*m_TouchPoint;
};

#endif // PROCTOUCHS_H

