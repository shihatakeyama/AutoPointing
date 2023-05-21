// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  SetParam.cpp
// 
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


#include "stdafx.h"

#include "GnrlDefine.h"

#include "GnrlFilepath.h"
#include "AM_define.h"
#include "AM_extern.h"
#include "ProcBase.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// XML ÉtÉ@ÉCÉãì«Ç›çûÇ›
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t initParam()
{
	int32_t ack;
	TCHAR InitFilePath[MAX_PATH];

	ack = GnrlFilepath::getModuleAttachmentFilePath(InitFilePath, MAX_PATH ,_T("am_ini.xml"));
	if(ack <= 0)	InitFilePath[0] = '\x0';

	rapidxml::document_t doc;
	rapidxml::string_t docbuf;

	ack = rapidxml::load_document(doc ,InitFilePath ,docbuf);
	if(ack < 0)	return ERC_ng;

	rapidxml::node_t *root = doc.first_node();
	rapidxml::first_node(root ,_T("title") ,gTitle);

	rapidxml::node_t *work	= rapidxml::first_node(root ,_T("works"));
	if(work == nullptr)	return ERC_ng;

	ack = ProcBase::loadProcList(gWorks ,work);

	return ERC_ok;
}
int32_t clearParam()
{

	for(auto *e:gWorks)	delete e;
	gWorks.clear();
	gWorkNames.clear();

	return ERC_ok;
}




