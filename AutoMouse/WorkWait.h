// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// WorkWait.h
//
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#ifndef WORKWAIT_H
#define WORKWAIT_H


#include "WorkBase.h"


class WorkWait
	: public WorkBase{

public:

	WorkWait();
	virtual ~WorkWait();

	// **** �M������  �e�������ʌĂяo������ ****
	virtual int32_t procOne();

	// **** �P����/������ ���ʓǂݏ��� �e�v���Z�X�̓��e��XML�I�u�W�F�N�g�� ****
	virtual int32_t loadXmlNode(const rapidxml::node_t* Child);
	virtual int32_t saveXmlNode(rapidxml::node_t *Parent, rapidxml::document_t &Doc) const;

private:
	int32_t	m_WaitMsec;
};



#endif // #ifndef WORKWAIT_H

