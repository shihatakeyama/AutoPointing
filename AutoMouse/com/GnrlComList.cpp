// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  
//  FILE        :GnrlComList.cpp		                               
//  DATE        :2014/07/04	
//  
//  PC���g�p�\��COM�|�[�g�ԍ��ƁA�t���l�[�����擾����
//    ��������(�ϐ�)�Ɋi�[���܂��B
//	
//  USE :Win32API,MFC
// 
// 2019/12/18	COM�ԍ���������o�b�t�@�T�C�Y���Ńt�@�C���Œ�`
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


#include "stdafx.h"

#include "setupapi.h"	// setupapi.lib���g�p
#include "GnrlString.h"
#include "GnrlCom.h"

#include "GnrlComList.h"
#pragma	comment(lib,"setupapi.lib")


#define COM_NAME_BUF_SIZE	16		// "COM1" �Ȃǂ������

namespace GnrlComList{

	unsigned char gComPortNoList[MAX_COMPORT_LIST];	// Com�|�[�g�ԍ����X�g  �i�Ⴂ�v�f�ԍ��l�߁j
	TCHAR *gComPortNameList[MAX_COMPORT_LIST];		// Com�t���l�[�����X�g	�i�Ⴂ�v�f�ԍ��l�߁j
	int	gComPortEnd;								// �擾�������X�g�̗v�f��


	static int DetectComPorts(unsigned char *ComPortTable, int ComPortMax);
	static void ListupSerialPort(const unsigned char *ComPortTable, int comports, TCHAR **ComPortDesc, int ComPortMax);


#ifdef USE_MFC
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// ComNo �̒l�������L�[�ɂ��� Com�|�[�g�̔ԍ��Ɩ��O��T���āA�R���{�{�b�N�X�ɕ\������B
	// @ComNo : �R���{�{�b�N�X�̃J�[�\����ݒ肵�܂��B �ݒ肵�Ȃ��ꍇ��-1��ݒ肵�ĉ������B
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	int setGuiComList(CComboBox &ComList, int ComNo)
	{
		int i, comcnt;

		// �R���{�{�b�N�X���N���A
		comcnt = ComList.GetCount();
		for (i = 0; i < comcnt; i++){
			ComList.DeleteString(0);
		}
		// �I�𒆂̃e�L�X�g���e�L�X�g���폜
		ComList.ResetContent();

		// Com�|�[�g������
		searchComList();

		// ComList���R���{�{�b�N�X�ɔ��f
		for (i = 0; i < gComPortEnd; i++){
			ComList.InsertString(i, gComPortNameList[i]);
		}

		if (ComNo>0){
			for (i = 0; i<gComPortEnd; i++){
				if (ComNo == gComPortNoList[i]){
					ComList.SetCurSel(i);
				}
			}
		}

		return 0;
	}
	int setGuiComList(CComboBox &ComList ,const GnrlCom &Com)
	{
		return setGuiComList(ComList ,Com.getComNo());
	}

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// GUI����|�[�gNo.���擾����
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	int getGuiPortNo(const CComboBox &ComList, GnrlCom &ComNo)
	{
		ComNo.putComNo(GnrlComList::getGuiPortNo(ComList));
		return 0;
	}
	int getGuiPortNo(const CComboBox &ComList)
	{
		int combosel = ComList.GetCurSel();
		if (combosel == CB_ERR){
			return -1;
		}

		return gComPortNoList[combosel];
	}
#endif

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	//  Com�|�[�g�ԍ��Ɩ��O���擾���܂��B
	//  Index ��0����n�܂�AgComPortEnd-1�ŏI���܂��B
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	int getComPortNo(int Index, int *PortNo, const TCHAR **PortName)
	{
		if ((Index < 0) || (Index >= gComPortEnd)){
			return -1;
		}
	
		*PortNo		= gComPortNoList[Index];
		*PortName	= gComPortNameList[Index];

		return 0;
	}


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	//  �N�����Ɉ�x�Ă�ŉ������B
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	void initComList()
	{
		memset(gComPortNoList, 0, sizeof(gComPortEnd));
		memset(gComPortNameList, 0, sizeof(gComPortNameList));
		gComPortEnd = 0;
	}

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	//  Com�ԍ��̃��X�g���擾���Ď�������(�ϐ�)�Ɋi�[���܂��B
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	int searchComList()
	{
		clearComList();

		gComPortEnd = DetectComPorts(gComPortNoList, sizeof(gComPortNoList));

		ListupSerialPort(gComPortNoList, gComPortEnd, gComPortNameList, 256);

		return 0;
	}

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// Com���X�g���N���A  �N���A�������Ƃ��ɉ��x�ł��Ă��
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	void clearComList()
	{
		int i;

		memset(gComPortNoList, 0, sizeof(gComPortEnd));

		for (i = 0; i < gComPortEnd; i++){
			if (gComPortNameList[i]){
				free(gComPortNameList[i]);
			}
		}
		memset(gComPortNameList, 0, sizeof(gComPortNameList));
		gComPortEnd = 0;
	}

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	//  �R���s���[�^���g�p�\��Com�|�[�g�ԍ����o���āA���X�g������B
	// 
	// ����		ComPortTable	�{�[�g�ԍ��i�[�z��
	//			ComPortMax		�z��̗v�f��
	// �߂�l					ComPortTable�Ɋi�[�����|�[�g��
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	static int DetectComPorts(unsigned char *ComPortTable, int ComPortMax)
	{
		HMODULE h;
		TCHAR   devicesBuff[65535];
		TCHAR   *p;
		int 	comports = 0;
		int 	i, j, min;
		char	s;

		if (((h = GetModuleHandle( _T("kernel32.dll"))) != NULL) &&
			(GetProcAddress(h, "QueryDosDeviceA") != NULL) &&
			(QueryDosDevice(NULL, devicesBuff, 65535) != 0)) {
			p = devicesBuff;
			while (*p != L'\0') {
				if (_tcsncmp(p, _T("COM"), 3) == 0 && p[3] != L'\0')	// strcmp()
					ComPortTable[comports++] = _wtoi(p + 3);		// atoi()
				if (comports >= ComPortMax)
					break;
				p += (_tcslen(p) + 1);	// strlen

			}

			// �|�[�g�ԍ����Ⴂ���Ƀ\�[�g
			for (i = 0; i < comports - 1; i++) {
				min = i;
				for (j = i + 1; j<comports; j++)
				if (ComPortTable[min] > ComPortTable[j])
					min = j;
				if (min != i) {
					s = ComPortTable[i];
					ComPortTable[i] = ComPortTable[min];
					ComPortTable[min] = s;
				}

			}

		}
		else {
#if 1
			for (i = 1; i <= ComPortMax; i++) {

				FILE *fp;
				TCHAR  buf[COM_NAME_BUF_SIZE];
				Tsprintf(buf, _T("\\\\.\\COM%d"), i);	// 

				Tfopen_s(&fp ,buf, _T("r"));
				if (fp != NULL) {	// fopen()
					fclose(fp);
					ComPortTable[comports++] = i;
				}

			}
#else
			comports = -1;
#endif

		}

		return comports;
	}

