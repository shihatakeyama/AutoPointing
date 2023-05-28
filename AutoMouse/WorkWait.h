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

	// **** 信号処理  各処理共通呼び出し処理 ****
	virtual int32_t procOne();

	// **** 単数個/複数個 共通読み書き 各プロセスの内容をXMLオブジェクトへ ****
	virtual int32_t loadXmlNode(const rapidxml::node_t* Child);
	virtual int32_t saveXmlNode(rapidxml::node_t *Parent, rapidxml::document_t &Doc) const;

private:
	int32_t	m_WaitMsec;
};



#endif // #ifndef WORKWAIT_H

