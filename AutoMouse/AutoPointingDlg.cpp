// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// AutoPointingDlg.cpp : �����t�@�C��
//
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"
#include "AutoPointing.h"
#include "AutoPointingDlg.h"
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


void APD_SleepAppend(int32_t msc);


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// VS_VERSIONINFO�͈ȉ��̂悤�ɒ�`���Ȃ���
struct VS_VERSIONINFO { 
    WORD  wLength;          // ���̍\���̂̃T�C�Y(2�o�C�g)
    WORD  wValueLength;     // Value�̃T�C�Y(2�o�C�g)
    WORD  wType;            // �o�C�i�����e�L�X�g��(2�o�C�g)
    WCHAR szKey[16];        // "VS_VERSION_INFO\0"��16����(32�o�C�g)
    WORD  Padding1[1];      // 2�o�C�g����č��v40�o�C�g.
    VS_FIXEDFILEINFO Value; // �����Ƀo�[�W�����ԍ�������.
//  WORD  Padding2[];       // �ϒ��̂���C����̍\���̂ł͕\���s��.
//  WORD  Children[];       // ���������܂�Ӗ��̂Ȃ��f�[�^.
};

// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg(unsigned int FiraWareVersion);

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
	DECLARE_MESSAGE_MAP()
public:
private:
public:
	CString m_StrAppVer,m_StrFirVer;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �o�[�W�����\��
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


// AutoPointingDlg �_�C�A���O



CAutoPointingDlg::CAutoPointingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoPointingDlg::IDD, pParent)
	, m_Pointm(_T(""))
	, m_Pointw(_T(""))
	, m_StrEndTime(_T(""))
	, m_EndTime((time_t)-1)
	, m_TargetWindowName(_T(""))
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
	DDX_Control(pDX, IDC_COMBO_COMSEL, m_ComSel);
	DDX_Control(pDX, IDC_BUTTON_COM_CONNECT, mButtonConnect);
	DDX_Text(pDX, IDC_STATIC_END_TIME, m_StrEndTime);
	DDX_Control(pDX, IDC_COMBO_OPERATION, m_OperationSel);
	DDX_Text(pDX, IDC_STATIC_TARGET_WINDOW_NAME, m_TargetWindowName);
	DDV_MaxChars(pDX, m_TargetWindowName, 256);
	DDX_Control(pDX, IDC_BUTTON_NOWLONG, m_NowLong);
	DDX_Control(pDX, IDC_BUTTON_NOWSHORT, m_NotShort);
}

BEGIN_MESSAGE_MAP(CAutoPointingDlg, CDialogEx)
//	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CAutoPointingDlg::OnBnClickedButton1)
	ON_WM_DESTROY()

	// �ǉ�
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
// �E�C���h�E�����ݒ�
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
BOOL CAutoPointingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�


	srand( (unsigned)time( NULL ) );  

	// TODO: �������������ɒǉ����܂��B
	pAutoPointingDlg = this;

	// COM �T�[�`
	OnBnClickedButtonComserch();

	try{
		initParam();
	}
	catch (const std::wstring& e){
//		e.what();	// e.what()		/// _T("�C�j�V�����C�Y���s")
		MessageBox(e.c_str(), gApplicatonName.c_str(), MB_OK | MB_ICONSTOP);		// MB_ICONERROR ,
		return false;
	}

	{ // �����E�C���h�E�ʒu  ��Dialog�̢�v���p�e�B�>�Center��Œl��TRUE�ɂ��Ă����Ȃ��ƒ����ɖ߂���Ă��܂��B

		RECT  rect;
		int32_t width  = GetSystemMetrics(SM_CXSCREEN);		// �X�N���[���̕����擾
		int32_t height = GetSystemMetrics(SM_CYSCREEN);		// �X�N���[���̍������擾


//		::GetClientRect(nullptr ,&mrect);
		GetWindowRect(&rect);
		
		// ���E�C���h�E�̑傫��������
		width -= rect.right  - rect.left;
		height-= rect.bottom - rect.top;

		int32_t x = gWindowPos.x * width / gWindowDenominator ;
		int32_t y = gWindowPos.y * height / gWindowDenominator;

		BOOL aaa = SetWindowPos(this ,x ,y ,0 ,0 ,SWP_NOSIZE|SWP_NOZORDER);
	}


	// �ڑ��{�^���̐F
	m_CbrCom[0].CreateSolidBrush(RGB(0x80, 0x80, 0x80)); 
	m_CbrCom[1].CreateSolidBrush(RGB(0x00, 0x00, 0xFF)); 


	m_TargetWindowName = gTargetWindowName.c_str();



	// **** �e��X���b�h�N�� ****
	gMouseThread.beginThread(MousePointThread ,NULL ,FALSE);

	gOperationThread.beginThread(OperationThread ,NULL ,FALSE);

	gRecvThread.beginThread(RecvRootine ,NULL ,FALSE);


	m_Stop.EnableWindow(FALSE);

	// **** ���[�N ****
