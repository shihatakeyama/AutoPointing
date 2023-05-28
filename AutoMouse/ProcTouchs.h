// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ProcTouchs.h
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


#include <vector>

#include "ProcBase.h"

#ifndef PROCTOUCH_H
#define PROCTOUCH_H

class ProcTouchs
: ProcBase{

public:
	enum E_ProcTouchsMode{
		 EPTM_each		= 0
		,EPTM_anyone	= 1
		,EPTM_num		= 2
	};


	ProcTouchs();
	virtual ~ProcTouchs();

	// **** 信号処理  各処理共通呼び出し処理 ****
	virtual int32_t proc();
	int32_t	procOne();	// 1週するのみ

	// **** 単数個/複数個 共通読み書き 各プロセスの内容をXMLオブジェクトへ ****
	virtual int32_t loadXmlNode(const rapidxml::node_t* Child/*-- ,sheet_t *Db*/);
	virtual int32_t saveXmlNode(rapidxml::node_t *Parent ,rapidxml::document_t &Doc) const;

private:
//--	TouchPoint	*m_TouchPoint;
	enum E_ProcTouchsMode		m_Mode;
	void clearTouchPoints();
	std::vector<TouchPoint*>	m_TouchPoints;

	const static TCHAR			*m_ModeNames[];
};

#endif // PROCTOUCH_H
