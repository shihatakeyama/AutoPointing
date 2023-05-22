// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// WorkBase.h
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#ifndef PROCBASE_H
#define PROCBASE_H

#include <stdint.h>
#include <vector>

struct TouchPoint;

/*namespace rapidxml{
	class node_t;
	class document_t;
}*/

class WorkBase{

public:

	// 処理種別
	enum E_ProcType{
		  EPT_touch			= 0
		, EPT_touchs		= 1
		, EPT_wait			= 2
		, EPT_num			= 3
	};

	WorkBase(enum E_ProcType ProcType);
	virtual ~WorkBase() = 0;

	int32_t getTypeId() const						{ return m_ProcType;					}
	virtual const TCHAR *getProcName() const		{ return m_ProcNames[m_ProcType];		}

	// 処理オブジェクトを生成
	static WorkBase *newProc(enum E_ProcType ValTypeId);

	// **** 信号処理  各処理共通呼び出し処理 ****
	virtual int32_t proc();
	int32_t touchPoint(const TouchPoint *Point);
	void delay(int32_t Msec);
	bool isLife() const;	// true:通常運転中

	// **** 単数個/複数個 共通読み書き 各プロセスの内容をXMLオブジェクトへ ****
	virtual int32_t loadXmlNode(const rapidxml::node_t* Child/*-- ,sheet_t *Db*/);
	virtual int32_t saveXmlNode(rapidxml::node_t *Parent ,rapidxml::document_t &Doc) const;

	// XMLからプロセスリストを読み込み
	static int32_t loadProcList(std::vector<WorkBase*>	&List, rapidxml::node_t* ProcessXml, std::vector<std::wstring> &WorkNames);
	static int32_t saveProcList(const std::vector<WorkBase*> &List, rapidxml::document_t &Doc, rapidxml::node_t* ProcessXml);

	// タッチ位置と遅延の読み書き
	static int32_t loadTouchPoint(const rapidxml::node_t* Node ,TouchPoint *Point);
	static int32_t saveTouchPoint(rapidxml::node_t *Parent ,rapidxml::document_t &Doc ,const TouchPoint *Point);

protected:
	int32_t					m_LoopNum;

private:
	enum E_ProcType			m_ProcType;

	const static TCHAR		*m_ProcNames[];

};

#endif // PROCBASE_H
