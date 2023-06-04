// D:\My Documents\Visual Studio 2013\Projects\SignalProcDlg\SignalProcDlg\com\ComSet.cpp : 実装ファイル
//

#include "stdafx.h"
//--#include "SignalProcDlg.h"
#include "ComSet.h"
#include "afxdialogex.h"


// ComSet ダイアログ

const CString ComSet::m_ParityList[3] = {_T("奇数"), _T("偶数"), _T("無し") };


IMPLEMENT_DYNAMIC(ComSet, CDialogEx)

ComSet::ComSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(ComSet::IDD, pParent)
{}

ComSet::~ComSet()
{
}

void ComSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORTNO, m_PortNo);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_BaudRate);
	//  DDX_Control(pDX, IDC_COMBO_DATA, m_DataBit);
	DDX_Control(pDX, IDC_COMBO_PARITY, m_Parity);
	DDX_Control(pDX, IDC_COMBO_FROW, m_Frow);
	DDX_Control(pDX, IDC_COMBO_DATA, m_Data);
	DDX_Control(pDX, IDC_COMBO_STOP, m_StopBit);
}


BEGIN_MESSAGE_MAP(ComSet, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &ComSet::OnBnClickedOk)
END_MESSAGE_MAP()


// ComSet メッセージ ハンドラー


BOOL ComSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	updateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void ComSet::OnBnClickedOk()
{
	updateData(TRUE);

	CDialogEx::OnOK();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  設定ファイルから値を読みだしてGUIに反映する
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int ComSet::updateData(bool bSaveAndValidate)
{

	if (bSaveAndValidate){

		UpdateData(TRUE);

		GnrlComList::getGuiComList(m_PortNo ,m_Com);

		m_Com.getGuiBaudRate(m_BaudRate);
		m_Com.getGuiDataBit(m_Data);
		m_Com.getGuiParity(m_Parity);
		m_Com.getGuiFrow(m_Frow);
	}
	else{
		GnrlComList::setGuiComList(m_PortNo ,m_Com);	

		m_Com.setGuiBaudRate(m_BaudRate);
		m_Com.setGuiData(m_Data);
		m_Com.setGuiParity(m_Parity);
		m_Com.setGuiStopBit(m_StopBit);
		m_Com.setGuiFrow(m_Frow);

		UpdateData(FALSE);
	}

	return 0;
}
