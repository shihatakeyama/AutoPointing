// ****************************************************************************
// �X���b�h�쐬�A�Ǘ��A�I������
//
// GnrlThread.h
// 
// ****************************************************************************

#ifndef GENERALTHREAD_H
#define GENERALTHREAD_H


class GnrlThread
{
public:

	// ���[�J�[�X���b�h�̏��
	enum E_WorkState
	{
		EWS_signal			= 0,	// �ʏ탏�[�N
		EWS_non_signal		= 1,	// �ꎞ��~	waitfor()�Ȃǂő҂�����Ă�A
		EWS_stop			= 2		// ���~, ��~,�p��
	};

	GnrlThread();
	GnrlThread(void *lpStartAddress);
	~GnrlThread();

	void*	beginThread(void *StartAddless, void *Param, bool Suspend);	// �@ �X���b�h�N��
	int32_t	resumeThread()	{return ResumeThread(m_hThread);}	// �@�f�ăX�^�[�g �֐�����������ƁA�֐��Ăяo���O�̃T�X�y���h�J�E���g���Ԃ�܂��B�֐������s����ƁA0xFFFFFFFF ���Ԃ�܂��B
	void	endThread();								// �A���[�J�[�X���b�h�֏I�����˗�����B
	bool	isLife() const		{ return m_LIFE; }		// �X���b�h�I����Ԏ擾
	void	setLife(bool Life)	{ m_LIFE = Life; }		// Life���Z�b�g
	bool	isEndThread(uint32_t dwMilliseconds = 10000);	// �B���[�J�[�X���b�h���I���������m�F 
	void*	setStartAddress(void* lpStartAddress);
//--	virtual Sint32 VRoutine();	// �p����̃N���X�ŁA���[�`�����`����ꍇ�̃����o�֐�		 

private:
	bool	m_LIFE;					// FALS�ɂȂ�����A�����Ƀ��[�v���I�����A���[�J�[�X���b�h���I�������ĉ������B 
//	static	void s_callRoutine(void* Arg);	// BeginThread()���ĂԈׂ̊֐�	 
	void*	m_lpStartAddress;		// s_callRoutine()�́A���[�J�X���b�h�̐擪�A�h���X��ݒ肷��֐� 

	uint32_t	m_ThreadID;				//	m_lpStartAddress = (LPTHREAD_START_ROUTINE)&CAboutDlg::test;�@��staic�錾�̊֐�
	void*	m_hThread;
};

#endif //GENERALTHREAD

// ---------------------- GeneralThread / Mutex -------------------------------

// �~���[�e�N�X���b�N
// ���̃N���X�̓V���O���^�X�N�̂ݗL���ł�
//  GnrlMutex.h
//  C++
 	 
#ifndef GENERALMUTEX_H
#define GENERALMUTEX_H

class GnrlMutex
{
public:

	GnrlMutex();
	~GnrlMutex();

	//�@���ݏ������Ă���n���h����߂��܂��B
	//�@�߂�l�@==NUNN �n���h���擾���s�܂��͖��擾

#ifdef HAVE_CXX_VOLATILE
	volatile
#endif
	const void *getHandle() const
	{
		return theMutex;
	}

	int32_t lock();		// ���������0��Ԃ��@�ȊO�ُ͈� 
	int32_t trylock();	// ���������0��Ԃ��@�ȊO�ُ͈� 
	int32_t unlock();	// ���������0��Ԃ��@�ȊO�ُ͈� 

	static const int32_t m_Busy;

private:
	GnrlMutex(const GnrlMutex& arg);
//	GnrlMutex& operator=(const GnrlMutex& arg);

	int32_t m_State;	// GetLastError();�̖߂�l

#ifdef HAVE_CXX_VOLATILE
	volatile
#endif
	void *theMutex;
};

// ---------------------- Mutex / LockedMutex ---------------------------------

// {}�X�R�[�v�����~���[�e�N�X�Ŕr�����Ƃ�܂��B
// std::tr1::shared_ptr�́AVS6.0�ł͑Ή����Ă��Ȃ��݂����B
// #include <memory> 
// using namespace std;

// �����ϐ��錾�p�~���[�e�N�X���b�N�N���X 
class GnrlLockedMutex
{
public:
	explicit GnrlLockedMutex(GnrlMutex &MutexObj);				// �R���X�g���N�^��Lock���܂��B		 

	~GnrlLockedMutex();

	// �߂�l ==0 ����	 
//	Sint32 getStare()		{ return m_Mutex.getState();	}

private:
//	std::tr1::shared_ptr<GnrlMutex> mutexPtr;
	GnrlMutex	&m_Mutex;
};

#endif //GENERALMUTEX_H

