// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// AutoPointingDlg.cpp : 実装ファイル
//
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"

#include "afxdialogex.h"
#include "vector"
#include "Windows.h"

#include "GnrlNumerical.h"
#include "GnrlThread.h"
#include "Define.h"

#include "GnrlCom.h"
#include "GnrlComList.h"

#include "global.h"
#include "extern.h"
#include "global.h"
#include "sub.h"
#include "GnrlDefine.h"
#include "GnrlFilepath.h"
#include "GnrlCom.h"
#include "WorkBase.h"

#include "AutoPointing.h"
#include "AutoPointingDlg.h"

void APD_SleepAppend(int32_t msc);


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// VS_VERSIONINFOは以下のように定義しなおし
struct VS_VERSIONINFO { 
    WORD  wLength;          // この構造体のサイズ(2バイト)
    WORD  wValueLength;     // Valueのサイズ(2バイト)
    WORD  wType;            // バイナリかテキストか(2バイト)
    WCHAR szKey[16];        // "VS_VERSION_INFO\0"の16文字(32バイト)
    WORD  Padding1[1];      // 2バイト入れて合計40バイト.
    VS_FIXEDFILEINFO Value; // ここにバージョン番号が入る.
//  WORD  Padding2[];       // 可変長のためC言語の構造体では表現不可.
//  WORD  Children[];       // しかもあまり意味のないデータ.
};

// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg(unsigned int FiraWareVersion);

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
public:
private:
public:
	CString m_StrAppVer,m_StrFirVer;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// バージョン表示
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
CAboutDlg::CAboutDlg(unsigned int FiraWareVersion) : CDialogEx(CAboutDlg::IDD)
, m_StrAppVer(_T(""))
{
	HRSRC hRsrc = FindResource(NULL,(LPCTSTR)VS_VERSION_INFO,RT_VERSION);
	HGLOBAL hGlobal = LoadResource(NULL,hRsrc);
	struct VS_VERSIONINFO *pData = (VS_VERSIONINFO*)LockResource(hGlobal);
	
	m_StrAppVer.Format(_T("Aplication Version: %08X"), SOFT_VERSION);	//  pData->Value.dwProductVersionLS

	if(FiraWareVersion == 0xFFFFFFFF){
		m_StrFirVer		 = _T("Firmware  Version: --------");
	}else{
		m_StrFirVer.Format(_T("Firmware  Version: %08X")	, FiraWareVersion);
	}
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_APPVER, m_StrAppVer);
	DDX_Text(pDX, IDC_STATIC_FIRVER, m_StrFirVer);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_DRAWITEM()
//	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// AutoPointingDlg ダイアログ



CAutoPointingDlg::CAutoPointingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoPointingDlg::IDD, pParent)
	, m_Pointm(_T(""))
	, m_Pointw(_T(""))
	, m_StrEndTime(_T(""))
	, m_EndTime((time_t)-1)
	, m_TargetWindowName(_T(""))
	, m_Delay(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAutoPointingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_POINTM, m_Pointm);
	DDX_Text(pDX, IDC_STATIC_POINTW, m_Pointw);
	DDX_Control(pDX, IDC_BUTTON_START, m_Start);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_Stop);
	//  DDX_Control(pDX, IDC_COMBO_COMNO, m_ComNoCombo);
	DDX_Control(pDX, IDC_COMBO_COMSEL, m_ComPortCombo);
	DDX_Control(pDX, IDC_BUTTON_COM_CONNECT, mButtonConnect);
	DDX_Text(pDX, IDC_STATIC_END_TIME, m_StrEndTime);
	DDX_Control(pDX, IDC_COMBO_OPERATION, m_OperationSel);
	DDX_Text(pDX, IDC_STATIC_TARGET_WINDOW_NAME, m_TargetWindowName);
	DDV_MaxChars(pDX, m_TargetWindowName, 256);
	DDX_Control(pDX, IDC_BUTTON_NOWLONG, m_NowLong);
	DDX_Control(pDX, IDC_BUTTON_NOWSHORT, m_NotShort);
	DDX_Text(pDX, IDC_STATIC_DELAY, m_Delay);
}

