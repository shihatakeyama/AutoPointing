// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// AutoMouseDlg.cpp : �����t�@�C��
//
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"
#include "AutoMouse.h"
#include "AutoMouseDlg.h"
#include "afxdialogex.h"
#include "vector"

#include "Windows.h"

#include "GnrlNumerical.h"
#include "GnrlThread.h"
#include "Define.h"
#define  USE_MFC
#include "GnrlComList.h"
#include "GnrlCom.h"

#include "global.h"
#include "extern.h"
#include "global.h"


int32_t initParam();


CAutoMouseDlg *pCAutoMouseDlg;

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
	
	if(FiraWareVersion == 0xFFFFFFFF){
		m_StrAppVer.Format(_T("Aplication Version: %08X")	, pData->Value.dwProductVersionLS);
		m_StrFirVer		 = _T("Firmware  Version: --------");
	}else{
		m_StrAppVer.Format(_T("Aplication Version: %08X")	, pData->Value.dwProductVersionLS);
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


// CAutoMouseDlg �_�C�A���O



CAutoMouseDlg::CAutoMouseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoMouseDlg::IDD, pParent)
	, m_Pointm(_T(""))
	, m_Pointw(_T(""))
	, m_StrEndTime(_T(""))
	, m_EndTime((time_t)-1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAutoMouseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_POINTM,	m_Pointm);
	DDX_Text(pDX, IDC_STATIC_POINTW,	m_Pointw);
	DDX_Control(pDX, IDC_BUTTON_START, m_Start);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_Stop);
	//  DDX_Control(pDX, IDC_COMBO_COMNO, m_ComNoCombo);
	DDX_Control(pDX, IDC_COMBO_COMSEL, m_ComSel);
	DDX_Control(pDX, IDC_BUTTON_COM_CONNECT, mButtonConnect);
	DDX_Text(pDX, IDC_STATIC_END_TIME, m_StrEndTime);
	DDX_Control(pDX, IDC_COMBO_OPERATION, m_OperationSel);
}

