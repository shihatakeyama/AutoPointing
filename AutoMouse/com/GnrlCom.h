// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// GnrlCom.h
//
// COMポートアクセスの為のクラス
// デバグ入出力用なので、移植性が無い構成とした。
//
// use: C++,WIN32API
//
// 2018/12/09	isOpened() 追加 
//
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#ifndef GNRLCOM_H
#define GNRLCOM_H

class GnrlCom
{
public:

	enum EBps{
		  EB_300bps		= 0
		, EB_1200bps	= 1
		, EB_4800bps	= 2
		, EB_9600bps	= 3
		, EB_19200bps	= 4
		, EB_38400bps	= 5
		, EB_115200bps	= 6
		, EB_230400bps	= 7
		, EB_num		= 8
	};

	enum EParity {
		EPARITY_no		= 0,
		EPARITY_even	= 1,	// 偶数		 
		EPARITY_odd		= 2,	// 奇数		 
		EPARITY_mark	= 3,	// マーク	 
		EPARITY_space	= 4		// スペース
	};

	enum EStopBit {
		ESTOPBIT_1		= 0,	// 1
		ESTOPBIT_1_5	= 1,	// 1.5
		ESTOPBIT_2		= 2		// 2
	};

	enum EStateBit{
		ESTATEBIT_clear		= 0,
		ESTATEBIT_param		= 1,
		ESTATEBIT_open		= 2,
		ESTATEBIT_baudrate	= 4,
		ESTATEBIT_timeout	= 8,
		ESTATEBIT_good		= 15
	};

	// 各種モジュールのリターンコード
	enum E_ComReturnCode{
		  ECRC_ok		= 0
		, ECRC_ng		= -101
		, ECRC_open		= -102
		, ECRC_baudrate	= -105
		, ECRC_timeout	= -106
	};

#ifdef USE_MFC
	int fromGuiBaudRate(const CComboBox &BaudRate);
	int fromGuiDataBit(const CComboBox &Data);		// 8 固定で良い？
	enum EParity fromGuiParity(const CComboBox &Parity);
	enum EStopBit fromGuiStopBit(const CComboBox &StopBit);
	int fromGuiFrow(const CComboBox &Frow);			// Floaw 無しでよい？

	int getGuiBaudRate(const CComboBox &BaudRate);
	int getGuiDataBit(const CComboBox &Data);
	int getGuiParity(const CComboBox &Parity);
	int getGuiStopBit(const CComboBox &StopBit);
	int getGuiFrow(const CComboBox &Frow);

	int setGuiBaudRate(CComboBox &BaudRate) const;
	int setGuiData(CComboBox &Data) const;			// 8 固定で良い？
	int setGuiParity(CComboBox &Parity) const;
	int setGuiStopBit(CComboBox &StopBit) const;
	int setGuiFrow(CComboBox &Frow) const;			// Floaw 無しでよい？
#endif

#ifdef USE_XML	// XMLからへのアクセス
	int32_t loadXmlNode(const rapidxml::node_t *Node);
	int32_t saveXmlNode(rapidxml::document_t &Doc ,rapidxml::node_t *&Node) const;
#endif

	GnrlCom();
	GnrlCom(const TCHAR *IniFile, const TCHAR *Section);

	~GnrlCom();

	// 通信設定
	void loadParameter(const TCHAR *IniFile = _T("ComSet.ini"), const TCHAR *Section = _T("ComSet"));	// 旧 putParameter()
	void saveParameter(const TCHAR *IniFile = _T("ComSet.ini"), const TCHAR *Section = _T("ComSet"));

	void putParameter(int PortNo, int BaudRate, int TimeOut, enum EStopBit StopBit, enum EParity Parity);
	void getParameter(int &PortNo, int &BaudRate, int &TimeOut, enum EStopBit &StopBit, enum EParity &Parity) const;

	// ボーレート(速度bps)を序数に変換
	static int bpsToEbps(int Bps);
	static int ebpsToBps(int EBps);

	int open();
	int openAndSetParam();
	int close();
	bool isLoad() const				{ return 	m_State & ESTATEBIT_param; }	// 条件読み成功？	初期条件の場合はFALSEを返します。
	bool isOpened() const			{ return	m_hFile != EC_FileOpenError;	}
	void setBad();
	void setGood()					{ m_State = ESTATEBIT_good;					}
	bool isBad() const				{ return	m_State != ESTATEBIT_good;		}
	bool isGood() const				{ return	m_State == ESTATEBIT_good;		}

	// データ通信
	int clearBuffer() const;									// Comのバッファをパーする
	int read(unsigned char *Data, int Size) const;				// 読み込み
	int write(const unsigned char *Data, int Size) const ;		// 書き込み
	const HANDLE getHandle()const {return m_hFile;}				// ファイルポインタの取得
	int setBaudRate();

	// メンバアクセス
	int getComNo() const		{ return m_ComNo; }
	void putComNo(int Portno)	{ m_ComNo = Portno; }

private:
//--public:	//----
	void init();						// メンバーの初期化
	void preset();						// 初期設定
	int setTimeout();

	HANDLE			m_hFile; 			// ファイルハンドル	
	int				m_ComNo;			// COM番号
	int				m_BaudRate;			// 速度				
//--	int				m_Bps;				// 速度	
	enum EParity	m_Parity;			// パリティ
	enum EStopBit	m_StopBit;			// ストップビット
	int				m_TimeOut;			// タイムアウト
	unsigned int	m_State;

	// 文字列定義
	const static TCHAR	*m_PortNoText;
	const static TCHAR	*m_BaudRateText;
	const static TCHAR	*m_TimeoutText;
	const static TCHAR	*m_StopbitText;
	const static TCHAR	*m_ParityText;

	// 定数定義 
	const static int	EC_ComOpen;
	const static int	EC_Read;
	const static int	EC_Write;
	static void* const	EC_FileOpenError;
	const static int	m_BaudRateList[EB_num];
};

#endif //GMRLCOM