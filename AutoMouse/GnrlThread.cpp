// ****************************************************************************
// �X���b�h�쐬�A�Ǘ��A�I������
//
// GnrlThread.cpp
// 
// 2013/06/31	Createed
// 2018/12/08	�����C��
// ****************************************************************************

// ���ˑ�
#include "stdafx.h"
#include "Windows.h"

// �ėp
#include "GnrlNumerical.h"
//#include "GnrlDefine.h"
#include "GnrlThread.h"


GnrlThread::GnrlThread()
 :m_LIFE(FALSE)
 ,m_lpStartAddress(NULL)
 ,m_ThreadID(NULL)
 ,m_hThread(NULL)
{}

GnrlThread::GnrlThread(void* lpStartAddress)
 :m_LIFE(FALSE)
 ,m_lpStartAddress(lpStartAddress)
 ,m_ThreadID(NULL)
 ,m_hThread(NULL)
{
	m_lpStartAddress = lpStartAddress;
}

GnrlThread::~GnrlThread()
{
	endThread();
}

// ****************************************************************************
// �X���b�h�̃X�^�[�g�A�h���X��ݒ肷��
// ****************************************************************************
void* GnrlThread::setStartAddress(void* lpStartAddress)
{
	if(m_LIFE == TRUE)
		return NULL;	// ���ɃX���b�h�N���� 

	return (void*) m_lpStartAddress = lpStartAddress;
}

// ****************************************************************************
// �X���b�h���N�����܂��B
// �X���b�h�N����ň����̎w��͂ł��܂���B
// �p�����[�^��n���ꍇ�́A�O���[�o���ϐ��œn�����A�܂��́AGnrlThread���p���܂��̓R���{���V�������āA�N���X�̃����o�ōs���ĉ������B
// �����@Parm�@NULL�F�@���̊֐������s����O�ɁA�\�߃X���b�h�̃X�^�[�g�A�h���X���ݒ肳��Ă���K�v������܂��B
//�@�@�@�@�@�@�X���b�h�̃X�^�[�g�A�h���X�F
// ****************************************************************************
void* GnrlThread::beginThread(void *StartAddless, void *Param, Bool8 Suspend)
{
	Sint32 creationflag = 0;

	if(Suspend)
	{
		creationflag |= CREATE_SUSPENDED;
	}

	if(isEndThread(0) == FALSE)
		return NULL;			// ���ɃX���b�h�N����

	if(StartAddless != NULL){
		m_lpStartAddress = StartAddless;
	}
	
	if(m_lpStartAddress == NULL){
		return NULL;
	}

	m_hThread = ::CreateThread(0, 0,
		(LPTHREAD_START_ROUTINE)m_lpStartAddress,
		Param, creationflag, (unsigned long*)&m_ThreadID);

	if(m_hThread != NULL)
	{
		m_LIFE = TRUE;
	}

	return m_hThread;
}

// ****************************************************************************
// ���[�J�[�X���b�h�֏I�����˗�����B
// ****************************************************************************
void GnrlThread::endThread()
{
	m_LIFE = FALSE;
}

// ****************************************************************************
// �X���b�h���I�������Ȃ�TRUE��Ԃ��܂�
//�@�X���b�h���I�����Ă��Ȃ��ꍇ��FALSE��Ԃ��܂�
// ����0�͑҂��Ȃ��A0�ȊO�͏I������܂Ń~���b�ҋ@���܂�
// ****************************************************************************
Bool8 GnrlThread::isEndThread(Uint32 dwMilliseconds)
{
	if(WaitForSingleObject((void*)m_hThread, dwMilliseconds) == WAIT_TIMEOUT)
		return FALSE;

	return TRUE;
}
/*
// ****************************************************************************
// �h�����vRoutine()���Ăяo���A���̊֐����ŁA
// ���[�J�[�X���b�h�����삵�܂��B
// �h����N���X�ɁA�����o�[���쐬���āA�g�p���ĉ������B
// ****************************************************************************
void GnrlThread::s_callRoutine(void* Arg)
{
	Sint32 ret;
	
	ret = m_lpStartAddress(Arg);

	::ExitThread(ret);
}

// ****************************************************************************
// �s���ȌĂяo���Ȃ̂ŁA�����ɏI��
// ****************************************************************************
Sint32 GnrlThread::VRoutine()
{
	endThread();
//	ExitThread(0);
	return -1;
}
*/

// ----------------- GeneralThread / GeneralMutex -------------------------------

const int GnrlMutex::m_Busy = -1;	// �g�p���̏ꍇ��-1��Ԃ�

GnrlMutex::GnrlMutex()
: theMutex(NULL)
, m_State(-1)
{
#ifdef WINDOWS_INTERFACE
  theMutex = (void *)(CreateMutex(NULL, FALSE, NULL));
#elif defined(POSIX_INTERFACE)
  pthread_mutex_t *mtx = new pthread_mutex_t;
  if (mtx)
  {
    if (pthread_mutex_init(mtx, NULL)) delete mtx;
    else theMutex = mtx;
  }
#elif defined(SOLARIS_INTERFACE)
  mutex_t *mtx = new mutex_t;
  if (mtx)
  {
    if (mutex_init(mtx, USYNC_THREAD, NULL)) delete mtx;
    else theMutex = mtx;
  }
#else
#endif
}


GnrlMutex::~GnrlMutex()
{
#ifdef WINDOWS_INTERFACE
  CloseHandle((HANDLE)theMutex);
#elif defined(POSIX_INTERFACE)
  if (theMutex) pthread_mutex_destroy(OFthread_cast(pthread_mutex_t *, theMutex));
  delete OFthread_cast(pthread_mutex_t *, theMutex);
#elif defined(SOLARIS_INTERFACE)
  if (theMutex) mutex_destroy(OFthread_cast(mutex_t *, theMutex));
  delete OFthread_cast(mutex_t *, theMutex);
#else
#endif
}

// ****************************************************************************
//  �߂�l ==0 ���b�N����
//		  !=0 ���b�N���s
// ****************************************************************************
Sint32 GnrlMutex::lock()
{
#ifdef WINDOWS_INTERFACE
  if (WaitForSingleObject((HANDLE)theMutex, INFINITE) == WAIT_OBJECT_0)
  {
	  return 0;
  }
  else
  {
	  m_State = (Sint32)GetLastError();
	  return m_State;
  }
#elif defined(POSIX_INTERFACE)
  if (theMutex)
	  return pthread_mutex_lock(OFthread_cast(pthread_mutex_t *, theMutex));
  else
	  return EINVAL;
#elif defined(SOLARIS_INTERFACE)
  if (theMutex)
	  return mutex_lock(OFthread_cast(mutex_t *, theMutex));
  else
	  return EINVAL;
#else
  return -1;
#endif
}

// ****************************************************************************
// �u���b�L���O���s��Ȃ����b�N���s��
//  �߂�l
//    == 0�@�u���b�L���O����
//    == -1 ���b�N�Ɏ��s�����ꍇ�i�������g�p���̏ꍇ�j
//      ���̒l�@���s
// ****************************************************************************
Sint32 GnrlMutex::trylock()
{
#ifdef WINDOWS_INTERFACE
	Sint32 ret;
	ret = WaitForSingleObject((HANDLE)theMutex, 0);
	if(ret == WAIT_OBJECT_0)
		return 0;
	else
		if(ret == WAIT_TIMEOUT)
			return GnrlMutex::m_Busy;
	else
	{
		m_State = (Sint32)GetLastError();
		return m_State;
	}
#elif defined(POSIX_INTERFACE)
  if (theMutex) return pthread_mutex_trylock(OFthread_cast(pthread_mutex_t *, theMutex)); else return EINVAL; // may return EBUSY.
#elif defined(SOLARIS_INTERFACE)
  if (theMutex) return mutex_trylock(OFthread_cast(mutex_t *, theMutex)); else return EINVAL; // may return EBUSY.
#else
  return -1;
#endif
}

Sint32 GnrlMutex::unlock()
{
#ifdef WINDOWS_INTERFACE
  if (ReleaseMutex((HANDLE)theMutex))
	  return 0;
  else
  {
	  m_State = (Sint32)GetLastError();
	  return m_State;
  }
#elif defined(POSIX_INTERFACE)
  if (theMutex) return pthread_mutex_unlock(OFthread_cast(pthread_mutex_t *, theMutex)); else return EINVAL;
#elif defined(SOLARIS_INTERFACE)
  if (theMutex) return mutex_unlock(OFthread_cast(mutex_t *, theMutex)); else return EINVAL;
#else
  return -1;
#endif
}

// ----------------- GnrlMutex / GnrlLockedMutex -------------------------------

GnrlLockedMutex::GnrlLockedMutex(GnrlMutex &MutexObj)
:m_Mutex(MutexObj)
{
	m_Mutex.lock();
}

GnrlLockedMutex::~GnrlLockedMutex()
{
	m_Mutex.unlock();
}
