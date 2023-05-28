#pragma once

#include "stdafx.h"	
#include "resource1.h"
#include "afxwin.h"

// ComSet ダイアログ

class ComSet : public CDialogEx
{
	DECLARE_DYNAMIC(ComSet)

public:
	ComSet(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~ComSet();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_PORTSET };

	const static CString m_ParityList[3];

	int updateData(bool bSaveAndValidate);

	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_PortNo;
	CComboBox m_BaudRate;
	CComboBox m_Data;
	CComboBox m_Parity;
	CComboBox m_StopBit;
	CComboBox m_Frow;

	GnrlCom	m_Com;

	afx_msg void OnBnClickedOk();
};
