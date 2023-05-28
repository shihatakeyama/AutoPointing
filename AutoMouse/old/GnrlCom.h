// ****************************************************************************
// GnrlCom.h
//
// COMポートアクセスの為のクラス
// デバグ入出力用なので、移植性が無い構成とした。
//
// use: C++,WIN32API
//
// 2018/12/09	isOpened() 追加 
//
// ****************************************************************************

#ifndef GNRLCOM_H
#define GNRLCOM_H

class GnrlCom
{
public:
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
		ESTATEBIT_open		= 1,
		ESTATEBIT_baudrate	= 2,
		ESTATEBIT_timeout	= 4,
		ESTATEBIT_good		= 7
	};

	enum E_ReturnCode {
		ERC_ok			=  0,
		ERC_open		= -1,
		ERC_baudrate	= -2,
		ERC_timeout		= -3,
		ERC_ng			= -5,
		ERC_othererr	= -99
	};

	GnrlCom();
//	GnrlCom(const char *IniFile, const char *Section);	/* IniFile：通信の設定ファイル名、Section：セクション名	 */

	~GnrlCom();

	void putParameter(const char *IniFile, const char *Section);		// 通信設定
	void putParameter(int ComNum, int Bps, int TimeOut, enum EStopBit StopBit, enum EParity Parity);
	void getParameter(int &ComNum, int &Bps, int &TimeOut, enum EStopBit &StopBit, enum EParity &Parity) const;

	int open();
	int openAndSetParam();
	int close();
	int isOpened() const			{ return	m_hFile != EC_FileOpenError;	}
	void setBad()					{ m_State = ESTATEBIT_clear;				}
	void setGood()					{ m_State = ESTATEBIT_good;					}
	bool isBad() const				{ return	m_State != ESTATEBIT_good;		}
	bool isGood() const				{ return	m_State == ESTATEBIT_good;		}

	int clearBuffer() const;								// Comのバッファをパーする
	int read(unsigned char *Data, int Size) const;			// 読み込み					 
	int write(const unsigned char *Data, int Size) const ;	// Comへ書き込み
	const HANDLE getHandle()const	{ return m_hFile;							}			// ファイルポインタの取得
	int setBaudRate(/*int Bps, enum EStopBit StopBit, enum EParity Parity*/);

private:
//--public:	//----
	void init();						// メンバーの初期化
	int setTimeout(unsigned int TimeOut);


	HANDLE	m_hFile; 					// ファイルハンドル
//	bool	m_Good;						// ボーレートなどを設定しないままread()してしまうとフリーズしてしまうため。
	unsigned int	m_State;
	static const int EC_ComOpen;
	static const int EC_Read;
	static const int EC_Write;
	static void* const EC_FileOpenError;
	int		m_Bps;						/* 速度				 */
	int		m_ComNo;
	int		m_TimeOut;					/* タイムアウト		 */
	enum EStopBit m_StopBit;			/* ストップビット	 */
	enum EParity m_Parity;				/* パリティ			 */
};

#endif //GMRLCOM