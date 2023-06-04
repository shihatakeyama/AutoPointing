// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// GnrlCom.cpp
//
// COM�|�[�g�A�N�Z�X�ׂ̈̃N���X
// �f�o�O���o�͗p�Ȃ̂ŁA�ڐA���������\���Ƃ����B
//
// Visual studio�@�Ŏg�p����ꍇ�A
// [�v���p�e�B] >[C / C++] >[�v���v���Z�b�T]�ɉ��L���`
//  _CRT_SECURE_NO_WARNINGS
//
// �ݒ�l�͉��L�̃y�[�W���Q�Ƃ����B
//  http://homepage2.nifty.com/meiwadensokuki/dcb.html
//
// use: C++,WIN32API
// Version 1.1
//
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 
// ����
//   2012/12/31  �X�y�[�X�p���e�B��ǉ�
//
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


#include "stdafx.h"	

#include "windows.h"
#include "GnrlString.h"
#include "GnrlCom.h"

#ifdef _MSC_VER
#define DISABLE_C4996   __pragma(warning(push)) __pragma(warning(disable:4996))
#define ENABLE_C4996    __pragma(warning(pop))
#else
#define DISABLE_C4996
#define ENABLE_C4996
#endif

const TCHAR	*GnrlCom::m_PortNoText		= _T("PortNo");
const TCHAR	*GnrlCom::m_BaudRateText	= _T("BaudRate");
const TCHAR	*GnrlCom::m_TimeoutText		= _T("Timeout");
const TCHAR	*GnrlCom::m_StopbitText		= _T("Stopbit");
const TCHAR	*GnrlCom::m_ParityText		= _T("Parity");


const int GnrlCom::EC_ComOpen	= -1;				// �ʐM�|�[�g���I�[�v�����Ă��Ȃ�
const int GnrlCom::EC_Read		= -2;				// ���肩���M�ł��Ȃ������ꍇ�̃G���[�R�[�h
const int GnrlCom::EC_Write		= -3;				// ���肩���M�ł��Ȃ������ꍇ�̃G���[�R�[�h
void* const GnrlCom::EC_FileOpenError = (void*)0xFFFFFFFF;	// COM�|�[�g�I�[�v�����s�����ꍇ�̃G���[�R�[�h

