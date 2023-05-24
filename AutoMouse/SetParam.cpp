// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  SetParam.cpp
// 
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


#include "stdafx.h"

#include <mutex>

#include "GnrlDefine.h"
#include "GnrlFilepath.h"
#include "define.h"
#include "extern.h"
#include "global.h"
#include "WorkBase.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// XML �t�@�C���ǂݍ���
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t initParam()
{
	int32_t ack;
	TCHAR InitFilePath[MAX_PATH];

	// �r��
	std::lock_guard<std::mutex> lock(gWorkMutex);

	// **** �ݒ�t�@�C���I�[�v�� ****

	ack = GnrlFilepath::getModuleAttachmentFilePath(InitFilePath, MAX_PATH ,_T("apd_ini.xml"));
	if (ack <= 0)	throw std::wstring(_T("apd_ini.xml ������܂���"));

	rapidxml::document_t doc;
	rapidxml::string_t docbuf;

	try{
		ack = rapidxml::load_document(doc, InitFilePath, docbuf);
		if (ack < 0){ throw std::wstring(_T("load document")); return ERC_ng; }
	}
	catch (const std::wstring& e){
		std::wstring  str(_T("�f�[�^�ǂݍ��ݎ��s\n"));
		str += e;
		throw str;
	}
#if 1
	catch (const rapidxml::parse_error &e){
		std::wstring  str(_T("XML �f�[�^�ǂݍ��ݎ��s\n"));
		str += CString(e.what()) + '\n' + e.where<rapidxml::char_t>();

		throw str;
	}
#endif

	// **** �e��p�����[�^�ǂݍ��� ****

	rapidxml::node_t *root = doc.first_node();
	rapidxml::node_t *node;
	if (rapidxml::comp_node_name(root, gApplicatonName.c_str()) != 0){
		throw std::wstring(_T("XML �̏������Ⴂ�܂�"));
	}

	rapidxml::first_node(root ,_T("title") ,gTitle);

	node = rapidxml::first_node(root, _T("target"), gTitle);
	if (node){
		int32_t val;
		rapidxml::first_attribute(node, _T("window_name"), gTargetWindowName);
		rapidxml::first_attribute(node, _T("x"), val);	gBasePoint.x = val;
		rapidxml::first_attribute(node, _T("y"), val);	gBasePoint.y = val;
	}

	node = rapidxml::first_node(root, _T("blur"));
	if (node == nullptr){
		gBurePoint= CPoint(4, 4);
		gBureTime = 4;
	}else{
		int32_t val;
		rapidxml::first_attribute(node, _T("x"), val);	gBurePoint.x = val;
		rapidxml::first_attribute(node, _T("y"), val);	gBurePoint.y = val;
		rapidxml::first_attribute(node, _T("time"), val);	gBureTime = val;
	}

	// 
	node = rapidxml::first_node(root, _T("end_time"));
	if (node == nullptr){
	}
	else{
		rapidxml::first_attribute(node, _T("time0"), gNowTime[0]);
		rapidxml::first_attribute(node, _T("time1"), gNowTime[1]);
		rapidxml::first_attribute(node, _T("time2"), gNowTime[2]);
	}

	rapidxml::node_t *work	= rapidxml::first_node(root ,_T("works"));
	if (work == nullptr){
		throw std::wstring(_T("XML ��works �^�O������܂���B"));
	}

	ack = WorkBase::loadWorkList(gWorks, work, gWorkNames);


	return ERC_ok;
}
int32_t clearParam()
{
	std::lock_guard<std::mutex> lock(gWorkMutex);

	WorkBase::clearProcList(gWorks);

	gWorkNames.clear();

	return ERC_ok;
}




