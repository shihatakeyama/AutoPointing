// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// GnrlCom.cpp
//
// COMポートアクセスの為のクラス
// デバグ入出力用なので、移植性が無い構成とした。
//
// Visual studio　で使用する場合、
// [プロパティ] >[C / C++] >[プリプロセッサ]に下記を定義
//  _CRT_SECURE_NO_WARNINGS
//
// 設定値は下記のページを参照した。
//  http://homepage2.nifty.com/meiwadensokuki/dcb.html
//
// use: C++,WIN32API
// Version 1.1
//
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 
// 履歴
//   2012/12/31  スペースパリティを追加
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


const int GnrlCom::EC_ComOpen	= -1;				// 通信ポートがオープンしていない
const int GnrlCom::EC_Read		= -2;				// 相手から受信できなかった場合のエラーコード
const int GnrlCom::EC_Write		= -3;				// 相手から受信できなかった場合のエラーコード
void* const GnrlCom::EC_FileOpenError = (void*)0xFFFFFFFF;	// COMポートオープン失敗した場合のエラーコード

const int GnrlCom::m_BaudRateList[EB_num] = {
	300, 1200, 4800, 9600, 19200, 38400, 115200, 230400
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// sectionName	セクション名
// KeyName		キー(=の左側)名
// Val			値(=の右側)
// FilePath		ファイルパス
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
static BOOL WritePrivateProfileInt(LPCTSTR sectionName, LPCTSTR KeyName, int Val, LPCTSTR FilePath)
{
	TCHAR	str[32];

	Tsprintf_s(str, 32, _T("%d"), Val);
	//	_itow(Val, tmp, 10);

	return ::WritePrivateProfileString(sectionName, KeyName, str, FilePath);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  MFC GUI 関連
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#ifdef USE_MFC
// GUI → 数値 変換
int GnrlCom::fromGuiBaudRate(const CComboBox &BaudRate)
{
	return m_BaudRateList[BaudRate.GetCurSel()];
}
int GnrlCom::fromGuiDataBit(const CComboBox &Data)
{
	return 0;	// 8固定
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
	return 0;	// None固定
}

// GUI → 自クラスへ
int GnrlCom::getGuiBaudRate(const CComboBox &BaudRate)
{
	return m_BaudRate = fromGuiBaudRate(BaudRate);
}
int GnrlCom::getGuiDataBit(const CComboBox &Data)
{
	return 0;	// 8固定
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
	return 0;	// None固定
}

// 自クラス　→ GUI へ
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
// XML 読み書き
// load()/save()ともにNodeは自ノードを設定して下さい。
// save()時に返すNodeは、タグ名が設定されていませんので、呼び元で名前つけて下さい。
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
// iniファイルから通信設定を読み込む.
// 引数 IniFile 設定ファイルが記述されたiniファイルの相対パス(絶対パスは不可)
//      Section セクション名
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
// 設定ファイルなどの同一ディレクトリ内に存在するIniFileNameファイルの絶対パスを戻す
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int GnrlComgetFinePath(TCHAR *OutFilePath,int OutBufSize , const TCHAR *IniFileName)
{
	const TCHAR	*pc;
	TCHAR *cpc;
	int		ret;

	pc = Tstrrchr(IniFileName, '\\');
	if(pc == NULL)
	{
//		strcpy(OutFilePath,__argv[0]);	// コマンド引数から自プログラム位置を取得
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
// 通信設定をiniファイルから読み込み
//   IniFile  設定ファイルが記述されたiniファイルの相対パス(絶対パスは不可)
//   Section  iniファイルのセクション
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
// 通信設定を 設定/取得
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void GnrlCom::putParameter(int ComNum, int Bps, int TimeOut, enum EStopBit StopBit, enum EParity Parity)
{
	m_ComNo		= ComNum;
	m_Bps		= Bps;			// 速度	
	m_TimeOut	= TimeOut;		// タイムアウト	
	m_StopBit	= StopBit;		// ストップビット
	m_Parity	= Parity;		// パリティ	
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
	ComNo	= m_ComNo;			// Comの番号
	Bps		= m_Bps;			// 速度		
	TimeOut	= m_TimeOut;		// タイムアウト
	StopBit = m_StopBit;		// ストップビット
	Parity	= m_Parity;			// パリティ
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
// PCのCOMポートをオープンする
//  戻り値 == 0 成功
//         < 0 失敗
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int GnrlCom::open()
{
	close();	// オープンしていても指示されたのだからオープンをやり直す。

#if 1
	DISABLE_C4996//
	wchar_t buf[16];
	Tsprintf(buf, _T("\\\\.\\COM%d"), m_ComNo);	// COM番号10以上対応
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
	Tsprintf_s(buf, sizeof(buf) , "\\\\.\\COM%d", m_ComNo);	// COM番号10以上対応

	error = fopen_s(&fp, buf, "w");
	m_hFile = fp;
	if (error != 0) {	// fopen()
		return EC_ComOpen;	//	COMオープンNG
	}
#endif

	if (m_hFile == EC_FileOpenError){
		return ESTATEBIT_open;				//	COMオープンNG
	}

	m_State |= ESTATEBIT_open;

	return ECRC_ok;							//  処理成功
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Comオープンした後にボーレートなどを設定します。
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int GnrlCom::openAndSetParam()
{
	int ack;

	ack = open();
	if (ack < ECRC_ok)	return ack;		// ComOen NG

	ack = setBaudRate();
	if (ack < ECRC_ok)	return ack;		// ボーレート NG

	ack = setTimeout();
	if (ack < ECRC_ok)	return ack;		// タイムアウト NG

	return ECRC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  通信ボーレート設定
//  戻り値
//  関数が成功すると、0 以外の値が返ります。
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
		case EPARITY_even :   // 偶数パリティー
			commconfig.dcb.fParity           = TRUE;
			commconfig.dcb.Parity            = EVENPARITY;
			break;
		case EPARITY_odd :   // 奇数パリティー
			commconfig.dcb.fParity           = TRUE;
			commconfig.dcb.Parity            = ODDPARITY;
			break;
		case EPARITY_mark :   // マーク
			commconfig.dcb.fParity           = TRUE;
			commconfig.dcb.Parity            = MARKPARITY;
			break;
		case EPARITY_space :   // スペース
			commconfig.dcb.fParity           = TRUE;
			commconfig.dcb.Parity            = SPACEPARITY;
			break;
		default :			 // パリティーなし
			commconfig.dcb.fParity           = FALSE;
			commconfig.dcb.Parity            = NOPARITY;
			break;
	}

	switch(StopBit)
	{
	case ESTOPBIT_1:		//ストップビット1
		commconfig.dcb.StopBits				= ONESTOPBIT;
		break;

	case ESTOPBIT_1_5:		//ストップビット1.5
		commconfig.dcb.StopBits				= ONE5STOPBITS;
		break;

	case ESTOPBIT_2:		//ストップビット2
	default:
		commconfig.dcb.StopBits				= TWOSTOPBITS;
		break;
	}

	commconfig.dcb.fOutxCtsFlow		= 0;	// このメンバーがTRUEである場合、CTS(clear-to-send)の信号は出力フロー制御のためにモニターされます。
											// このメンバーがTRUEで、CTSがoffされる場合、CTSが再びonになるまで、出力が保留されます。

	commconfig.dcb.fOutxDsrFlow		= 0;	// このメンバーがTRUEである場合、DSR(data-set-read)信号は出力フロー制御のためにモニターされます。
											// このメンバーがTRUEで、DSRがoffせれる場合、DSRが再びonになるまで、出力が保留されます。

	commconfig.dcb.fDtrControl		= DTR_CONTROL_ENABLE;// DTR(data-terminal-ready)フロー制御。このメンバーは次の値の1つでありえます。
	// Value					 Meaning 
	// DTR_CONTROL_DISABLE		 DTRラインを使用不可にします。
	// DTR_CONTROL_ENABLE		 DTRラインを可能にします。
	// DTR_CONTROL_HANDSHAKE	 DTRハンドシェークを可能にします。ハンドシェークが可能になる場合、アプリケーションがEscapeCommFunctionファンクションの使用によりラインを操作することはエラーです。

	commconfig.dcb.fDsrSensitivity	= 0;	// このメンバーがTRUEである場合、コミュニケーション・ドライバーはDSR信号の状態を感知できます。
											// もしDSRモデム入力ラインがハイでなければ、ドライバーは受け取られた任意のバイトを無視します。

	commconfig.dcb.fTXContinueOnXoff= 0;	// このメンバーがTRUEである場合、入力バッファーが十分なことのXonLimバイト内に来て、ドライバーがバイトを受け取ることをやめるためにXoffChar文字を送信した後、送信は継続します。
											// このメンバーがFALSEである場合、入力バッファーが空のことのXonLimバイト内にあり、ドライバーが受理を再開するためにXoffChar文字を送信したまで、送信は継続しません。

	commconfig.dcb.fOutX			= 0;	// XON/XOFFフロー制御が送信の間に使用されるかどうか示します。
											// このメンバーがTRUEである場合、XoffChar文字が受け取られ、XonChar文字が受け取られる場合、再びスタートする場合、送信は止まります。

	commconfig.dcb.fInX				= 0;	// XON/XOFFフロー制御が受理中に使用されるかどうか示します。
											// このメンバーがTRUEである場合、入力バッファーが十分なことのXoffLimバイト内に来る場合、XoffChar文字は送られます。また、入力バッファーが空のことのXonLimバイト内に来る場合、XonChar文字が送られます。

	commconfig.dcb.fErrorChar		= 0;	// ErrorCharメンバーによって指定された文字とパリティー・エラーで受け取られたバイトが取り替えられるかどうか示します。このメンバーがTRUEで、fParityメンバーがTRUEである場合、置換は生じます。 
	commconfig.dcb.fNull			= 0;	// このメンバーがTRUEである場合、受け取られた時、nullのバイトが廃棄されます。
	commconfig.dcb.fRtsControl		= RTS_CONTROL_DISABLE;	// RTS (request-to-send) flow control. This member can be one of the following values. 
	// Value				 Meaning
	// RTS_CONTROL_DISABLE	 RTS ラインを使用不可にします。
	// RTS_CONTROL_ENABLE	 RTS ラインを可能にします。
	// RTS_CONTROL_HANDSHAKE RTSハンドシェークを可能にします。バッファーが2分の1未満で、RTSラインを上げます、 四分の三の以上でRTSラインを下げる。アプリケーションが EscapeCommFunctionファンクションの使用によりラインを直すことはエラーです。
	// RTS_CONTROL_TOGGLE	 バイトが送信に利用可能な場合RTSラインがハイ。バッファーを用いたバイトがすべて送られた後、RTSラインはロー。
	// 						 ※ Windows Me/98/95:  この値は支援されません。
 
	commconfig.dcb.fAbortOnError	= 0;	// このメンバーがTRUEである場合、ドライバーはすべてを終了します、エラーが起こる場合、エラー・ステータスを持ったオペレーションを読み書きます。アプリケーションがClearCommError関数を呼ぶことによりエラーを承認するまで、ドライバーはそれ以上のコミュニケーション・オペレーションを受け取らないでしょう。
	commconfig.dcb.fDummy2			= 0;	// 予約;使用しません。
	commconfig.dcb.wReserved		= 0;	// 予約;0であるに違いありません。
	commconfig.dcb.XonLim			= 2048;	// フロー制御の前に入力バッファー中で許可されたバイトの最小の数は、送信器を禁じるために起動されます。
											// フロー制御信号が起動された後送信器が文字を送信するかもしれないことに注意してください。したがって、この値は0ではあってはならない。
											// これは、fInX、fRtsControlあるいはfDtrControlの中でXON/XOFF(RTS)あるいはDTR入力フロー制御のいずれかが指定されると仮定します。
	commconfig.dcb.XoffLim			= 512;	// フロー制御の前に入力バッファー中で許可されたバイトの最大の数は、送信器による送信を許可するために起動されます。
											// これは、fInX、fRtsControlあるいはfDtrControlの中でXON/XOFF(RTS)あるいはDTR入力フロー制御のいずれかが指定されると仮定します。
											// 許可されたバイトの最大の数は入力バッファーに、バイトで、サイズからこの値を引くことにより計算されます。

	commconfig.dcb.ByteSize			= 8;	// １データのビット数を指定する。
//	commconfig.dcb.StopBits=	2;			// 設定済み
	commconfig.dcb.XonChar			= 17;	// 送信および受信の両方のXON文字を指定する。
	//commconfig.dcb.XoffCha=	19;
	commconfig.dcb.ErrorChar		= 0;	// パリティー・エラーの文字を指定する。
//	commconfig.dcb.EofChar=	26;
	commconfig.dcb.EvtChar			= 0;	// イベントの文字（受信時）
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
//  タイムアウト時間を設定します。
//  メンバ変数m_TimeOut(ms)経過しても何も受信しない場合、この関数を負えます。
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int GnrlCom::setTimeout()
{
	BOOL ack;
	COMMTIMEOUTS	cto;
	
	GetCommTimeouts(m_hFile, &cto );			// タイムアウトの設定状態を取得

	cto.ReadIntervalTimeout			= m_TimeOut;	// ReadTotalTimeoutMultiplierとReadTotalTimeoutConstantが0でMAXWARDならreadfileが即時リターンする
	cto.ReadTotalTimeoutMultiplier	= 1			;	// 1Byte毎のタイマ  m_TimeOut
	cto.ReadTotalTimeoutConstant	= m_TimeOut;	// １関数コール毎のタイマ
	cto.WriteTotalTimeoutMultiplier = m_TimeOut;	// 1Byte毎のタイマ
	cto.WriteTotalTimeoutConstant	= m_TimeOut;	// １関数コール毎のタイマ

	ack = SetCommTimeouts(m_hFile, &cto);			// タイムアウトの状態を設定
	if (ack == false)	return ESTATEBIT_timeout;

	m_State |= ESTATEBIT_timeout;

	return ECRC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  問題無ければ0以上のあいたいを戻す
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
//  読み込んだバイト数を戻す
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
//  書き込んだバイト数を戻す
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int GnrlCom::write(const unsigned char *Data, int Size) const
{
	unsigned long	len;

	if (isBad())	return EC_ComOpen;

	if(WriteFile( m_hFile, Data, Size, &len, NULL) != 0)
		return len;						// 読み込めた分だけテキストファイルに書き込む
	
	return EC_Write;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  メンバーの初期化  clear()がいいかな？
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void GnrlCom::init()
{
	close();
	m_hFile = EC_FileOpenError;
}
void GnrlCom::preset()
{
	m_PortNo	= 1;
	m_BaudRate	= 9600;					// 速度				
	m_TimeOut	= 1000;					// タイムアウト		
	m_StopBit	= ESTOPBIT_1;			// ストップビット	
	m_Parity	= EPARITY_no;			// パリティ
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  Comのバッファをパー(クリア)する
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int GnrlCom::clearBuffer() const
{
	if(m_hFile != NULL){
		return PurgeComm(m_hFile, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
	}
	return EC_ComOpen;					// 送受信できないのでエラーとする
}

