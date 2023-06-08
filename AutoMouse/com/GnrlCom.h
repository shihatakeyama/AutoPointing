// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// GnrlCom.h
//
// COM�|�[�g�A�N�Z�X�ׂ̈̃N���X
// �f�o�O���o�͗p�Ȃ̂ŁA�ڐA���������\���Ƃ����B
//
// use: C++,WIN32API
//
// 2018/12/09	isOpened() �ǉ� 
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
		EPARITY_even	= 1,	// ����		 
		EPARITY_odd		= 2,	// �		 
		EPARITY_mark	= 3,	// �}�[�N	 
		EPARITY_space	= 4		// �X�y�[�X
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

	// �e�탂�W���[���̃��^�[���R�[�h
	enum E_ComReturnCode{
		  ECRC_ok		= 0
		, ECRC_ng		= -101
		, ECRC_open		= -102
		, ECRC_baudrate	= -105
		, ECRC_timeout	= -106
	};

#ifdef USE_MFC
	int fromGuiBaudRate(const CComboBox &BaudRate);
	int fromGuiDataBit(const CComboBox &Data);		// 8 �Œ�ŗǂ��H
	enum EParity fromGuiParity(const CComboBox &Parity);
	enum EStopBit fromGuiStopBit(const CComboBox &StopBit);
	int fromGuiFrow(const CComboBox &Frow);			// Floaw �����ł悢�H

	int getGuiBaudRate(const CComboBox &BaudRate);
	int getGuiDataBit(const CComboBox &Data);
	int getGuiParity(const CComboBox &Parity);
	int getGuiStopBit(const CComboBox &StopBit);
	int getGuiFrow(const CComboBox &Frow);

	int setGuiBaudRate(CComboBox &BaudRate) const;
	int setGuiData(CComboBox &Data) const;			// 8 �Œ�ŗǂ��H
	int setGuiParity(CComboBox &Parity) const;
	int setGuiStopBit(CComboBox &StopBit) const;
	int setGuiFrow(CComboBox &Frow) const;			// Floaw �����ł悢�H
#endif

#ifdef USE_XML	// XML����ւ̃A�N�Z�X
	int32_t loadXmlNode(const rapidxml::node_t *Node);
	int32_t saveXmlNode(rapidxml::document_t &Doc ,rapidxml::node_t *&Node) const;
#endif

	GnrlCom();
	GnrlCom(const TCHAR *IniFile, const TCHAR *Section);

	~GnrlCom();

	// �ʐM�ݒ�
	void loadParameter(const TCHAR *IniFile = _T("ComSet.ini"), const TCHAR *Section = _T("ComSet"));	// �� putParameter()
	void saveParameter(const TCHAR *IniFile = _T("ComSet.ini"), const TCHAR *Section = _T("ComSet"));

	void putParameter(int PortNo, int Bps, int TimeOut, enum EStopBit StopBit, enum EParity Parity);
	void getParameter(int &PortNo, int &Bps, int &TimeOut, enum EStopBit &StopBit, enum EParity &Parity) const;

	// �{�[���[�g(���xbps)�������ɕϊ�
	static int bpsToEbps(int Bps);
	static int ebpsToBps(int EBps);

	int open();
	int openAndSetParam();
	int close();
	bool isLoad() const				{ return 	m_State & ESTATEBIT_param; }	// �����ǂݐ����H	���������̏ꍇ��FALSE��Ԃ��܂��B
	bool isOpened() const			{ return	m_hFile != EC_FileOpenError;	}
	void setBad();
	void setGood()					{ m_State = ESTATEBIT_good;					}
	bool isBad() const				{ return	m_State != ESTATEBIT_good;		}
	bool isGood() const				{ return	m_State == ESTATEBIT_good;		}

	// �f�[�^�ʐM
	int clearBuffer() const;									// Com�̃o�b�t�@���p�[����
	int read(unsigned char *Data, int Size) const;				// �ǂݍ���
	int write(const unsigned char *Data, int Size) const ;		// ��������
	const HANDLE getHandle()const {return m_hFile;}				// �t�@�C���|�C���^�̎擾
	int setBaudRate();

	// �����o�A�N�Z�X
	int getComNo() const		{ return m_ComNo; }
	void putComNo(int Portno)	{ m_ComNo = Portno; }

private:
//--public:	//----
	void init();						// �����o�[�̏�����
	void preset();						// �����ݒ�
	int setTimeout();

	HANDLE	m_hFile; 					// �t�@�C���n���h��	
	int		m_ComNo;					// COM�ԍ�
	int		m_BaudRate;					// ���x				
	int		m_Bps;						// ���x	
	enum EParity m_Parity;				// �p���e�B
	enum EStopBit m_StopBit;			// �X�g�b�v�r�b�g
	int		m_TimeOut;					// �^�C���A�E�g
	unsigned int	m_State;

	// �������`
	const static TCHAR	*m_PortNoText;
	const static TCHAR	*m_BaudRateText;
	const static TCHAR	*m_TimeoutText;
	const static TCHAR	*m_StopbitText;
	const static TCHAR	*m_ParityText;

	// �萔��` 
	const static int EC_ComOpen;
	const static int EC_Read;
	const static int EC_Write;
	static void* const EC_FileOpenError;
	const static int m_BaudRateList[EB_num];
//--	int				 m_ComNo;
};

#endif //GMRLCOM