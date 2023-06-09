// ****************************************************************************
// スレッド作成、管理、終了する
//
// GnrlThread.h
// 
// ****************************************************************************

#ifndef GENERALTHREAD_H
#define GENERALTHREAD_H


class GnrlThread
{
public:

	// ワーカースレッドの状態
	enum E_WorkState
	{
		EWS_signal			= 0,	// 通常ワーク
		EWS_non_signal		= 1,	// 一時停止	waitfor()などで待たされてる、
		EWS_stop			= 2		// 中止, 停止,廃棄
	};

	GnrlThread();
	GnrlThread(void *lpStartAddress);
	~GnrlThread();

	void*	beginThread(void *StartAddless, void *Param, bool Suspend);	// �@ スレッド起動
	int32_t	resumeThread()	{return ResumeThread(m_hThread);}	// �@’再スタート 関数が成功すると、関数呼び出し前のサスペンドカウントが返ります。関数が失敗すると、0xFFFFFFFF が返ります。
	void	endThread();								// �Aワーカースレッドへ終了を依頼する。
	bool	isLife() const		{ return m_LIFE; }		// スレッド終了状態取得
	void	setLife(bool Life)	{ m_LIFE = Life; }		// Lifeをセット
	bool	isEndThread(uint32_t dwMilliseconds = 10000);	// �Bワーカースレッドが終了したか確認 
	void*	setStartAddress(void* lpStartAddress);
//--	virtual Sint32 VRoutine();	// 継承先のクラスで、ルーチンを定義する場合のメンバ関数		 

private:
	bool	m_LIFE;					// FALSになったら、直ちにループを終了し、ワーカースレッドを終了させて下さい。 
//	static	void s_callRoutine(void* Arg);	// BeginThread()が呼ぶ為の関数	 
	void*	m_lpStartAddress;		// s_callRoutine()は、ワーカスレッドの先頭アドレスを設定する関数 

	uint32_t	m_ThreadID;				//	m_lpStartAddress = (LPTHREAD_START_ROUTINE)&CAboutDlg::test;　←staic宣言の関数
	void*	m_hThread;
};

#endif //GENERALTHREAD

// ---------------------- GeneralThread / Mutex -------------------------------

// ミューテクスロック
// このクラスはシングルタスクのみ有効です
//  GnrlMutex.h
//  C++
 	 
#ifndef GENERALMUTEX_H
#define GENERALMUTEX_H

class GnrlMutex
{
public:

	GnrlMutex();
	~GnrlMutex();

	//　現在所持しているハンドルを戻します。
	//　戻り値　==NUNN ハンドル取得失敗または未取得

#ifdef HAVE_CXX_VOLATILE
	volatile
#endif
	const void *getHandle() const
	{
		return theMutex;
	}

	int32_t lock();		// 成功すると0を返す　以外は異常 
	int32_t trylock();	// 成功すると0を返す　以外は異常 
	int32_t unlock();	// 成功すると0を返す　以外は異常 

	static const int32_t m_Busy;

private:
	GnrlMutex(const GnrlMutex& arg);
//	GnrlMutex& operator=(const GnrlMutex& arg);

	int32_t m_State;	// GetLastError();の戻り値

#ifdef HAVE_CXX_VOLATILE
	volatile
#endif
	void *theMutex;
};

// ---------------------- Mutex / LockedMutex ---------------------------------

// {}スコープ内をミューテクスで排他をとります。
// std::tr1::shared_ptrは、VS6.0では対応していないみたい。
// #include <memory> 
// using namespace std;

// 自動変数宣言用ミューテクスロッククラス 
class GnrlLockedMutex
{
public:
	explicit GnrlLockedMutex(GnrlMutex &MutexObj);				// コンストラクタでLockします。		 

	~GnrlLockedMutex();

	// 戻り値 ==0 成功	 
//	Sint32 getStare()		{ return m_Mutex.getState();	}

private:
//	std::tr1::shared_ptr<GnrlMutex> mutexPtr;
	GnrlMutex	&m_Mutex;
};

#endif //GENERALMUTEX_H

