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

	// **** 信号処理  各処理共通呼び出し処理 ****
//	virtual int32_t proc();
	virtual int32_t	procOne();		// 1週するのみ

	// **** 単数個/複数個 共通読み書き 各プロセスの内容をXMLオブジェクトへ ****
	virtual int32_t loadXmlNode(const rapidxml::node_t* Node);
	virtual int32_t saveXmlNode(rapidxml::document_t &Doc ,rapidxml::node_t *&Node) const;

private:
	enum E_WorkTouchsMode		m_Mode;
	void clearTouchPoints();
	std::vector<WorkBase*>		m_TouchPoints;

	const static TCHAR			*m_ModeNames[];
};

#endif // PROCTOUCH_H
