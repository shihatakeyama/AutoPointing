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

	// **** 信号処理  各処理共通呼び出し処理 ****
	virtual int32_t proc();

	// **** 単数個/複数個 共通読み書き 各プロセスの内容をXMLオブジェクトへ ****
	virtual int32_t loadXmlNode(const rapidxml::node_t* Child/*-- ,sheet_t *Db*/);
	virtual int32_t saveXmlNode(rapidxml::node_t *Parent ,rapidxml::document_t &Doc) const;

private:
	TouchPoint	*m_TouchPoint;
};

#endif // PROCTOUCHS_H

