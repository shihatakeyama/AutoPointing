// ****************************************************************************
// スレッド作成、管理、終了する
//
// GnrlThread.cpp
// 
// 2013/06/31	Createed
// 2018/12/08	引数修正
// ****************************************************************************

// 環境依存
#include "stdafx.h"
#include "Windows.h"

// 汎用
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
// スレッドのスタートアドレスを設定する
// ****************************************************************************
void* GnrlThread::setStartAddress(void* lpStartAddress)
{
	if(m_LIFE == TRUE)
		return NULL;	// 既にスレッド起動中 

	return (void*) m_lpStartAddress = lpStartAddress;
}

// ****************************************************************************
// スレッドを起動します。
// スレッド起動先で引数の指定はできません。
// パラメータを渡す場合は、グローバル変数で渡すか、または、GnrlThreadを継承またはコンボじションして、クラスのメンバで行って下さい。
// 引数　Parm　NULL：　この関数を実行する前に、予めスレッドのスタートアドレスが設定されている必要があります。
//　　　　　　スレッドのスタートアドレス：
// ****************************************************************************
void* GnrlThread::beginThread(void *StartAddless, void *Param, Bool8 Suspend)
{
	Sint32 creationflag = 0;

	if(Suspend)
	{
		creationflag |= CREATE_SUSPENDED;
	}

	if(isEndThread(0) == FALSE)
		return NULL;			// 既にスレッド起動中

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
// ワーカースレッドへ終了を依頼する。
// ****************************************************************************
void GnrlThread::endThread()
{
	m_LIFE = FALSE;
}

// ****************************************************************************
// スレッドが終了したならTRUEを返します
//　スレッドが終了していない場合はFALSEを返します
// 引数0は待たない、0以外は終了するまでミリ秒待機します
// ****************************************************************************
Bool8 GnrlThread::isEndThread(Uint32 dwMilliseconds)
{
	if(WaitForSingleObject((void*)m_hThread, dwMilliseconds) == WAIT_TIMEOUT)
		return FALSE;

	return TRUE;
}
/*
// ****************************************************************************
// 派生先のvRoutine()を呼び出し、この関数内で、
// ワーカースレッドが動作します。
// 派生先クラスに、メンバーを作成して、使用して下さい。
// ****************************************************************************
void GnrlThread::s_callRoutine(void* Arg)
{
	Sint32 ret;
	
	ret = m_lpStartAddress(Arg);

	::ExitThread(ret);
}

// ****************************************************************************
// 不正な呼び出しなので、すぐに終了
// ****************************************************************************
Sint32 GnrlThread::VRoutine()
{
	endThread();
//	ExitThread(0);
	return -1;
}
*/

// ----------------- GeneralThread / GeneralMutex -------------------------------

const int GnrlMutex::m_Busy = -1;	// 使用中の場合は-1を返す

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
//  戻り値 ==0 ロック成功
//		  !=0 ロック失敗
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
// ブロッキングを行わないロックを行う
//  戻り値
//    == 0　ブロッキング成功
//    == -1 ロックに失敗した場合（資源が使用中の場合）
//      他の値　失敗
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