BEGIN_MESSAGE_MAP(CAutoMouseDlg, CDialogEx)
//	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CAutoMouseDlg::OnBnClickedButton1)
	ON_WM_DESTROY()

	// �ǉ�
	ON_MESSAGE(WM_UPDATEDATA, OnUpdatedataA)
	ON_MESSAGE(WM_SHOW_VERSION, OnShowVersion)

	ON_BN_CLICKED(IDC_BUTTON_START, &CAutoMouseDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CAutoMouseDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON2, &CAutoMouseDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_COMSERCH, &CAutoMouseDlg::OnBnClickedButtonComserch)
	ON_BN_CLICKED(IDC_BUTTON_COM_CONNECT, &CAutoMouseDlg::OnBnClickedButtonComConnect)
	ON_WM_DRAWITEM()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_NOW, &CAutoMouseDlg::OnBnClickedButtonNow)
	ON_WM_TIMER()
	ON_WM_SYSCOMMAND()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_10, &CAutoMouseDlg::OnDeltaposSpin10)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_BUTTON_NOWSHORT, &CAutoMouseDlg::OnBnClickedButtonNowshort)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CAutoMouseDlg ���b�Z�[�W �n���h���[

BOOL CAutoMouseDlg::OnInitDialog()
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
	pCAutoMouseDlg = this;

	initParam();

	// �ڑ��{�^���̐F
	m_CbrCom[0].CreateSolidBrush(RGB(0x80, 0x80, 0x80)); 
	m_CbrCom[1].CreateSolidBrush(RGB(0x00, 0x00, 0xFF)); 


	OnBnClickedButtonComserch();

	// **** �e��X���b�h�N�� ****
	gMouseThread.beginThread(MousePointThread ,NULL ,FALSE);

	gOperationThread.beginThread(OperationThread ,NULL ,FALSE);

	gRecvThread.beginThread(RecvThread ,NULL ,FALSE);


	m_Stop.EnableWindow(FALSE);

//	m_OperationSel.AddString(_T("�ʏ�t�F�X"),0);
	m_OperationSel.InsertString(-1,_T("�ʏ�t�F�X"));
	m_OperationSel.InsertString(-1,_T("�����t�F�X"));
	m_OperationSel.InsertString(-1,_T("�������t�F�X"));
	m_OperationSel.InsertString(-1,_T("10�A�K�`��"));
	m_OperationSel.SetCurSel(0);

	OnBnClickedButtonNow();

	SetTimer(ET_1s,1000,0);


	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}
void CAutoMouseDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	gMouseThread.endThread();
	gOperationThread.endThread();
	gRecvThread.endThread();

	gMouseThread.isEndThread(1000);
	gOperationThread.isEndThread(1000);
	gRecvThread.isEndThread(1000);

	KillTimer(ET_1s);

	GnrlComList::clearComList();

//	g_Life = 0;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ���[�U�[���o�[�W�����N���b�N
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoMouseDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		int comno,ack;

		if(!gCom.isOpened()){
			openCom();
		}

		ack = sendCommand(0x99 ,0x99 ,NULL ,0);
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
void CAutoMouseDlg::OnPaint()
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
HCURSOR CAutoMouseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �^�C�}�[
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoMouseDlg::OnTimer(UINT_PTR nIDEvent)
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
BOOL CAutoMouseDlg::PreTranslateMessage(MSG* pMsg)
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
LRESULT CAutoMouseDlg::OnUpdatedataA(WPARAM wParam, LPARAM lParam)
{
///	this->PostMessage(WM_UPDATEDATA);
	UpdateData(FALSE);

	return 0; // ���b�Z�[�W�ŗL�̖߂�l��Ԃ�
}

void CAutoMouseDlg::OnMousePoint(CString Strm ,CString Strw)
{
	m_Pointm = Strm;
	m_Pointw = Strw;

	this->PostMessage(WM_UPDATEDATA, 0);
}

LRESULT CAutoMouseDlg::OnShowVersion(WPARAM wParam, LPARAM lParam)
{
/*	HRSRC hRsrc = FindResource(NULL,(LPCTSTR)VS_VERSION_INFO,RT_VERSION);
	HGLOBAL hGlobal = LoadResource(NULL,hRsrc);
	struct VS_VERSIONINFO *pData = (VS_VERSIONINFO*)LockResource(hGlobal);

	CString str;
	CString strcaption(L"AutoMouse");	///
	

	str.Format(L"Aplication Version: %08X\nFirmware Version: %08X" ,pData->Value.dwFileVersionMS , wParam);
	MessageBox(str ,_T("AutoMouse") ,MB_OK | MB_ICONINFORMATION);
	*/

	CAboutDlg dlgAbout(wParam);
	dlgAbout.DoModal();	

	return 0; // ���b�Z�[�W�ŗL�̖߂�l��Ԃ�
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �{�^�������ꂽ
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoMouseDlg::OnBnClickedButton1()
{
	int comno,ack;

	if(!gCom.isOpened()){
		openCom();
	}

	ack = sendCommand(0x99 ,0x99 ,NULL ,0);
	if(ack < 0){
		
	}
}

void CAutoMouseDlg::OnBnClickedButton2()
{
	int i;
	WINDOWINFO windowInfo;

#if 1	// �E�C���h�E�����w�肵�Ă��̃E�C���h�E�����B
	LPCTSTR winname = _T("DOAX VenusVacation");
	HWND hWndChild = ::FindWindowEx(NULL, NULL, NULL, winname);
	if (hWndChild == NULL) {
		return;
	}
//		::SendMessage(hWndChild, WM_CLOSE, 0, 0);
//		::SendMessage(hWndChild, WM_DESTROY, 0, 0);

	RECT rec;

	::GetWindowRect( hWndChild, &rec );		//�f�X�N�g�b�v�̃n���h�����炻��(��ʂ�)�傫�����擾
	windowInfo.cbSize = sizeof(WINDOWINFO);
	::GetWindowInfo(hWndChild, &windowInfo);
    rec.bottom; 
    rec.left;
    rec.right;
    rec.top;

#endif

}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// DOAX�̃E�C���h�E�ʒu���擾����B
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t getDoaxWindowPos(RECT &Rect)
{
	LPCTSTR winname = _T("DOAX VenusVacation");
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
void CAutoMouseDlg::OnBnClickedButtonStart()
{
	int32_t curpos,ack;

	if(!gCom.isOpened()){
		openCom();
	}

	if(!gCom.isOpened()){
		Beep(400,400);
		MessageBox(_T("�f�W�^�C�U��������܂���B") ,WINDOW_NAME ,MB_OK | MB_ICONSTOP);
		return;
	}

	// �f�W�^�C�U�ɉ�ʂ̉𑜓x��ݒ�
	RECT rec;
	HWND hDeskWnd = ::GetDesktopWindow();	//���̊֐��Ńf�X�N�g�b�v�̃n���h�����擾
	::GetWindowRect( hDeskWnd, &rec );		//�f�X�N�g�b�v�̃n���h�����炻��(��ʂ�)�傫�����擾
	AM_setDisplayResolution(rec.right ,rec.bottom);
//	AM_click(321 ,421);

	ack = getDoaxWindowPos(rec);
	if(ack < 0){
		MessageBox(_T("DAOX�E�C���h�E��������܂���B") ,WINDOW_NAME ,MB_OK | MB_ICONSTOP);
		return;
	}

	curpos = m_OperationSel.GetCurSel();
	setPointVector(curpos ,0 ,0);
//	m_OperationSel.SetFocus();

	gAddSleep = 0;
	g_Operation = 1;
	m_Start.EnableWindow(FALSE);
	m_Stop.EnableWindow(TRUE);
	m_Stop.SetFocus();
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ��~ �{�^�������ꂽ
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoMouseDlg::OnBnClickedButtonStop()
{
	g_Operation = 0;
	m_Start.EnableWindow(TRUE);
	m_Start.SetFocus();
	m_Stop.EnableWindow(FALSE);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  �T�[�`�{�^��
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoMouseDlg::OnBnClickedButtonComserch()
{
	int32_t i;
	CString str;

	GnrlComList::putComList(m_ComSel);

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
void CAutoMouseDlg::OnBnClickedButtonComConnect()
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
void CAutoMouseDlg::openCom()
{
	int32_t ack,comno;

	comno = GnrlComList::getComPortNo(m_ComSel);
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
void CAutoMouseDlg::closeCom()
{
	gCom.close();

	mButtonConnect.SetWindowText(L"�ڑ�");

	UpdateData(FALSE);
}


void CAutoMouseDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
//	mButtonConnect.GetDC()->SetBkColor(RGB(0xFF, 0, 0));

	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}



#if 0
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �`�悷��K�v������Ƃ��ɌĂ΂��
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoMouseDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
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

HBRUSH CAutoMouseDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CAutoMouseDlg::OnBnClickedButtonNow()
{
	time_t now,endtime;
    struct tm *s_tm;

	if (time(&now) == (time_t)-1) {
		m_StrEndTime = "Error";
		m_EndTime = (time_t)-1;
	}else{
		
		endtime = now + 3600*2 + 360*8;
		m_EndTime = endtime;
		setEndtime(0);
	}

	UpdateData(FALSE);
}
void CAutoMouseDlg::OnBnClickedButtonNowshort()
{
	time_t now,endtime;
    struct tm *s_tm;

	if (time(&now) == (time_t)-1) {
		m_StrEndTime = "Error";
		m_EndTime = (time_t)-1;
	}else{
		
		endtime = now + 360*2;
		m_EndTime = endtime;
		setEndtime(0);
	}

	UpdateData(FALSE);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// +-10��
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoMouseDlg::OnDeltaposSpin10(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B

    LPNMHDR   pnhm;
	int		delta;

    pnhm = ( LPNMHDR )pNMUpDown;
    if( pnhm->code == UDN_DELTAPOS ) {
		delta = pNMUpDown->iDelta;
		if(delta < 0){
			// �����
			setEndtime(+60*10);
			UpdateData(FALSE);
		}else if(delta > 0){
			// ������
			setEndtime(-60*10);
			UpdateData(FALSE);
		}
	}

	*pResult = 0;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// m_EndTime ���X�V���ďI��������ύX���܂��B
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoMouseDlg::setEndtime(int DistanceSec)
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
void CAutoMouseDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	if ((nState == WA_ACTIVE) || (nState == WA_CLICKACTIVE)){
		gAddSleep = 6;		// �A�N�e�B�u�ɂȂ��Ă��班������̐l�Ԃ̎��ԗ^����

	}

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
}