#if 0
//	m_OperationSel.AddString(_T("�ʏ�t�F�X"),0);
	m_OperationSel.InsertString(-1,_T("�ʏ�t�F�X"));
	m_OperationSel.InsertString(-1,_T("�����t�F�X"));
	m_OperationSel.InsertString(-1,_T("�������t�F�X"));
	m_OperationSel.InsertString(-1,_T("10�A�K�`��"));
#else
	for (const auto &e : gWorkNames){
		m_OperationSel.InsertString(-1, e.c_str());
	}
#endif
	m_OperationSel.SetCurSel(gWorkIndex);

	{ // now �{�^��
		const std::size_t size = 16;
		TCHAR buf[size];

		toNowText(buf, size ,gNowTime[1]);
		m_NowLong.SetWindowTextW(buf);

		toNowText(buf, size, gNowTime[2]);
		m_NotShort.SetWindowTextW(buf);
	}

	setEndTime(gNowTime[0]);

	SetTimer(ET_1s,1000,0);


	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}
void CAutoPointingDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	g_Operation = 0;

	gMouseThread.endThread();
	gOperationThread.endThread();
	gRecvThread.endThread();

	gMouseThread.isEndThread(1000);
	gOperationThread.isEndThread(1000);
	gRecvThread.isEndThread(1000);

	KillTimer(ET_1s);

	GnrlComList::clearComList();

	clearParam();
