// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  SetParam.cpp
// 
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


#include "stdafx.h"

#include "GnrlDefine.h"

#include "GnrlFilepath.h"
#include "define.h"
#include "extern.h"
#include "global.h"
#include "WorkBase.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// XML ファイル読み込み
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t initParam()
{
	int32_t ack;
	TCHAR InitFilePath[MAX_PATH];

	// **** 設定ファイルオープン ****

	ack = GnrlFilepath::getModuleAttachmentFilePath(InitFilePath, MAX_PATH ,_T("apd_ini.xml"));
	if(ack <= 0)	InitFilePath[0] = '\x0';

	rapidxml::document_t doc;
	rapidxml::string_t docbuf;

	ack = rapidxml::load_document(doc ,InitFilePath ,docbuf);
	if(ack < 0)	return ERC_ng;

	// **** 各種パラメータ読み込み ****

	rapidxml::node_t *root = doc.first_node();
	rapidxml::node_t *node;
	rapidxml::first_node(root ,_T("title") ,gTitle);

	node = rapidxml::first_node(root, _T("bure"));
	if (node == nullptr){
		gBurePoint = CPoint(4, 4);
	}else{
		int32_t val;
		rapidxml::first_attribute(node, _T("x"), val);	gBurePoint.x = val;
		rapidxml::first_attribute(node, _T("y"), val);	gBurePoint.y = val;
	}

	rapidxml::node_t *work	= rapidxml::first_node(root ,_T("works"));
	if(work == nullptr)	return ERC_ng;

	ack = WorkBase::loadProcList(gWorks, work, gWorkNames);

	return ERC_ok;
}
int32_t clearParam()
{

	for(auto *e:gWorks)	delete e;
	gWorks.clear();
	gWorkNames.clear();

	return ERC_ok;
}




