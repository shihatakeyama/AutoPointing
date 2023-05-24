
// AutoMouseDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxwin.h"
#include "define.h"

// CAutoMouseDlg �_�C�A���O
class CAutoMouseDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CAutoMouseDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_AUTOMOUSE_DIALOG };

//	void OnUpdatedata();
	void OnMousePoint(CString Strm ,CString Strw);

	enum E_Timer{
		ET_1s = 1
	};

protected:

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g

// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	// �ǉ�
	afx_msg LRESULT OnUpdatedataA(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowVersion(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
private:
public:
private:
	CString m_Pointm;	// ���j�^���W
	CString m_Pointw;	// �E�C���h�E���W
	CString m_StrEndTime;
	CBrush m_CbrCom[3]; 
	time_t	m_EndTime;

public:
	CString m_FirmwareVersion;

public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	CButton m_Start;
	CButton m_Stop;
//--	afx_msg void OnBnClickedButton2();
	// Com�ԍ��I��
	afx_msg void OnBnClickedButtonComserch();
	CComboBox m_ComSel;
	afx_msg void OnBnClickedButtonComConnect();

	void openCom();
	void closeCom();
	CButton mButtonConnect;

	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonNow();
	void setEndtime(int DistanceSec);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg virtual void OnDeltaposSpin10(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_OperationSel;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnBnClickedButtonNowshort();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	CString m_TargetWindowName;
};
