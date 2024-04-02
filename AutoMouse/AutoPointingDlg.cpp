// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// AutoPointingDlg.cpp : �����t�@�C��
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
//	, m_Pointm(_T(""))
//	, m_Pointw(_T(""))
	, m_StrEndTime(_T(""))
	, m_EndTime((time_t)-1)
	, m_TargetWindowName(_T(""))
//--	, m_Delay(_T("0.000"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAutoPointingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_STATIC_POINTM, m_Pointm);
	//  DDX_Text(pDX, IDC_STATIC_POINTW, m_Pointw);
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
	//  DDX_Text(pDX, IDC_STATIC_DELAY, m_Delay);
	DDX_Control(pDX, IDC_STATIC_DELAY, m_Delay);
	DDX_Control(pDX, IDC_STATIC_POINTM, m_Pointm);
	DDX_Control(pDX, IDC_STATIC_POINTW, m_Pointw);
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
	ON_MESSAGE(WM_COMMENT	,OnComment)

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
		// Load/Save
		pSysMenu->InsertMenu(1, MF_SEPARATOR);
		pSysMenu->InsertMenu(1, MF_STRING, IDC_LOAD_XML, _T("LOAD"));
		pSysMenu->InsertMenu(1, MF_STRING, IDC_SAVE_XML, _T("SAVE"));
		pSysMenu->InsertMenu(1, MF_STRING, IDC_TARGET_SIZE, _T("TARGET SIZE"));
//++		pSysMenu->InsertMenu(1, MF_STRING, IDC_ADVANCE, _T("ADVANCE"));

		// �o�[�W�������
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


	uint32_t seed  = time( NULL );
	srand(seed);  


	// TODO: �������������ɒǉ����܂��B
	pAutoPointingDlg = this;

	loadXmlIni();

	// COM �T�[�`
	OnBnClickedButtonComserch();
#if 0
	try{
		initParam();
	}
	catch (const std::wstring& e){
//		e.what();	// e.what()		/// _T("�C�j�V�����C�Y���s")
		MessageBox(e.c_str(), gApplicatonName.c_str(), MB_OK | MB_ICONSTOP);		// MB_ICONERROR ,
		return false;
	}
#endif

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

		BOOL aaa = SetWindowPos(this ,x ,y ,0 ,0 ,0);	// SWP_NOSIZE|SWP_NOZORDER
	}


	// �ڑ��{�^���̐F
	m_CbrCom[0].CreateSolidBrush(RGB(0x80, 0x80, 0x80)); 
	m_CbrCom[1].CreateSolidBrush(RGB(0x00, 0x00, 0xFF)); 

	// �^�[�Q�b�g�E�C���h�E����GUI�֔��f
#if SPY_MODE == 0
	m_TargetWindowName = gTargetWindowName.c_str();
#elif SPY_MODE == 1
	m_TargetWindowName = gTargetWindowName[0];	// �O������̃n�b�L���O�΍� 1�����̂ݔ��f
#endif

	// �_�C�A���O�E�C���h�E���ݒ�
#if SPY_MODE == 0
	SetWindowText(_T("AutoPointing"));
#elif SPY_MODE == 1
	SetWindowText(_T("AP"));
#endif



	// **** �e��X���b�h�N�� ****
//--	gMouseThread.beginThread(MousePointThread ,NULL ,FALSE);



	gOperationThread.beginThread(OperationThread ,NULL ,FALSE);

	gRecvThread.beginThread(RecvRootine ,NULL ,FALSE);


	m_Stop.EnableWindow(FALSE);


	{ // now �{�^��
		const std::size_t size = 16;
		TCHAR buf[size];

		toNowText(buf, size ,gNowTime[1]);
		m_NowLong.SetWindowTextW(buf);

		toNowText(buf, size, gNowTime[2]);
		m_NotShort.SetWindowTextW(buf);
	}

	setEndTime(gNowTime[0]);

	SetTimer(ET_100ms, 100, 0);


	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}
void CAutoPointingDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	g_Operation = 0;

	saveXmlIni();

	gMouseThread.endThread();
	gOperationThread.endThread();
	gRecvThread.endThread();

	gMouseThread.isEndThread(1000);
	gOperationThread.isEndThread(1000);
	gRecvThread.isEndThread(1000);

	KillTimer(ET_100ms);

	GnrlComList::clearComList();


	WorkBase::clearProcList(gWorks);
	gWorkNames.clear();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// XML save/load
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t CAutoPointingDlg::loadXmlIni()
{
	int32_t ack;
	TCHAR InitFilePath[MAX_PATH];

	ack = GnrlFilepath::getModuleAttachmentFilePath(InitFilePath, MAX_PATH, _T("apd_ini.xml"));
	if (ack <= 0)	throw std::wstring(_T("apd_ini.xml ������܂���"));

	return loadXml(InitFilePath);
}
int32_t CAutoPointingDlg::loadXmlGui()
{
	CFileDialog dlgFile(TRUE);
	OPENFILENAME &ofn = dlgFile.GetOFN();
	ofn.lpstrTitle = _T("�p�����[�^���J��");
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
	std::wstring errmsg;

	// �r��
	std::lock_guard<std::mutex> lock(gWorkMutex);

	try{
		ack = rapidxml::load_document(doc, Path, docbuf);
		if (ack < 0){ throw std::wstring(_T("load document")); return ERC_ng; }
	}
	catch (const std::wstring& e){
//		std::wstring  str(_T("�f�[�^�ǂݍ��ݎ��s\n"));
//		str += e;
//		throw str;
		errmsg = _T("�f�[�^�ǂݍ��ݎ��s\n") + e;
	}
	catch (const rapidxml::parse_error &e){
//		std::wstring  str(_T("XML �f�[�^�ǂݍ��ݎ��s\n"));
//		str += CString(e.what()) + '\n' + e.where<rapidxml::char_t>();
//		throw str;
		errmsg = _T("XML �f�[�^�ǂݍ��ݎ��s\n") + CString(e.what()) + '\n' + e.where<rapidxml::char_t>();
	}

	if(!errmsg.empty()){
		MessageBox(errmsg.c_str(), gApplicatonName.c_str(), MB_OK);
		return ERC_ng;
	}

	// **** �e��p�����[�^�ǂݍ��� ****

	rapidxml::node_t *root = doc.first_node();
	if(root == nullptr){
		throw std::wstring(_T("XML �̏��������Ă��܂��B"));
	}


	rapidxml::node_t *node;
	if (root->comp_node_name(gApplicatonName.c_str()) != 0){
		throw std::wstring(_T("XML �̏������Ⴂ�܂�"));
	}

	root->first_node(_T("title"), gTitle);

	node = root->first_node(_T("target"));	//  rapidxml::first_node(root, _T("target"));
	if (node){
		node->first_attribute(_T("window_name"), gTargetWindowName);
		node->first_attribute(_T("x"), val);	gBasePoint.x = val;
		node->first_attribute(_T("y"), val);	gBasePoint.y = val;
	}

	// �E�C���h�E�����܂�`�F�b�N
	node = root->first_node(_T("inside"));	//   rapidxml::first_node(root, _T("inside"));
	if (node){
		node->first_attribute_check(gInsideCheck);
		node->first_attribute(_T("margin_x"), gInsideMargin.x);
		node->first_attribute(_T("margin_y"), gInsideMargin.y);
	}

	// �E�C���h�E�\�������ʒu
	node = root->first_node(_T("window"));	//   rapidxml::first_node(root, _T("window"));
	if (node){
		rapidxml::attribute_t *attr;

		attr = node->first_attribute(_T("vpos"), val);
		if (attr){
			gWindowPos.x = val;
		}

		attr = node->first_attribute(_T("hpos"), val);
		if (attr){
			gWindowPos.y = val;
		}
	}

	// COM
	node = root->first_node(_T("com"));
	if (node){
		gCom.loadXmlNode(node);
	}

	// �u��
	node = root->first_node(_T("blur"));
	if (node == nullptr){
		gBurePoint = CPoint(4, 4);
		gBureTime = 4;
	}
	else{
		node->first_attribute(_T("x"), val);	gBurePoint.x = val;
		node->first_attribute(_T("y"), val);	gBurePoint.y = val;
		node->first_attribute(_T("time"), val);	gBureTime = val;
	}

	// �A�N�e�B�u���̃|�[�Y
	node = root->first_node(_T("active"));
	if (node){
		node->first_attribute(_T("pause_time"), gActivePauseTime);
	}

	// �I������
	node = root->first_node(_T("end_time"));
	if (node == nullptr){
	}
	else{
		node->first_attribute(_T("time0"), gNowTime[0]);
		node->first_attribute(_T("time1"), gNowTime[1]);
		node->first_attribute(_T("time2"), gNowTime[2]);
		node->first_attribute(_T("spin") , gSpinTime);
	}

	// ���[�N�ǂݍ���
	rapidxml::node_t *work = root->first_node(_T("works"));
	if (work == nullptr){
		throw std::wstring(_T("XML ��works �^�O������܂���B"));
	}

	work->first_attribute(_T("index"), gWorkIndex);
	ack = WorkBase::loadWorkList(gWorks, work, gWorkNames);
	if (gWorkIndex >= static_cast<int32_t>(gWorks.size())){
		gWorkIndex = 0;
	}

	// **** ���[�N ****
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
	if (ack <= 0)	throw std::wstring(_T("apd_ini.xml ������܂���"));

	return saveXml(InitFilePath);
}
int32_t CAutoPointingDlg::saveXmlGui()
{
	CFileDialog dlgFile(FALSE);
	OPENFILENAME &ofn = dlgFile.GetOFN();
	ofn.lpstrTitle = _T("�p�����[�^��ۑ�");
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
	rapidxml::document_t doc;
	rapidxml::node_t *root = doc.allocate_node(rapidxml::node_element ,gApplicatonName.c_str());
	rapidxml::node_t *node;

	root->append_attribute(doc, _T("version"), _T("1.0"));
	root->append_attribute(doc, _T("encoding"), _T("utf-8"));

	// �r��
	std::lock_guard<std::mutex> lock(gWorkMutex);

	// **** �e��p�����[�^�������� ****
	root->append_node(doc, _T("title"), gTitle);

	root->append_node_hex(doc, _T("soft_version"), SOFT_VERSION);

	node = root->append_node(doc, _T("target"));

	node->append_attribute(doc, _T("window_name"), gTargetWindowName);
	node->append_attribute(doc, _T("x"), gBasePoint.x);
	node->append_attribute(doc, _T("y"), gBasePoint.y);

	// �E�C���h�E�����܂�`�F�b�N
	node = root->append_node(doc, _T("inside"));
	node->append_attribute_check(doc, gInsideCheck);
	node->append_attribute(doc, _T("margin_x"), gInsideMargin.x);
	node->append_attribute(doc, _T("margin_y"), gInsideMargin.y);

	// �E�C���h�E�\�������ʒu
	{ // �����E�C���h�E�ʒu  ��Dialog�̢�v���p�e�B�>�Center��Œl��TRUE�ɂ��Ă����Ȃ��ƒ����ɖ߂���Ă��܂��B

		RECT  rect;
		int32_t width  = GetSystemMetrics(SM_CXSCREEN);		// �X�N���[���̕����擾
		int32_t height = GetSystemMetrics(SM_CYSCREEN);		// �X�N���[���̍������擾

		GetWindowRect(&rect);
		
		// ���E�C���h�E�̑傫��������
		width -= rect.right  - rect.left;
		height-= rect.bottom - rect.top;

		gWindowPos.x	= rect.left * gWindowDenominator / width;
		gWindowPos.y	= rect.top  * gWindowDenominator / height;
	}
//	node = rapidxml::append_node(doc, root, _T("window"));
	node = root->append_node(doc, _T("window"));
//	rapidxml::append_attribute(doc, node, _T("denominator"), gWindowDenominator);
	node->append_attribute(doc, _T("vpos"), gWindowPos.x);
	node->append_attribute(doc, _T("hpos"), gWindowPos.y);

	// COM
	GnrlComList::getGuiPortNo(m_ComPortCombo, gCom);
	gCom.saveXmlNode(doc ,node);
	node->name(_T("com"));
//--	rapidxml::append_node(root, node);
	root->append_node(node);

	// �u��
//	node = rapidxml::append_node(doc ,root , _T("blur"));
	node = root->append_node(doc, _T("blur"));
	node->append_attribute(doc, _T("x"), gBurePoint.x);
	node->append_attribute(doc, _T("y"), gBurePoint.y);
	node->append_attribute(doc, _T("time"), gBureTime);

	// �A�N�e�B�u���̃|�[�Y
	node = root->append_node(doc, _T("active"));
	node->append_attribute(doc, _T("pause_time"), gActivePauseTime);

	// �I������
	node = root->append_node(doc ,_T("end_time"));
	node->append_attribute(doc, _T("time0"), gNowTime[0]);
	node->append_attribute(doc, _T("time1"), gNowTime[1]);
	node->append_attribute(doc, _T("time2"), gNowTime[2]);
	node->append_attribute(doc, _T("spin"), gSpinTime);

	// ���[�N
	node = root->append_node(doc ,_T("works"));
	gWorkIndex = m_OperationSel.GetCurSel();
	node->append_attribute(doc, _T("index"), gWorkIndex);
	ack = WorkBase::saveWorkList(gWorks, doc ,node, gWorkNames);

	// XML��������
	doc.append_node(root);
	ack = rapidxml::save_document(doc, Path);

	return ack;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ���[�U�[���E�C���h�E�T�C�Y/�ʒu�\���v��
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::showTargetSize()
{
	RECT rec;
	int32_t ack;
	CString	txt;

	ack = getTargetWindowPos(rec);
	if(ack < 0){
		txt = _T("----");
	}else{
		txt.Format(_T("x=%d y=%d h=%d w=%d") ,rec.right ,rec.top ,rec.bottom-rec.top+1 ,rec.right-rec.left+1);
	}

	::SetDlgItemText(m_hWnd, IDC_STATIC_COMMENT, txt.GetBuffer(0));
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �E�C���h�E�T�C�Y�ύX�\�ɂ��܂��B
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::setAdvance()
{
	
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ���[�U�[���o�[�W�����{�^���N���b�N
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
	case IDC_TARGET_SIZE:
		showTargetSize();
		break;
	case IDC_ADVANCE:
		setAdvance();
		break;
	default:
		CDialogEx::OnSysCommand(nID, lParam);
	}
//--	CDialogEx::OnSysCommand(nID, lParam);
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
// �����^�C�}�[
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::OnTimer(UINT_PTR nIDEvent)
{
	time_t now;
	static int32_t delay = 0;
	static POINT oldpoint = { 0, 0 };
	Sint32 ack;

	if (nIDEvent == ET_100ms){
		// **** Delay �\�� ****
		CString str;
		if (delay != gDelayRemine){
			delay = gDelayRemine;
			str.Format(_T("%4d.%03d"), delay / 1000, delay % 1000);
			m_Delay.SetWindowText(str);
			m_Delay.UpdateData(false);
		}

		// **** ���W�\�� ****
		RECT rect;
		POINT point;

		GetCursorPos(&point);

		if ((point.x != oldpoint.x) || (point.y != oldpoint.y)){

			str.Format(_T("x=%5d , y=%5d"), point.x, point.y);
			m_Pointm.SetWindowText(str);
			m_Pointm.UpdateData(false);

			ack = getTargetWindowPos(rect);
			if (ack < 0){
				str = _T("-------  -------");
			}
			else{
				str.Format(_T("x=%5d , y=%5d"), point.x - rect.left, point.y - rect.top);
			}
			m_Pointw.SetWindowText(str);
			m_Pointw.UpdateData(false);

			oldpoint = point;
		}

		// **** �I������ ****
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

LRESULT CAutoPointingDlg::OnShowVersion(WPARAM wParam, LPARAM lParam)
{
	CAboutDlg dlgAbout(wParam);
	dlgAbout.DoModal();	

	return 0; // ���b�Z�[�W�ŗL�̖߂�l��Ԃ�
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �R�����g�\��
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void WorkBase::SendComment(const std::wstring &Txt)
{
	const WCHAR *pc = Txt.c_str();

	SendMessageA(pAutoPointingDlg->m_hWnd, WM_COMMENT, reinterpret_cast<WPARAM>(pc), 0);
}

LRESULT CAutoPointingDlg::OnComment(WPARAM wParam, LPARAM lParam)
{
	::SetDlgItemText(m_hWnd, IDC_STATIC_COMMENT, reinterpret_cast<WCHAR*>(wParam));


	return 0;
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

	return ERC_ok;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �J�n �{�^�������ꂽ
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::OnBnClickedButtonStart()
{
	int32_t curpos,ack;


	if (gWorks.size() < gWorkIndex){
		MessageBox(_T("�ғ�������������܂���B"), gApplicatonName.c_str(), MB_OK | MB_ICONSTOP);
		return;
	}


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

#if 1	// �^�[�Q�b�g�E�C���h�E�����݂��邩�`�F�b�N
	ack = getTargetWindowPos(rec);
	if(ack < 0){
		MessageBox(_T("�^�[�Q�b�g�E�C���h�E��������܂���B"), gApplicatonName.c_str(), MB_OK | MB_ICONSTOP);
		return;
	}
#endif

	curpos = m_OperationSel.GetCurSel();

	gWorkIndex = m_OperationSel.GetCurSel();

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

	GnrlComList::setGuiComList(m_ComPortCombo, gCom);

	for (i = 0; i<m_ComPortCombo.GetCount(); i++){
		m_ComPortCombo.GetLBText(i, str);
		if(str.Find(_T("Arduino")) >=0){
			m_ComPortCombo.SetCurSel(i);
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

	comno = GnrlComList::getGuiPortNo(m_ComPortCombo);
	if(comno < 0) return;
//	gCom.putParameter(comno ,115200 ,400 ,GnrlCom::ESTOPBIT_1 ,GnrlCom::EPARITY_no);
	gCom.putComNo(comno);

	gCom.openAndSetParam();

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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �I�[�i�[�`��
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
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
	setEndTime(gNowTime[1]);
}
void CAutoPointingDlg::OnBnClickedButtonNowshort()
{
	setEndTime(gNowTime[2]);
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

	struct tm s_tm;
	localtime_s(&s_tm, &m_EndTime);
	m_StrEndTime.Format(_T("%d/%02d/%02d %02d:%02d:%02d"), s_tm.tm_year + 1900, s_tm.tm_mon + 1, s_tm.tm_mday, s_tm.tm_hour, s_tm.tm_min, s_tm.tm_sec);
}

#include "MyTemplate.h"

typedef MyTemplateA<TCHAR> mta_t;

template <class T>
void MyTemplate<T>::printValue(T value) {
	std::cout << "Value: " << value << std::endl;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �E�C���h�E���A�N�e�B�u�ɂȂ���
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void CAutoPointingDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	if ((nState == WA_ACTIVE) || (nState == WA_CLICKACTIVE)){
//		gAddSleep = 6;		// �A�N�e�B�u�ɂȂ��Ă��班���E�G�C�g���āA����̐l�Ԃ̎��ԗ^����
		APD_SleepAppend(gActivePauseTime);


		mta_t mt;

		mt.printValueA(44);

	}

}



//template class MyTemplate<int>;  // int�^�ɑ΂����̉�





// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �f�W�^�C�U�̃o�[�W���� �����C���X���b�h�֒ʒm
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void recvVersion(const uint8_t *Data, uint8_t Len)
{
	Uint32 ver = Uint8ArrowToUint32(&Data[2]);

	pAutoPointingDlg->PostMessage(WM_SHOW_VERSION, ver);


}