	// *********************************************************************
	//  COM�|�[�g�̃t���l�[���擾����B
	//  
	//  ����	ComPortTable	Com�ԍ����X�g	����
	//			comports		ComPortTable�Ɋi�[����Ă���Com�ԍ��̌�(�z��ŎႢ������l�߂��Ă��鎖)
	//			ComPortDesc		Com�ԍ��ɊY������t���l�[�� �o��
	//			ComPortMax		���g�p
	//  
	//  ��L�̏��������ł����[�U�r���e�B�͌��シ��̂ł����A����Ȃ�~���Ƃ��āA
	//  �eCOM�|�[�g�ɕt������u�t���l�[���v�𓯎��ɕ\���������Ȃ�܂��B
	//  COM�|�[�g�̔ԍ��ƂƂ��ɁA�t���l�[�����t���\���ł���ƁA����Ɏg�����肪�悭�Ȃ邱�Ƃ����҂���܂��B
	// *********************************************************************
	static void ListupSerialPort(const unsigned char *ComPortTable, int comports, TCHAR **ComPortDesc, int ComPortMax)
	{
		GUID ClassGuid[1];
		DWORD dwRequiredSize;
		BOOL bRet;
		HDEVINFO DeviceInfoSet = NULL;
		SP_DEVINFO_DATA DeviceInfoData;
		DWORD dwMemberIndex = 0;
		int i;

		for (i = 0; i < gComPortEnd; i++){	// ���X�g�Ƀt���l�[�����c���Ă�����폜�B
			if (gComPortNameList[i]){
				free(gComPortNameList[i]);
			}
		}
		memset(gComPortNameList, 0, sizeof(gComPortNameList));

		DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

		bRet =
			SetupDiClassGuidsFromName(_T("PORTS"), (LPGUID)& ClassGuid, 1,
			&dwRequiredSize);
		if (!bRet) {
			goto cleanup;
		}

		DeviceInfoSet =
			SetupDiGetClassDevs(&ClassGuid[0], NULL, NULL, DIGCF_PRESENT | DIGCF_PROFILE);

		if (DeviceInfoSet) {
			dwMemberIndex = 0;
			while (SetupDiEnumDeviceInfo
				(DeviceInfoSet, dwMemberIndex++, &DeviceInfoData)) {
				TCHAR szFriendlyName[MAX_PATH];
				TCHAR szPortName[MAX_PATH];
				DWORD dwReqSize = 0;
				DWORD dwPropType;
				DWORD dwType = REG_SZ;
				HKEY hKey = NULL;

				bRet = SetupDiGetDeviceRegistryProperty(DeviceInfoSet,
					&DeviceInfoData,
					SPDRP_FRIENDLYNAME,
					&dwPropType,
					(LPBYTE)
					szFriendlyName,
					sizeof(szFriendlyName),
					&dwReqSize);

				hKey = SetupDiOpenDevRegKey(DeviceInfoSet,
					&DeviceInfoData,
					DICS_FLAG_GLOBAL,
					0, DIREG_DEV, KEY_READ);
				if (hKey) {
					long lRet;
					dwReqSize = sizeof(szPortName);
					lRet = RegQueryValueEx(hKey,
						_T("PortName"),
						0,
						&dwType,
						(LPBYTE)& szPortName,
						&dwReqSize);
					RegCloseKey(hKey);
				}

				if (_wcsnicmp(szPortName, _T("COM"), 3) == 0) {  // COM�|�[�g�h���C�o�𔭌�	_strnicmp()
					int port = _wtoi(&szPortName[3]);	// atoi()
					int i;

					for (i = 0; i < comports; i++) {
						if (ComPortTable[i] == port) {  // �ڑ����m�F	
							ComPortDesc[i] = _wcsdup(szFriendlyName);	// �t���l�[�����R�s�[���� _strdup
							break;
						}
					}
				}
			}
		}

	cleanup:
		SetupDiDestroyDeviceInfoList(DeviceInfoSet);
	}

};	// namespace GnrlComList