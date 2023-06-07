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
	virtual int32_t procOne();

	int32_t touchPointAndDelay(const TouchPoint *Point);
	void delay(int32_t Msec);
	static bool isLife();	// true:通常運転中

	// **** 単数個/複数個 共通読み書き 各プロセスの内容をXMLオブジェクトへ ****
	virtual int32_t loadXmlNode(const rapidxml::node_t* Node);
	virtual int32_t saveXmlNode(rapidxml::document_t &Doc ,rapidxml::node_t *&Node) const;

	// XMLからプロセスリストを読み込み
	static int32_t loadWorkList(std::vector<WorkBase*>	&List, rapidxml::node_t* ProcessXml, std::vector<std::wstring> &WorkNames);
	static int32_t saveWorkList(const std::vector<WorkBase*> &List, rapidxml::document_t &Doc, rapidxml::node_t* ProcessXml, const std::vector<std::wstring> &WorkNames);

	// メモリ解放
	static int32_t clearProcList(std::vector<WorkBase*> &List);


	// 単体のタッチ位置と遅延の読み書き
	static int32_t loadTouchPoint(const rapidxml::node_t* Node ,TouchPoint *Point);
	static int32_t saveTouchPoint(rapidxml::document_t &Doc ,rapidxml::node_t *Node, const TouchPoint *Point);

	// 回数読み
	int32_t loadXmlLoop_n(const rapidxml::node_t* Node);
	int32_t saveXmlLoop_n(rapidxml::document_t &Doc ,rapidxml::node_t* Node) const;


protected:
	int32_t					m_LoopNum;
	const static TCHAR		*m_ProcNames[];

private:
	enum E_ProcType			m_ProcType;

};

#endif // PROCBASE_H