const int GnrlCom::m_BaudRateList[EB_num] = {
	300, 1200, 4800, 9600, 19200, 38400, 115200, 230400
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// sectionName	�Z�N�V������
// KeyName		�L�[(=�̍���)��
// Val			�l(=�̉E��)
// FilePath		�t�@�C���p�X
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
static BOOL WritePrivateProfileInt(LPCTSTR sectionName, LPCTSTR KeyName, int Val, LPCTSTR FilePath)
{
	TCHAR	str[32];

	Tsprintf_s(str, 32, _T("%d"), Val);
	//	_itow(Val, tmp, 10);

	return ::WritePrivateProfileString(sectionName, KeyName, str, FilePath);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  MFC GUI �֘A
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#ifdef USE_MFC
// GUI �� ���l �ϊ�
int GnrlCom::fromGuiBaudRate(const CComboBox &BaudRate)
{
	return m_BaudRateList[BaudRate.GetCurSel()];
}
int GnrlCom::fromGuiDataBit(const CComboBox &Data)
{
	return 0;	// 8�Œ�
}
GnrlCom::EParity GnrlCom::fromGuiParity(const CComboBox &Parity)
{
	return (EParity)Parity.GetCurSel();
}
GnrlCom::EStopBit GnrlCom::fromGuiStopBit(const CComboBox &StopBit)
{
	return (EStopBit)StopBit.GetCurSel();
}
int GnrlCom::fromGuiFrow(const CComboBox &Frow)
{
	return 0;	// None�Œ�
}

// GUI �� ���N���X��
int GnrlCom::getGuiBaudRate(const CComboBox &BaudRate)
{
	return m_BaudRate = fromGuiBaudRate(BaudRate);
}
int GnrlCom::getGuiDataBit(const CComboBox &Data)
{
	return 0;	// 8�Œ�
}
int GnrlCom::getGuiParity(const CComboBox &Parity)
{
	return m_Parity = fromGuiParity(Parity);
}
int GnrlCom::getGuiStopBit(const CComboBox &StopBit)
{
	return m_StopBit = fromGuiStopBit(StopBit);
}
int GnrlCom::getGuiFrow(const CComboBox &Frow)
{
	return 0;	// None�Œ�
}

// ���N���X�@�� GUI ��
int GnrlCom::setGuiBaudRate(CComboBox &BaudRate) const
{
	int i;
	int ordinal;
	TCHAR	buf[32];

	BaudRate.Clear();

	for (i = 0; i<EB_num; i++){
		Tsprintf_s(buf, 32, _T("%d"), m_BaudRateList[i]);
		BaudRate.InsertString(i, buf);
	}

	ordinal = bpsToEbps(m_BaudRate);

	BaudRate.SetCurSel(ordinal);

	return 0;
}
int GnrlCom::setGuiData(CComboBox &Data) const
{
	Data.Clear();


	Data.InsertString(0, _T("8"));

	Data.SetCurSel(0);

	return 0;
}
int GnrlCom::setGuiParity(CComboBox &Parity) const
{
	Parity.Clear();

	Parity.InsertString(EPARITY_no		, _T("NONE"));
	Parity.InsertString(EPARITY_even	, _T("EVEN"));
	Parity.InsertString(EPARITY_odd		, _T("ODD"));

	if (m_Parity >= Parity.GetCount()){
		Parity.SetCurSel(0);
	}
	else{
		Parity.SetCurSel(m_Parity);
	}

	return 0;
}
int GnrlCom::setGuiStopBit(CComboBox &StopBit) const
{
	StopBit.Clear();

	StopBit.InsertString(ESTOPBIT_1		, _T("1"));
	StopBit.InsertString(ESTOPBIT_1_5	, _T("1.5"));
	StopBit.InsertString(ESTOPBIT_2		, _T("2"));

	if (m_StopBit >= StopBit.GetCount()){
		StopBit.SetCurSel(0);
	}
	else{
		StopBit.SetCurSel(m_StopBit);
	}

	return 0;
}
int GnrlCom::setGuiFrow(CComboBox &Frow) const
{
	Frow.Clear();

	Frow.InsertString(0, _T("None"));

	Frow.SetCurSel(m_Parity);

	return 0;
}
#endif

#ifdef USE_XML
	
//#include "rapidxml.hpp"
//#include "rapidxml_utils.hpp"
//#include "rapidxml_iterators.hpp"
//#include "rapidxml_std.hpp"
//#include "rapidxml_print.hpp"	// std::ofstream << rapidxml::xml_document<>
//#include "rapidxml_support.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// XML �ǂݏ���
// load()/save()�Ƃ���Node�͎��m�[�h��ݒ肵�ĉ������B
// save()���ɕԂ�Node�́A�^�O�����ݒ肳��Ă��܂���̂ŁA�Ăь��Ŗ��O���ĉ������B
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t GnrlCom::loadXmlNode(const rapidxml::node_t *Node)
{
	int32_t val;

	init();	

	if (Node == nullptr)	return ECRC_ng;
	const rapidxml::node_t *node = Node;

	rapidxml::first_attribute(node ,m_PortNoText	,m_PortNo);
	rapidxml::first_attribute(node ,m_BaudRateText	,m_BaudRate);

	rapidxml::first_attribute(node ,m_TimeoutText	,m_TimeOut);
	rapidxml::first_attribute(node ,m_StopbitText	,val);
	m_StopBit	= static_cast<enum EStopBit>(val);	
	rapidxml::first_attribute(node ,m_ParityText	,val);
	m_Parity = static_cast<enum EParity>(val);
	
	m_State |= ESTATEBIT_load;

	return ECRC_ok;
}
int32_t GnrlCom::saveXmlNode(rapidxml::node_t *&Node ,rapidxml::document_t &Doc) const
{
	if (Node == nullptr)	return ECRC_ng;

	rapidxml::node_t *node = Doc.allocate_node(rapidxml::node_element);

	rapidxml::append_attribute(Doc ,node ,m_PortNoText	,m_PortNo);
	rapidxml::append_attribute(Doc ,node ,m_BaudRateText,m_BaudRate);

	rapidxml::append_attribute(Doc ,node ,m_TimeoutText	,m_TimeOut);
	rapidxml::append_attribute(Doc ,node ,m_StopbitText	,m_StopBit);
	rapidxml::append_attribute(Doc ,node ,m_ParityText	,m_Parity);

	Node = node;

	return ECRC_ok;
}
#endif

GnrlCom::GnrlCom()
 : m_ComNo(1)
 , m_State(ESTATEBIT_clear)
{
	init();
	preset();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ini�t�@�C������ʐM�ݒ��ǂݍ���.
// ���� IniFile �ݒ�t�@�C�����L�q���ꂽini�t�@�C���̑��΃p�X(��΃p�X�͕s��)
//      Section �Z�N�V������
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
GnrlCom::GnrlCom(const TCHAR *IniFile, const TCHAR *Section)
: m_ComNo(1)
, m_State(ESTATEBIT_clear)
{
	init();
	preset();
	loadParameter(IniFile, Section);
}

GnrlCom::~GnrlCom()
{
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �ݒ�t�@�C���Ȃǂ̓���f�B���N�g�����ɑ��݂���IniFileName�t�@�C���̐�΃p�X��߂�
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int GnrlComgetFinePath(TCHAR *OutFilePath,int OutBufSize , const TCHAR *IniFileName)
{
	const TCHAR	*pc;
	TCHAR *cpc;
	int		ret;

	pc = Tstrrchr(IniFileName, '\\');
	if(pc == NULL)
	{
//		strcpy(OutFilePath,__argv[0]);	// �R�}���h�������玩�v���O�����ʒu���擾
		ret = GetModuleFileName(NULL, OutFilePath, MAX_PATH);
		if(ret <= 0)
			return ret;
		cpc = Tstrrchr(OutFilePath, '\\');
		cpc[1] = '\0';
		Tstrcat_s(OutFilePath, OutBufSize, IniFileName);
	}else
	{
		Tstrcpy_s(OutFilePath, OutBufSize, IniFileName);
	}

	return 0;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �ʐM�ݒ��ini�t�@�C������ǂݍ���
//   IniFile  �ݒ�t�@�C�����L�q���ꂽini�t�@�C���̑��΃p�X(��΃p�X�͕s��)
//   Section  ini�t�@�C���̃Z�N�V����
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void GnrlCom::loadParameter(const TCHAR *IniFile, const TCHAR *Section)
{
	TCHAR	file_name[MAX_PATH];

#if 0
	char	*pc;
	strcpy(file_name,__argv[0]);
	pc=strrchr(file_name,'\\');
	pc[1]='\0';
	strcat(file_name,IniFile);
#else
	GnrlComgetFinePath(file_name, MAX_PATH, IniFile);
#endif

	m_PortNo	= GetPrivateProfileInt(Section, m_PortNoText	, m_PortNo	, file_name);
	m_BaudRate	= GetPrivateProfileInt(Section,m_BaudRateText	, m_BaudRate, file_name);
	m_TimeOut	= GetPrivateProfileInt(Section,m_TimeoutText	, m_TimeOut	, file_name);
	m_StopBit	= (enum GnrlCom::EStopBit)GetPrivateProfileInt(Section, m_StopbitText, m_StopBit, file_name);
	m_Parity	= (enum GnrlCom::EParity)GetPrivateProfileInt(Section, m_ParityText, m_Parity, file_name);

	m_State |= ESTATEBIT_load;
}
void GnrlCom::saveParameter(const TCHAR *IniFile, const TCHAR *Section)
{
	TCHAR	file_name[MAX_PATH];

	GnrlComgetFinePath(file_name, MAX_PATH, IniFile);

	// WritePrivateProfileInt(LPCTSTR sectionName, LPCTSTR KeyName, int Val, LPCTSTR FilePath)

	WritePrivateProfileInt(Section, _T("PortNo"), m_PortNo, file_name);
	WritePrivateProfileInt(Section, _T("BaudRate"), m_BaudRate, file_name);
	WritePrivateProfileInt(Section, _T("Timeout"), m_TimeOut, file_name);
	WritePrivateProfileInt(Section, _T("Stopbit"), m_StopBit, file_name);
	WritePrivateProfileInt(Section, _T("Parity"), m_Parity, file_name);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �ʐM�ݒ�� �ݒ�/�擾
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void GnrlCom::putParameter(int ComNum, int Bps, int TimeOut, enum EStopBit StopBit, enum EParity Parity)
{
	m_ComNo		= ComNum;
	m_Bps		= Bps;			// ���x	
	m_TimeOut	= TimeOut;		// �^�C���A�E�g	
	m_StopBit	= StopBit;		// �X�g�b�v�r�b�g
	m_Parity	= Parity;		// �p���e�B	
}

void GnrlCom::saveParameter(const TCHAR *IniFile, const TCHAR *Section)
{
	TCHAR	file_name[MAX_PATH];

	GnrlComgetFinePath(file_name, MAX_PATH, IniFile);

	// WritePrivateProfileInt(LPCTSTR sectionName, LPCTSTR KeyName, int Val, LPCTSTR FilePath)

	WritePrivateProfileInt(Section, m_PortNoText	,m_PortNo,	file_name);
	WritePrivateProfileInt(Section, m_BaudRateText	,m_BaudRate,file_name);
	WritePrivateProfileInt(Section, m_TimeoutText	,m_TimeOut,	file_name);
	WritePrivateProfileInt(Section, m_StopbitText	,m_StopBit, file_name);
	WritePrivateProfileInt(Section, m_ParityText	,m_Parity,	file_name);
}

void GnrlCom::getParameter(int &ComNo, int &Bps, int &TimeOut, enum EStopBit &StopBit, enum EParity &Parity) const
{
	ComNo	= m_ComNo;			// Com�̔ԍ�
	Bps		= m_Bps;			// ���x		
	TimeOut	= m_TimeOut;		// �^�C���A�E�g
	StopBit = m_StopBit;		// �X�g�b�v�r�b�g
	Parity	= m_Parity;			// �p���e�B
}



int GnrlCom::bpsToEbps(int Bps)
{
	int	i;

	for (i = 0; i < EB_num; i++){
		if (Bps == m_BaudRateList[i]){
			return i;
		}
	}

	return 0;
}
int GnrlCom::ebpsToBps(int EBps)
{
	if ((EBps < 0) || (EBps >= EB_num))	{ return m_BaudRateList[EB_300bps]; }

	return m_BaudRateList[EBps];
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// PC��COM�|�[�g���I�[�v������
//  �߂�l == 0 ����
//         < 0 ���s
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int GnrlCom::open()
{
	close();	// �I�[�v�����Ă��Ă��w�����ꂽ�̂�����I�[�v������蒼���B

#if 1
	DISABLE_C4996//
	wchar_t buf[16];
	Tsprintf(buf, _T("\\\\.\\COM%d"), m_ComNo);	// COM�ԍ�10�ȏ�Ή�
	m_hFile = CreateFile(buf,
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0); 
	ENABLE_C4996//
#else
	char buf[16];
	FILE *fp;
	errno_t error;
	Tsprintf_s(buf, sizeof(buf) , "\\\\.\\COM%d", m_ComNo);	// COM�ԍ�10�ȏ�Ή�

	error = fopen_s(&fp, buf, "w");
	m_hFile = fp;
	if (error != 0) {	// fopen()
		return EC_ComOpen;	//	COM�I�[�v��NG
	}
#endif

	if (m_hFile == EC_FileOpenError){
		return ESTATEBIT_open;				//	COM�I�[�v��NG
	}

	m_State |= ESTATEBIT_open;

	return ECRC_ok;							//  ��������
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Com�I�[�v��������Ƀ{�[���[�g�Ȃǂ�ݒ肵�܂��B
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int GnrlCom::openAndSetParam()
{
	int ack;

	ack = open();
	if (ack < ECRC_ok)	return ack;		// ComOen NG

	ack = setBaudRate();
	if (ack < ECRC_ok)	return ack;		// �{�[���[�g NG

	ack = setTimeout();
	if (ack < ECRC_ok)	return ack;		// �^�C���A�E�g NG

	return ECRC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  �ʐM�{�[���[�g�ݒ�
//  �߂�l
//  �֐�����������ƁA0 �ȊO�̒l���Ԃ�܂��B
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int GnrlCom::setBaudRate()
{
	int Bps					= m_Bps;
	enum EStopBit StopBit	= m_StopBit;
	enum EParity Parity		= m_Parity;

	unsigned long ncomcfg;
	COMMCONFIG commconfig;
	BOOL	ack;

	ncomcfg = sizeof(commconfig);
	
	ack = GetCommConfig(m_hFile, &commconfig, &ncomcfg);
	if (ack == false){
		return ESTATEBIT_baudrate;
	}

	commconfig.dcb.DCBlength=	28;
	commconfig.dcb.BaudRate=	Bps;
	commconfig.dcb.fBinary=		1;

//	commconfig.dcb.fParity=	0;
//	commconfig.dcb.Parity=	0;
	switch( Parity )
	{
		case EPARITY_even :   // �����p���e�B�[
			commconfig.dcb.fParity           = TRUE;
			commconfig.dcb.Parity            = EVENPARITY;
			break;
		case EPARITY_odd :   // ��p���e�B�[
			commconfig.dcb.fParity           = TRUE;
			commconfig.dcb.Parity            = ODDPARITY;
			break;
		case EPARITY_mark :   // �}�[�N
			commconfig.dcb.fParity           = TRUE;
			commconfig.dcb.Parity            = MARKPARITY;
			break;
		case EPARITY_space :   // �X�y�[�X
			commconfig.dcb.fParity           = TRUE;
			commconfig.dcb.Parity            = SPACEPARITY;
			break;
		default :			 // �p���e�B�[�Ȃ�
			commconfig.dcb.fParity           = FALSE;
			commconfig.dcb.Parity            = NOPARITY;
			break;
	}

	switch(StopBit)
	{
	case ESTOPBIT_1:		//�X�g�b�v�r�b�g1
		commconfig.dcb.StopBits				= ONESTOPBIT;
		break;

	case ESTOPBIT_1_5:		//�X�g�b�v�r�b�g1.5
		commconfig.dcb.StopBits				= ONE5STOPBITS;
		break;

	case ESTOPBIT_2:		//�X�g�b�v�r�b�g2
	default:
		commconfig.dcb.StopBits				= TWOSTOPBITS;
		break;
	}

	commconfig.dcb.fOutxCtsFlow		= 0;	// ���̃����o�[��TRUE�ł���ꍇ�ACTS(clear-to-send)�̐M���͏o�̓t���[����̂��߂Ƀ��j�^�[����܂��B
											// ���̃����o�[��TRUE�ŁACTS��off�����ꍇ�ACTS���Ă�on�ɂȂ�܂ŁA�o�͂��ۗ�����܂��B

	commconfig.dcb.fOutxDsrFlow		= 0;	// ���̃����o�[��TRUE�ł���ꍇ�ADSR(data-set-read)�M���͏o�̓t���[����̂��߂Ƀ��j�^�[����܂��B
											// ���̃����o�[��TRUE�ŁADSR��off�����ꍇ�ADSR���Ă�on�ɂȂ�܂ŁA�o�͂��ۗ�����܂��B

	commconfig.dcb.fDtrControl		= DTR_CONTROL_ENABLE;// DTR(data-terminal-ready)�t���[����B���̃����o�[�͎��̒l��1�ł��肦�܂��B
	// Value					 Meaning 
	// DTR_CONTROL_DISABLE		 DTR���C�����g�p�s�ɂ��܂��B
	// DTR_CONTROL_ENABLE		 DTR���C�����\�ɂ��܂��B
	// DTR_CONTROL_HANDSHAKE	 DTR�n���h�V�F�[�N���\�ɂ��܂��B�n���h�V�F�[�N���\�ɂȂ�ꍇ�A�A�v���P�[�V������EscapeCommFunction�t�@���N�V�����̎g�p�ɂ�胉�C���𑀍삷�邱�Ƃ̓G���[�ł��B

	commconfig.dcb.fDsrSensitivity	= 0;	// ���̃����o�[��TRUE�ł���ꍇ�A�R�~���j�P�[�V�����E�h���C�o�[��DSR�M���̏�Ԃ����m�ł��܂��B
											// ����DSR���f�����̓��C�����n�C�łȂ���΁A�h���C�o�[�͎󂯎��ꂽ�C�ӂ̃o�C�g�𖳎����܂��B

	commconfig.dcb.fTXContinueOnXoff= 0;	// ���̃����o�[��TRUE�ł���ꍇ�A���̓o�b�t�@�[���\���Ȃ��Ƃ�XonLim�o�C�g���ɗ��āA�h���C�o�[���o�C�g���󂯎�邱�Ƃ���߂邽�߂�XoffChar�����𑗐M������A���M�͌p�����܂��B
											// ���̃����o�[��FALSE�ł���ꍇ�A���̓o�b�t�@�[����̂��Ƃ�XonLim�o�C�g���ɂ���A�h���C�o�[���󗝂��ĊJ���邽�߂�XoffChar�����𑗐M�����܂ŁA���M�͌p�����܂���B

	commconfig.dcb.fOutX			= 0;	// XON/XOFF�t���[���䂪���M�̊ԂɎg�p����邩�ǂ��������܂��B
											// ���̃����o�[��TRUE�ł���ꍇ�AXoffChar�������󂯎���AXonChar�������󂯎����ꍇ�A�ĂуX�^�[�g����ꍇ�A���M�͎~�܂�܂��B

	commconfig.dcb.fInX				= 0;	// XON/XOFF�t���[���䂪�󗝒��Ɏg�p����邩�ǂ��������܂��B
											// ���̃����o�[��TRUE�ł���ꍇ�A���̓o�b�t�@�[���\���Ȃ��Ƃ�XoffLim�o�C�g���ɗ���ꍇ�AXoffChar�����͑����܂��B�܂��A���̓o�b�t�@�[����̂��Ƃ�XonLim�o�C�g���ɗ���ꍇ�AXonChar�����������܂��B

	commconfig.dcb.fErrorChar		= 0;	// ErrorChar�����o�[�ɂ���Ďw�肳�ꂽ�����ƃp���e�B�[�E�G���[�Ŏ󂯎��ꂽ�o�C�g�����ւ����邩�ǂ��������܂��B���̃����o�[��TRUE�ŁAfParity�����o�[��TRUE�ł���ꍇ�A�u���͐����܂��B 
	commconfig.dcb.fNull			= 0;	// ���̃����o�[��TRUE�ł���ꍇ�A�󂯎��ꂽ���Anull�̃o�C�g���p������܂��B
	commconfig.dcb.fRtsControl		= RTS_CONTROL_DISABLE;	// RTS (request-to-send) flow control. This member can be one of the following values. 
	// Value				 Meaning
	// RTS_CONTROL_DISABLE	 RTS ���C�����g�p�s�ɂ��܂��B
	// RTS_CONTROL_ENABLE	 RTS ���C�����\�ɂ��܂��B
	// RTS_CONTROL_HANDSHAKE RTS�n���h�V�F�[�N���\�ɂ��܂��B�o�b�t�@�[��2����1�����ŁARTS���C�����グ�܂��A �l���̎O�̈ȏ��RTS���C����������B�A�v���P�[�V������ EscapeCommFunction�t�@���N�V�����̎g�p�ɂ�胉�C���𒼂����Ƃ̓G���[�ł��B
	// RTS_CONTROL_TOGGLE	 �o�C�g�����M�ɗ��p�\�ȏꍇRTS���C�����n�C�B�o�b�t�@�[��p�����o�C�g�����ׂđ���ꂽ��ARTS���C���̓��[�B
	// 						 �� Windows Me/98/95:  ���̒l�͎x������܂���B
 
	commconfig.dcb.fAbortOnError	= 0;	// ���̃����o�[��TRUE�ł���ꍇ�A�h���C�o�[�͂��ׂĂ��I�����܂��A�G���[���N����ꍇ�A�G���[�E�X�e�[�^�X���������I�y���[�V������ǂݏ����܂��B�A�v���P�[�V������ClearCommError�֐����ĂԂ��Ƃɂ��G���[�����F����܂ŁA�h���C�o�[�͂���ȏ�̃R�~���j�P�[�V�����E�I�y���[�V�������󂯎��Ȃ��ł��傤�B
	commconfig.dcb.fDummy2			= 0;	// �\��;�g�p���܂���B
	commconfig.dcb.wReserved		= 0;	// �\��;0�ł���ɈႢ����܂���B
	commconfig.dcb.XonLim			= 2048;	// �t���[����̑O�ɓ��̓o�b�t�@�[���ŋ����ꂽ�o�C�g�̍ŏ��̐��́A���M����ւ��邽�߂ɋN������܂��B
											// �t���[����M�����N�����ꂽ�㑗�M�킪�����𑗐M���邩������Ȃ����Ƃɒ��ӂ��Ă��������B���������āA���̒l��0�ł͂����Ă͂Ȃ�Ȃ��B
											// ����́AfInX�AfRtsControl���邢��fDtrControl�̒���XON/XOFF(RTS)���邢��DTR���̓t���[����̂����ꂩ���w�肳���Ɖ��肵�܂��B
	commconfig.dcb.XoffLim			= 512;	// �t���[����̑O�ɓ��̓o�b�t�@�[���ŋ����ꂽ�o�C�g�̍ő�̐��́A���M��ɂ�鑗�M�������邽�߂ɋN������܂��B
											// ����́AfInX�AfRtsControl���邢��fDtrControl�̒���XON/XOFF(RTS)���邢��DTR���̓t���[����̂����ꂩ���w�肳���Ɖ��肵�܂��B
											// �����ꂽ�o�C�g�̍ő�̐��͓��̓o�b�t�@�[�ɁA�o�C�g�ŁA�T�C�Y���炱�̒l���������Ƃɂ��v�Z����܂��B

	commconfig.dcb.ByteSize			= 8;	// �P�f�[�^�̃r�b�g�����w�肷��B
//	commconfig.dcb.StopBits=	2;			// �ݒ�ς�
	commconfig.dcb.XonChar			= 17;	// ���M����ю�M�̗�����XON�������w�肷��B
	//commconfig.dcb.XoffCha=	19;
	commconfig.dcb.ErrorChar		= 0;	// �p���e�B�[�E�G���[�̕������w�肷��B
//	commconfig.dcb.EofChar=	26;
	commconfig.dcb.EvtChar			= 0;	// �C�x���g�̕����i��M���j
	//commconfig.wReserved1= 0;
	commconfig.dwProviderSubType	= 1;
	commconfig.dwProviderOffset		= 0;
	commconfig.dwProviderSize		= 0;

	ack = SetCommConfig(m_hFile, &commconfig, ncomcfg);
	if (ack == false)	return ESTATEBIT_baudrate;

	m_State |= ESTATEBIT_baudrate;

	return	ECRC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  �^�C���A�E�g���Ԃ�ݒ肵�܂��B
//  �����o�ϐ�m_TimeOut(ms)�o�߂��Ă�������M���Ȃ��ꍇ�A���̊֐��𕉂��܂��B
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int GnrlCom::setTimeout()
{
	BOOL ack;
	COMMTIMEOUTS	cto;
	
	GetCommTimeouts(m_hFile, &cto );			// �^�C���A�E�g�̐ݒ��Ԃ��擾

	cto.ReadIntervalTimeout			= m_TimeOut;	// ReadTotalTimeoutMultiplier��ReadTotalTimeoutConstant��0��MAXWARD�Ȃ�readfile���������^�[������
	cto.ReadTotalTimeoutMultiplier	= 1			;	// 1Byte���̃^�C�}  m_TimeOut
	cto.ReadTotalTimeoutConstant	= m_TimeOut;	// �P�֐��R�[�����̃^�C�}
	cto.WriteTotalTimeoutMultiplier = m_TimeOut;	// 1Byte���̃^�C�}
	cto.WriteTotalTimeoutConstant	= m_TimeOut;	// �P�֐��R�[�����̃^�C�}

	ack = SetCommTimeouts(m_hFile, &cto);			// �^�C���A�E�g�̏�Ԃ�ݒ�
	if (ack == false)	return ESTATEBIT_timeout;

	m_State |= ESTATEBIT_timeout;

	return ECRC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  ��薳�����0�ȏ�̂���������߂�
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int GnrlCom::close()
{
	setBad();

	if(m_hFile != EC_FileOpenError){
		CloseHandle(m_hFile);
		m_hFile = EC_FileOpenError;
	}

	return ECRC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  �ǂݍ��񂾃o�C�g����߂�
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int GnrlCom::read(unsigned char *Data, int Size) const
{
	unsigned long	len;

	if (isBad())	return EC_ComOpen;

	if(ReadFile(m_hFile, Data, Size, &len, 0 ) != 0)
		return len;

	return EC_Read;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  �������񂾃o�C�g����߂�
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int GnrlCom::write(const unsigned char *Data, int Size) const
{
	unsigned long	len;

	if (isBad())	return EC_ComOpen;

	if(WriteFile( m_hFile, Data, Size, &len, NULL) != 0)
		return len;						// �ǂݍ��߂��������e�L�X�g�t�@�C���ɏ�������
	
	return EC_Write;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  �����o�[�̏�����  clear()���������ȁH
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void GnrlCom::init()
{
	close();
	m_hFile = EC_FileOpenError;
}
void GnrlCom::preset()
{
	m_PortNo	= 1;
	m_BaudRate	= 9600;					// ���x				
	m_TimeOut	= 1000;					// �^�C���A�E�g		
	m_StopBit	= ESTOPBIT_1;			// �X�g�b�v�r�b�g	
	m_Parity	= EPARITY_no;			// �p���e�B
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  Com�̃o�b�t�@���p�[(�N���A)����
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int GnrlCom::clearBuffer() const
{
	if(m_hFile != NULL){
		return PurgeComm(m_hFile, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
	}
	return EC_ComOpen;					// ����M�ł��Ȃ��̂ŃG���[�Ƃ���
}