//	g_Life = 0;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ���[�U�[���o�[�W�����{�^���N���b�N
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		int ack;

		if(!gCom.isOpened()){
			openCom();
		}

		ack = APD_sendCommand(0x99, 0x99, NULL, 0);
		if(ack < 0){
			CAboutDlg dlgAbout(0xFFFFFFFF);
			dlgAbout.DoModal();		
		}
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
	CDialogEx::OnSysCommand(nID, lParam);
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B
void CAutoPointingDlg::OnPaint()
{
	UpdateData(FALSE);

	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CAutoPointingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �^�C�}�[
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::OnTimer(UINT_PTR nIDEvent)
{
	time_t now;

	if(nIDEvent == ET_1s){
		if ((g_Operation == 0) || (time(&now) == (time_t)-1) || (m_EndTime == (time_t)-1) ) {
			return;
		}
		if (time(&now) == (time_t)-1) {
			return;
		}

		if(m_EndTime < now){
			OnBnClickedButtonStop();
		}
		if (gAddSleep!=0) gAddSleep--;
	}

	CDialogEx::OnTimer(nIDEvent);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ���O���b�Z�[�W
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
//		gAddSleep = 5;
		break;

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


// ���X���b�h����UI�̕\���X�V����ꍇ�B
LRESULT CAutoPointingDlg::OnUpdatedataA(WPARAM wParam, LPARAM lParam)
{
///	this->PostMessage(WM_UPDATEDATA);
	UpdateData(FALSE);

	return 0; // ���b�Z�[�W�ŗL�̖߂�l��Ԃ�
}

void CAutoPointingDlg::OnMousePoint(CString Strm ,CString Strw)
{
	m_Pointm = Strm;
	m_Pointw = Strw;

	this->PostMessage(WM_UPDATEDATA, 0);
}

LRESULT CAutoPointingDlg::OnShowVersion(WPARAM wParam, LPARAM lParam)
{
	CAboutDlg dlgAbout(wParam);
	dlgAbout.DoModal();	

	return 0; // ���b�Z�[�W�ŗL�̖߂�l��Ԃ�
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �{�^�������ꂽ
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
// �^�[�Q�b�g�E�C���h�E�ʒu���擾����B
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t getTargetWindowPos(RECT &Rect)
{
	LPCTSTR winname = gTargetWindowName.c_str();
	HWND hWndChild = ::FindWindowEx(NULL, NULL, NULL, winname);
	if (hWndChild == NULL) {
		return -1;
	}

	::GetWindowRect( hWndChild, &Rect );		//�f�X�N�g�b�v�̃n���h�����炻��(��ʂ�)�傫�����擾

	return 0;
}

int32_t gPointListLen;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �J�n �{�^�������ꂽ
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::OnBnClickedButtonStart()
{
	int32_t curpos,ack;

	if(!gCom.isOpened()){
		openCom();
	}

	if(!gCom.isOpened()){
		Beep(400,400);
		MessageBox(_T("�f�W�^�C�U��������܂���B"), gApplicatonName.c_str(), MB_OK | MB_ICONSTOP);
		return;
	}

	// �f�W�^�C�U�ɉ�ʂ̉𑜓x��ݒ�
	RECT rec;
	HWND hDeskWnd = ::GetDesktopWindow();	//���̊֐��Ńf�X�N�g�b�v�̃n���h�����擾
	::GetWindowRect( hDeskWnd, &rec );		//�f�X�N�g�b�v�̃n���h�����炻��(��ʂ�)�傫�����擾
	AM_setDisplayResolution(rec.right ,rec.bottom);
//	AM_click(321 ,421);

	ack = getTargetWindowPos(rec);
	if(ack < 0){
		MessageBox(_T("�^�[�Q�b�g�E�C���h�E��������܂���B"), gApplicatonName.c_str(), MB_OK | MB_ICONSTOP);
		return;
	}

	curpos = m_OperationSel.GetCurSel();
	setPointVector(curpos ,0 ,0);
//	m_OperationSel.SetFocus();

	gWorkIndex = m_OperationSel.GetCurSel();

	gAddSleep = 0;
	g_Operation = 1;
	m_Start.EnableWindow(FALSE);
	m_Stop.EnableWindow(TRUE);
	m_Stop.SetFocus();
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ��~ �{�^�������ꂽ
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::OnBnClickedButtonStop()
{
	g_Operation = 0;
	m_Start.EnableWindow(TRUE);
	m_Start.SetFocus();
	m_Stop.EnableWindow(FALSE);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  �T�[�`�{�^��
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::OnBnClickedButtonComserch()
{
	int32_t i;
	CString str;

	GnrlComList::setGuiComList(m_ComSel ,gCom);

	for(i=0;i<m_ComSel.GetCount();i++){
		m_ComSel.GetLBText(i, str);
		if(str.Find(_T("Arduino")) >=0){
			m_ComSel.SetCurSel(i);
		}
	}
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �ڑ��{�^�������ꂽ
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
// COM �̃I�[�v���N���[�Y
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::openCom()
{
	int32_t comno;

	comno = GnrlComList::getGuiPortNo(m_ComSel);
	if(comno < 0) return;
	gCom.putParameter(comno ,115200 ,400 ,GnrlCom::ESTOPBIT_1 ,GnrlCom::EPARITY_no);

#if 1
//--mButtonConnect.SetWindowText(L"OpenStart");
//--mButtonConnect.UpdateData(FALSE);
gCom.openAndSetParam();
//--mButtonConnect.SetWindowText(L"Open�I��");
//--mButtonConnect.UpdateData(FALSE);
#endif

	if(gCom.isBad()){
		gCom.close();
	}
	else{
		mButtonConnect.SetWindowText(L"�ؒf");
	}

	UpdateData(FALSE);

//	mButtonConnect.SetBackGroundColor((RGB(255,0,0));//�o�b�N�J���[
}
void CAutoPointingDlg::closeCom()
{
	gCom.close();

	mButtonConnect.SetWindowText(L"�ڑ�");

	UpdateData(FALSE);
}


void CAutoPointingDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
//	mButtonConnect.GetDC()->SetBkColor(RGB(0xFF, 0, 0));

	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}



#if 0
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �`�悷��K�v������Ƃ��ɌĂ΂��
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CString caption;

	switch(nIDCtl){
		case IDC_BUTTON_COM_CONNECT:
	    pDC->DrawFrameControl(&lpDrawItemStruct->rcItem, DFC_BUTTON, DFCS_BUTTONPUSH);
	    pDC->SetTextColor(RGB(0xFF, 0, 0));
        GetDlgItem(nIDCtl)->GetWindowText(caption);
        pDC->DrawText(caption,&lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		break;
	
	default:
		CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
		break;

	}
}

#endif

HBRUSH CAutoPointingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: ������ DC �̑�����ύX���Ă��������B
	if(pWnd == &mButtonConnect){
	
		if(gCom.isOpened()){
			return m_CbrCom[1];
		}else{
			return m_CbrCom[0];			
		}
	}

	// TODO: ����l���g�p�������Ȃ��ꍇ�͕ʂ̃u���V��Ԃ��܂��B
	return hbr;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �I�������ݒ�
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
// �^�C�}���Z�b�g
// EndTime: now + Time    Time��60����100�Ƃ��܂��B
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
//  + or - 10��
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
			// + ����
			setGuiEndtime(+36 * gSpinTime);
			UpdateData(FALSE);
		}else if(delta > 0){
			// - ����
			setGuiEndtime(-36 * gSpinTime);
			UpdateData(FALSE);
		}
	}

	*pResult = 0;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// m_EndTime ���X�V���ďI��������ύX���܂��B
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
// �E�C���h�E���A�N�e�B�u�ɂȂ���
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	if ((nState == WA_ACTIVE) || (nState == WA_CLICKACTIVE)){
		gAddSleep = 6;		// �A�N�e�B�u�ɂȂ��Ă��班���E�G�C�g���āA����̐l�Ԃ̎��ԗ^����
		APD_SleepAppend(6000);

	}

}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �f�W�^�C�U�̃o�[�W���� �����C���X���b�h�֒ʒm
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void recvVersion(const uint8_t *Data, uint8_t Len)
{
	Uint32 ver = Uint8ArrowToUint32(&Data[2]);

	pAutoPointingDlg->PostMessage(WM_SHOW_VERSION, ver);
}

