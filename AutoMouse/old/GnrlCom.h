// ****************************************************************************
// GnrlCom.h
//
// COM�|�[�g�A�N�Z�X�ׂ̈̃N���X
// �f�o�O���o�͗p�Ȃ̂ŁA�ڐA���������\���Ƃ����B
//
// use: C++,WIN32API
//
// 2018/12/09	isOpened() �ǉ� 
//
// ****************************************************************************

#ifndef GNRLCOM_H
#define GNRLCOM_H

class GnrlCom
{
public:
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
//	GnrlCom(const char *IniFile, const char *Section);	/* IniFile�F�ʐM�̐ݒ�t�@�C�����ASection�F�Z�N�V������	 */

	~GnrlCom();

	void putParameter(const char *IniFile, const char *Section);		// �ʐM�ݒ�
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

	int clearBuffer() const;								// Com�̃o�b�t�@���p�[����
	int read(unsigned char *Data, int Size) const;			// �ǂݍ���					 
	int write(const unsigned char *Data, int Size) const ;	// Com�֏�������
	const HANDLE getHandle()const	{ return m_hFile;							}			// �t�@�C���|�C���^�̎擾
	int setBaudRate(/*int Bps, enum EStopBit StopBit, enum EParity Parity*/);

private:
//--public:	//----
	void init();						// �����o�[�̏�����
	int setTimeout(unsigned int TimeOut);


	HANDLE	m_hFile; 					// �t�@�C���n���h��
//	bool	m_Good;						// �{�[���[�g�Ȃǂ�ݒ肵�Ȃ��܂�read()���Ă��܂��ƃt���[�Y���Ă��܂����߁B
	unsigned int	m_State;
	static const int EC_ComOpen;
	static const int EC_Read;
	static const int EC_Write;
	static void* const EC_FileOpenError;
	int		m_Bps;						/* ���x				 */
	int		m_ComNo;
	int		m_TimeOut;					/* �^�C���A�E�g		 */
	enum EStopBit m_StopBit;			/* �X�g�b�v�r�b�g	 */
	enum EParity m_Parity;				/* �p���e�B			 */
};

#endif //GMRLCOM