BEGIN_MESSAGE_MAP(CAutoPointingDlg, CDialogEx)
//	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CAutoPointingDlg::OnBnClickedButton1)
	ON_WM_DESTROY()

	// 追加
	ON_MESSAGE(WM_UPDATEDATA, OnUpdatedataA)
	ON_MESSAGE(WM_SHOW_VERSION, OnShowVersion)

	ON_BN_CLICKED(IDC_BUTTON_START, &CAutoPointingDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CAutoPointingDlg::OnBnClickedButtonStop)
//	ON_BN_CLICKED(IDC_BUTTON2, &CAutoPointingDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_COMSERCH, &CAutoPointingDlg::OnBnClickedButtonComserch)
	ON_BN_CLICKED(IDC_BUTTON_COM_CONNECT, &CAutoPointingDlg::OnBnClickedButtonComConnect)
	ON_WM_DRAWITEM()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_NOWLONG, &CAutoPointingDlg::OnBnClickedButtonNow)
	ON_WM_TIMER()
	ON_WM_SYSCOMMAND()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_10, &CAutoPointingDlg::OnDeltaposSpin10)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_BUTTON_NOWSHORT, &CAutoPointingDlg::OnBnClickedButtonNowshort)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ウインドウ初期設定
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
BOOL CAutoPointingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		// Load/Save
		pSysMenu->InsertMenu(1, MF_SEPARATOR);
		pSysMenu->InsertMenu(1, MF_STRING, IDC_LOAD_XML, _T("LOAD"));
		pSysMenu->InsertMenu(1, MF_STRING, IDC_SAVE_XML, _T("SAVE"));

		// バージョン情報
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定


	srand( (unsigned)time( NULL ) );  

	// TODO: 初期化をここに追加します。
	pAutoPointingDlg = this;

	loadXmlIni();

	// COM サーチ
	OnBnClickedButtonComserch();
#if 0
	try{
		initParam();
	}
	catch (const std::wstring& e){
//		e.what();	// e.what()		/// _T("イニシャライズ失敗")
		MessageBox(e.c_str(), gApplicatonName.c_str(), MB_OK | MB_ICONSTOP);		// MB_ICONERROR ,
		return false;
	}
#endif

	{ // 初期ウインドウ位置  ※Dialogの｢プロパティ｣>｢Center｣で値をTRUEにしておかないと中央に戻されてしまう。

		RECT  rect;
		int32_t width  = GetSystemMetrics(SM_CXSCREEN);		// スクリーンの幅を取得
		int32_t height = GetSystemMetrics(SM_CYSCREEN);		// スクリーンの高さを取得


//		::GetClientRect(nullptr ,&mrect);
		GetWindowRect(&rect);
		
		// 自ウインドウの大きさを引く
		width -= rect.right  - rect.left;
		height-= rect.bottom - rect.top;

		int32_t x = gWindowPos.x * width / gWindowDenominator ;
		int32_t y = gWindowPos.y * height / gWindowDenominator;

		BOOL aaa = SetWindowPos(this ,x ,y ,0 ,0 ,SWP_NOSIZE|SWP_NOZORDER);
	}


	// 接続ボタンの色
	m_CbrCom[0].CreateSolidBrush(RGB(0x80, 0x80, 0x80)); 
	m_CbrCom[1].CreateSolidBrush(RGB(0x00, 0x00, 0xFF)); 


	m_TargetWindowName = gTargetWindowName.c_str();



	// **** 各種スレッド起動 ****
//--	gMouseThread.beginThread(MousePointThread ,NULL ,FALSE);

	gOperationThread.beginThread(OperationThread ,NULL ,FALSE);

	gRecvThread.beginThread(RecvRootine ,NULL ,FALSE);


	m_Stop.EnableWindow(FALSE);


	{ // now ボタン
		const std::size_t size = 16;
		TCHAR buf[size];

		toNowText(buf, size ,gNowTime[1]);
		m_NowLong.SetWindowTextW(buf);

		toNowText(buf, size, gNowTime[2]);
		m_NotShort.SetWindowTextW(buf);
	}

	setEndTime(gNowTime[0]);

	SetTimer(ET_100ms, 100, 0);


	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}
void CAutoPointingDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	g_Operation = 0;

//++	saveXmlIni();

	gMouseThread.endThread();
	gOperationThread.endThread();
	gRecvThread.endThread();

	gMouseThread.isEndThread(1000);
	gOperationThread.isEndThread(1000);
	gRecvThread.isEndThread(1000);

	KillTimer(ET_100ms);

	GnrlComList::clearComList();

//--	clearParam();

	WorkBase::clearProcList(gWorks);
	gWorkNames.clear();

//	g_Life = 0;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// XML save/load
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t CAutoPointingDlg::loadXmlIni()
{
	int32_t ack;
	TCHAR InitFilePath[MAX_PATH];

	ack = GnrlFilepath::getModuleAttachmentFilePath(InitFilePath, MAX_PATH, _T("apd_ini.xml"));
	if (ack <= 0)	throw std::wstring(_T("apd_ini.xml がありません"));

	return loadXml(InitFilePath);
}
int32_t CAutoPointingDlg::loadXmlGui()
{
	CFileDialog dlgFile(TRUE);
	OPENFILENAME &ofn = dlgFile.GetOFN();
	ofn.lpstrTitle = _T("パラメータを開く");
	ofn.lpstrFilter = _T("All Files\0*.*\0Process Files\0*.prc\0Log Files\0*.log\0");
	ofn.Flags |= OFN_SHOWHELP;

	if (dlgFile.DoModal() == IDOK){
		try{
			loadXml(dlgFile.GetPathName());
//			setGui();
		}
		catch (std::exception e){
			MessageBox(CString(e.what()));
			return ERC_ng;
		}
	}

	return ERC_ok;
}
int32_t CAutoPointingDlg::loadXml(const TCHAR *Path)
{
	int32_t ack;
	int32_t val;
	rapidxml::document_t doc;
	rapidxml::string_t docbuf;

	// 排他
	std::lock_guard<std::mutex> lock(gWorkMutex);

	try{
		ack = rapidxml::load_document(doc, Path, docbuf);
		if (ack < 0){ throw std::wstring(_T("load document")); return ERC_ng; }
	}
	catch (const std::wstring& e){
		std::wstring  str(_T("データ読み込み失敗\n"));
		str += e;
		throw str;
	}
	catch (const rapidxml::parse_error &e){
		std::wstring  str(_T("XML データ読み込み失敗\n"));
		str += CString(e.what()) + '\n' + e.where<rapidxml::char_t>();

		throw str;
	}

	// **** 各種パラメータ読み込み ****

	rapidxml::node_t *root = doc.first_node();
	rapidxml::node_t *node;
	if (rapidxml::comp_node_name(root, gApplicatonName.c_str()) != 0){
		throw std::wstring(_T("XML の書式が違います"));
	}

	rapidxml::first_node(root, _T("title"), gTitle);

	node = rapidxml::first_node(root, _T("target"));
	if (node){
		rapidxml::first_attribute(node, _T("window_name"), gTargetWindowName);
		rapidxml::first_attribute(node, _T("x"), val);	gBasePoint.x = val;
		rapidxml::first_attribute(node, _T("y"), val);	gBasePoint.y = val;
	}

	// ウインドウ内収まりチェック
	node = rapidxml::first_node(root, _T("inside"));
	if (node){
		rapidxml::first_attribute_check(node, gInsideCheck);
	}

	// ウインドウ表示初期位置
	node = rapidxml::first_node(root, _T("window"));
	if (node){
		rapidxml::attribute_t *attr;

		attr = rapidxml::first_attribute(node, _T("denominator"), gWindowDenominator);

		attr = rapidxml::first_attribute(node, _T("vpos"), val);
		if (attr){
			gWindowPos.x = val;
		}

		attr = rapidxml::first_attribute(node, _T("hpos"), val);
		if (attr){
			gWindowPos.y = val;
		}
	}

	// COM
	node = rapidxml::first_node(root, _T("com"));
	if (node){
		gCom.loadXmlNode(node);
	}

	// ブレ
	node = rapidxml::first_node(root, _T("blur"));
	if (node == nullptr){
		gBurePoint = CPoint(4, 4);
		gBureTime = 4;
	}
	else{
		rapidxml::first_attribute(node, _T("x"), val);	gBurePoint.x = val;
		rapidxml::first_attribute(node, _T("y"), val);	gBurePoint.y = val;
		rapidxml::first_attribute(node, _T("time"), val);	gBureTime = val;
	}

	// アクティブ時のポーズ
	node = rapidxml::first_node(root, _T("active"));
	if (node){
		rapidxml::first_attribute(node, _T("pause_time"), gActivePauseTime);
	}

	// 終了時刻
	node = rapidxml::first_node(root, _T("end_time"));
	if (node == nullptr){
	}
	else{
		rapidxml::first_attribute(node, _T("time0"), gNowTime[0]);
		rapidxml::first_attribute(node, _T("time1"), gNowTime[1]);
		rapidxml::first_attribute(node, _T("time2"), gNowTime[2]);
		rapidxml::first_attribute(node, _T("spin"),	gSpinTime);
	}

	// ワーク読み込み
	rapidxml::node_t *work = rapidxml::first_node(root, _T("works"));
	if (work == nullptr){
		throw std::wstring(_T("XML にworks タグがありません。"));
	}

	ack = WorkBase::loadWorkList(gWorks, work, gWorkNames);

	rapidxml::first_attribute(work, _T("index"), gWorkIndex);
	if (gWorkIndex >= static_cast<int32_t>(gWorks.size())){
		gWorkIndex = 0;
	}

	// **** ワーク ****
	for (const auto &e : gWorkNames){
		m_OperationSel.InsertString(-1, e.c_str());
	}

	m_OperationSel.SetCurSel(gWorkIndex);


	return ERC_ok;
}
int32_t CAutoPointingDlg::saveXmlIni()
{
	int32_t ack;
	TCHAR InitFilePath[MAX_PATH];

	ack = GnrlFilepath::getModuleAttachmentFilePath(InitFilePath, MAX_PATH, _T("apd_ini.xml"));
	if (ack <= 0)	throw std::wstring(_T("apd_ini.xml がありません"));

	return saveXml(InitFilePath);
}
int32_t CAutoPointingDlg::saveXmlGui()
{
	CFileDialog dlgFile(FALSE);
	OPENFILENAME &ofn = dlgFile.GetOFN();
	ofn.lpstrTitle = _T("パラメータを保存");
	ofn.lpstrFilter = _T("All Files\0*.*\0Process Files\0*.prc\0Log Files\0*.log\0");
	ofn.Flags |= OFN_SHOWHELP;

	if (dlgFile.DoModal() == IDOK){
		try{
			//			getGui();
			CString filepath = dlgFile.GetPathName();
			saveXml(filepath);
		}
		catch (std::exception e){
			MessageBox(CString(e.what()));
			return ERC_ng;
		}
	}
	return ERC_ok;
}
int32_t CAutoPointingDlg::saveXml(const TCHAR *Path)
{
	int32_t ack;
	int32_t val;
	rapidxml::document_t doc;
	rapidxml::node_t *root = rapidxml::allocate_node(doc, gApplicatonName.c_str());
	rapidxml::node_t *node;
	rapidxml::attribute_t *attr;

	rapidxml::append_attribute(doc, root, _T("version"), _T("1.0"));
	rapidxml::append_attribute(doc, root, _T("encoding"), _T("utf-8"));

	// 排他
	std::lock_guard<std::mutex> lock(gWorkMutex);

	// **** 各種パラメータ書き込み ****
	rapidxml::append_node(doc ,root, _T("title"), gTitle);

	rapidxml::append_node_hex(doc, root, _T("soft_version"), SOFT_VERSION);

	node = rapidxml::append_node(doc, root, _T("target"));
//	attr = rapidxml::allocate_attribute(doc, _T("window_name"), gTargetWindowName.c_str());
	rapidxml::append_attribute(doc ,node , _T("window_name"), gTargetWindowName.c_str());
	rapidxml::append_attribute(doc, node, _T("x"), gBasePoint.x);
	rapidxml::append_attribute(doc, node, _T("y"), gBasePoint.y);

	// ウインドウ内収まりチェック
	node = rapidxml::append_node(doc ,root, _T("inside"));
	rapidxml::append_attribute_check(doc, node, gInsideCheck);

	// ウインドウ表示初期位置
	node = rapidxml::append_node(doc, root, _T("window"));
	rapidxml::append_attribute(doc, node, _T("denominator"), gWindowDenominator);
	rapidxml::append_attribute(doc, node, _T("vpos"), gWindowPos.x);
	rapidxml::append_attribute(doc, node, _T("hpos"), gWindowPos.y);

	// COM
	GnrlComList::getGuiPortNo(m_ComPortCombo, gCom);
	gCom.saveXmlNode(doc ,node);
	rapidxml::name(node, _T("com"));
	rapidxml::append_node(root, node);

	// ブレ
	node = rapidxml::append_node(doc ,root , _T("blur"));
	rapidxml::append_attribute(doc, node, _T("x"), gBurePoint.x);
	rapidxml::append_attribute(doc, node, _T("y"), gBurePoint.y);
	rapidxml::append_attribute(doc, node, _T("time"), gBureTime);

	// アクティブ時のポーズ
	node = rapidxml::append_node(doc, root, _T("active"));
	rapidxml::append_attribute(doc, node, _T("pause_time"), gActivePauseTime);

	// 終了時刻
	node = rapidxml::append_node(doc,root , _T("end_time"));
	rapidxml::append_attribute(doc, node, _T("time0"), gNowTime[0]);
	rapidxml::append_attribute(doc, node, _T("time1"), gNowTime[1]);
	rapidxml::append_attribute(doc, node, _T("time2"), gNowTime[2]);
	rapidxml::append_attribute(doc, node, _T("spin"), gSpinTime);

	// ワーク
	node = rapidxml::append_node(doc, root ,_T("works"));
	rapidxml::append_attribute(doc, node, _T("index"), gWorkIndex);
	ack = WorkBase::saveWorkList(gWorks, doc ,node, gWorkNames);

	// XML書き込み
	doc.append_node(root);
	ack = rapidxml::save_document(doc, Path);

	return ack;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ユーザーがバージョンボタンクリック
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	int ack;

	switch (nID){
//	if ((nID & 0xFFFF) == IDM_ABOUTBOX){
	case IDM_ABOUTBOX:
	{
		if (!gCom.isOpened()){
			openCom();
		}

		ack = APD_sendCommand(0x99, 0x99, NULL, 0);
		if (ack < 0){
			CAboutDlg dlgAbout(0xFFFFFFFF);
			dlgAbout.DoModal();
		}
		break;
	}
	case IDC_LOAD_XML:
		loadXmlGui();
		break;
	case IDC_SAVE_XML:
		saveXmlGui();
		break;
	default:
		CDialogEx::OnSysCommand(nID, lParam);
	}
//--	CDialogEx::OnSysCommand(nID, lParam);
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。
void CAutoPointingDlg::OnPaint()
{
	UpdateData(FALSE);

	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CAutoPointingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// タイマー
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::OnTimer(UINT_PTR nIDEvent)
{
	time_t now;
	static int32_t delay = 0;
	static POINT oldpoint = { 0, 0 };
	Sint32 ack;

	if (nIDEvent == ET_100ms){
		// **** Delay 表示 ****
		if (delay != gDelayRemine){
			delay = gDelayRemine;
			m_Delay.Format(_T("%4d.%03d"), delay/1000 ,delay%1000);
		}

		// **** 座標表示 ****
		RECT rect;
		POINT point;

		GetCursorPos(&point);

		if ((point.x != oldpoint.x) || (point.y != oldpoint.y)){

			m_Pointm.Format(_T("x=%5d , y=%5d"), point.x, point.y);

			ack = getTargetWindowPos(rect);
			if (ack < 0){
				m_Pointw = _T("-------  -------");
			}
			else{
				m_Pointw.Format(_T("x=%5d , y=%5d"), point.x - rect.left, point.y - rect.top);
			}

			oldpoint = point;

			UpdateData(false);
		}

		// **** 終了判定 ****
		if ((g_Operation == 0) || (time(&now) == (time_t)-1) || (m_EndTime == (time_t)-1) ) {
			return;
		}
		if (time(&now) == (time_t)-1) {
			return;
		}

		if(m_EndTime < now){
			OnBnClickedButtonStop();
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 事前メッセージ
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
BOOL CAutoPointingDlg::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message & 0xFFFF){
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			return FALSE;
		case VK_ESCAPE:
			return FALSE;
		default:
			break;
		}
		break;
	}

	switch (pMsg->message){
//	case WM_SHOWWINDOW:
//	case WA_CLICKACTIVE:
	case WM_ACTIVATE:
	case WM_NCACTIVATE:
	case WM_MOUSEACTIVATE:
//	case WA_INACTIVE:
	case WM_CHILDACTIVATE:
	case WM_CLOSE:
		break;

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


// 他スレッドからUIの表示更新する場合。
LRESULT CAutoPointingDlg::OnUpdatedataA(WPARAM wParam, LPARAM lParam)
{
///	this->PostMessage(WM_UPDATEDATA);
	UpdateData(FALSE);

	return 0; // メッセージ固有の戻り値を返す
}

void CAutoPointingDlg::sendMousePoint(const CString &Strm ,const CString &Strw)
{
	m_Pointm = Strm;
	m_Pointw = Strw;

	this->PostMessage(WM_UPDATEDATA, 0);
}

LRESULT CAutoPointingDlg::OnShowVersion(WPARAM wParam, LPARAM lParam)
{
	CAboutDlg dlgAbout(wParam);
	dlgAbout.DoModal();	

	return 0; // メッセージ固有の戻り値を返す
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ボタン押された
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::OnBnClickedButton1()
{
#if 0
	int comno,ack;

	if(!gCom.isOpened()){
		openCom();
	}

	ack = APD_sendCommand(0x99, 0x99, NULL, 0);
	if(ack < 0){
		
	}
#endif
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ターゲットウインドウ位置を取得する。
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t getTargetWindowPos(RECT &Rect)
{
	LPCTSTR winname = gTargetWindowName.c_str();
	HWND hWndChild = ::FindWindowEx(NULL, NULL, NULL, winname);
	if (hWndChild == NULL) {
		return -1;
	}

	::GetWindowRect( hWndChild, &Rect );		//デスクトップのハンドルからその(画面の)大きさを取得

	return 0;
}

int32_t gPointListLen;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 開始 ボタン押された
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::OnBnClickedButtonStart()
{
	int32_t curpos,ack;

	if(!gCom.isOpened()){
		openCom();
	}

	if(!gCom.isOpened()){
		Beep(400,400);
		MessageBox(_T("デジタイザが見つかりません。"), gApplicatonName.c_str(), MB_OK | MB_ICONSTOP);
		return;
	}

	// デジタイザに画面の解像度を設定
	RECT rec;
	HWND hDeskWnd = ::GetDesktopWindow();	//この関数でデスクトップのハンドルを取得
	::GetWindowRect( hDeskWnd, &rec );		//デスクトップのハンドルからその(画面の)大きさを取得
	AM_setDisplayResolution(rec.right ,rec.bottom);
//	AM_click(321 ,421);

	ack = getTargetWindowPos(rec);
	if(ack < 0){
		MessageBox(_T("ターゲットウインドウが見つかりません。"), gApplicatonName.c_str(), MB_OK | MB_ICONSTOP);
		return;
	}

	curpos = m_OperationSel.GetCurSel();
//	setPointVector(curpos ,0 ,0);

	gWorkIndex = m_OperationSel.GetCurSel();

	g_Operation = 1;
	m_Start.EnableWindow(FALSE);
	m_Stop.EnableWindow(TRUE);
	m_Stop.SetFocus();
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 停止 ボタン押された
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::OnBnClickedButtonStop()
{
	g_Operation = 0;
	m_Start.EnableWindow(TRUE);
	m_Start.SetFocus();
	m_Stop.EnableWindow(FALSE);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  サーチボタン
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::OnBnClickedButtonComserch()
{
	int32_t i;
	CString str;

	GnrlComList::setGuiComList(m_ComPortCombo, gCom);

	for (i = 0; i<m_ComPortCombo.GetCount(); i++){
		m_ComPortCombo.GetLBText(i, str);
		if(str.Find(_T("Arduino")) >=0){
			m_ComPortCombo.SetCurSel(i);
		}
	}
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 接続ボタン押された
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::OnBnClickedButtonComConnect()
{
	if(gCom.isOpened()){
		closeCom();
	}else{
		openCom();
	}
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// COM のオープンクローズ
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::openCom()
{
	int32_t comno;

	comno = GnrlComList::getGuiPortNo(m_ComPortCombo);
	if(comno < 0) return;
	gCom.putParameter(comno ,115200 ,400 ,GnrlCom::ESTOPBIT_1 ,GnrlCom::EPARITY_no);

	gCom.openAndSetParam();

	if(gCom.isBad()){
		gCom.close();
	}
	else{
		mButtonConnect.SetWindowText(L"切断");
	}

	UpdateData(FALSE);

//	mButtonConnect.SetBackGroundColor((RGB(255,0,0));//バックカラー
}
void CAutoPointingDlg::closeCom()
{
	gCom.close();

	mButtonConnect.SetWindowText(L"接続");

	UpdateData(FALSE);
}


void CAutoPointingDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
//	mButtonConnect.GetDC()->SetBkColor(RGB(0xFF, 0, 0));

	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


HBRUSH CAutoPointingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: ここで DC の属性を変更してください。
	if(pWnd == &mButtonConnect){
	
		if(gCom.isOpened()){
			return m_CbrCom[1];
		}else{
			return m_CbrCom[0];			
		}
	}

	// TODO: 既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 終了時刻設定
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::OnBnClickedButtonNow()
{
#if 0
	time_t now,endtime;
    struct tm *s_tm;

	if (time(&now) == (time_t)-1) {
		m_StrEndTime = "Error";
		m_EndTime = (time_t)-1;
	}else{
		
		endtime = now + 3600*2 + 360*8;
		m_EndTime = endtime;
		setGuiEndtime(0);
	}

	UpdateData(FALSE);
#else

	setEndTime(gNowTime[1]);

#endif
}
void CAutoPointingDlg::OnBnClickedButtonNowshort()
{
#if 0
	time_t now,endtime;
    struct tm *s_tm;

	if (time(&now) == (time_t)-1) {
		m_StrEndTime = "Error";
		m_EndTime = (time_t)-1;
	}else{
		
		endtime = now + 360*2;
		m_EndTime = endtime;
		setGuiEndtime(0);
	}

	UpdateData(FALSE);
#else

	setEndTime(gNowTime[2]);

#endif
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// タイマをセット
// EndTime: now + Time    Timeは60分を100とします。
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::setEndTime(int32_t Time)
{
	time_t now, endtime;

	if (time(&now) == (time_t)-1) {
		m_StrEndTime = "Error";
		m_EndTime = (time_t)-1;
	}
	else{

		endtime = now + 36 * Time;
		m_EndTime = endtime;
		setGuiEndtime(0);
	}

	UpdateData(FALSE);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  + or - 10分
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::OnDeltaposSpin10(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    LPNMHDR   pnhm;
	int		delta;

    pnhm = ( LPNMHDR )pNMUpDown;
    if( pnhm->code == UDN_DELTAPOS ) {
		delta = pNMUpDown->iDelta;
		if(delta < 0){
			// + 方向
			setGuiEndtime(+36 * gSpinTime);
			UpdateData(FALSE);
		}else if(delta > 0){
			// - 方向
			setGuiEndtime(-36 * gSpinTime);
			UpdateData(FALSE);
		}
	}

	*pResult = 0;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// m_EndTime を更新して終了時刻を変更します。
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::setGuiEndtime(int DistanceSec)
{

	if(m_EndTime == (time_t)-1){
		return;
	}
	m_EndTime +=DistanceSec;
#if 0
	struct tm *s_tm;
	s_tm = localtime(&m_EndTime);
	m_StrEndTime.Format(_T("%d/%02d/%02d %02d:%02d:%02d"), s_tm->tm_year + 1900, s_tm->tm_mon + 1, s_tm->tm_mday, s_tm->tm_hour, s_tm->tm_min, s_tm->tm_sec);
#else
	struct tm s_tm;
	localtime_s(&s_tm, &m_EndTime);
	m_StrEndTime.Format(_T("%d/%02d/%02d %02d:%02d:%02d"), s_tm.tm_year + 1900, s_tm.tm_mon + 1, s_tm.tm_mday, s_tm.tm_hour, s_tm.tm_min, s_tm.tm_sec);
#endif
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ウインドウがアクティブになった
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	if ((nState == WA_ACTIVE) || (nState == WA_CLICKACTIVE)){
//		gAddSleep = 6;		// アクティブになってから少しウエイトして、操作の人間の時間与える
		APD_SleepAppend(gActivePauseTime);

	}

}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// デジタイザのバージョン をメインスレッドへ通知
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void recvVersion(const uint8_t *Data, uint8_t Len)
{
	Uint32 ver = Uint8ArrowToUint32(&Data[2]);

	pAutoPointingDlg->PostMessage(WM_SHOW_VERSION, ver);
}

