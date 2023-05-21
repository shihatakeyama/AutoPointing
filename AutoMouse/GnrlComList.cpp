// *********************************************************************
//                                                                     *
//  FILE        :GnrlComList.cpp		                               *
//  DATE        :2014/07/04			                                   *
//  
//  PCが使用可能なCOMポート番号と、フルネームを取得して
//    自メモリ(変数)に格納します。									   *
//	
//  USE :Win32API,MFC                                                  *
// *********************************************************************
// 2018/11/28	Unicode対応


#include "stdafx.h"
#include < cstdio >

#include "setupapi.h"	// setupapi.libを使用


#include "GnrlComList.h"

#ifdef _MSC_VER
#define DISABLE_C4996   __pragma(warning(push)) __pragma(warning(disable:4996))
#define ENABLE_C4996    __pragma(warning(pop))
#else
#define DISABLE_C4996
#define ENABLE_C4996
#endif

namespace GnrlComList{

	unsigned char gComPortNoList[MAX_COMPORT_LIST];	// Comポート番号リスト  （若い要素番号詰め）
	TCHAR *gComPortNameList[MAX_COMPORT_LIST];		// Comフルネームリスト	（若い要素番号詰め）
	int	gComPortEnd;								// 取得したリストの要素数


	static int DetectComPorts(unsigned char *ComPortTable, int ComPortMax);
	static void ListupSerialPort(const unsigned char *ComPortTable, int comports, TCHAR **ComPortDesc, int ComPortMax);


#ifdef USE_MFC
	// Comポートの番号と名前を探して、コンボボックスに表示する。
	int putComList(CComboBox &ComList)
	{
		int i, comcnt;
		
		// コンボボックスをクリア
		comcnt = ComList.GetCount();
		for (i = 0; i < comcnt; i++){
			ComList.DeleteString(0);
		}
		// 選択中のテキストをテキストを削除
		ComList.ResetContent();

		// Comポートを検索
		searchComList();

		// ComListをコンボボックスに反映
		for (i = 0; i < gComPortEnd; i++){
			ComList.InsertString(i, gComPortNameList[i]);
		}
		
		return 0;
	}
	int getComPortNo(const CComboBox &ComList)
	{
		int combosel = ComList.GetCurSel();
		if(combosel == CB_ERR){
			return -1;
		}

		return gComPortNoList[combosel];
		
	}

#endif

	// *********************************************************************
	//  Com番号のリストを取得して自メモリ(変数)に格納します。
	// *********************************************************************
	int searchComList()
	{
		clearComList();
		
		gComPortEnd = DetectComPorts(gComPortNoList, sizeof(gComPortNoList));

		ListupSerialPort(gComPortNoList, gComPortEnd, gComPortNameList, 256);
		
		return 0;
	}


	// *********************************************************************
	//  起動時に一度呼んで下さい。
	// *********************************************************************
	void initComList()
	{
		memset(gComPortNoList, 0, sizeof(gComPortEnd));
		memset(gComPortNameList, 0, sizeof(gComPortNameList));
		gComPortEnd = 0;
	}
	// *********************************************************************
	// Comリストをクリア  クリアしたいときに何度でも呼んで
	// *********************************************************************
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

	// *********************************************************************
	//  コンピュータが使用可能なComポート番号検出して、リスト化する。
	// 
	// 引数		ComPortTable	ボート番号格納配列
	//			ComPortMax		配列の要素数
	// 戻り値					ComPortTableに格納したポート数
	// *********************************************************************
	static int DetectComPorts(unsigned char *ComPortTable, int ComPortMax)
	{
		HMODULE h;
		TCHAR   devicesBuff[65535];
		TCHAR   *p;
		int 	comports = 0;
		int 	i, j, min;
		char	s;

		if (((h = GetModuleHandle( L"kernel32.dll")) != NULL) &&
			(GetProcAddress(h, "QueryDosDeviceA") != NULL) &&
			(QueryDosDevice(NULL, devicesBuff, 65535) != 0)) {
			p = devicesBuff;
			while (*p != L'\0') {
				if (_tcsncmp(p, L"COM", 3) == 0 && p[3] != L'\0')	// strcmp()
					ComPortTable[comports++] = _wtoi(p + 3);		// atoi()
				if (comports >= ComPortMax)
					break;
				p += (_tcslen(p) + 1);	// strlen

			}

			// ポート番号が若い順にソート
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
			for (i = 1; i <= ComPortMax; i++) {
				FILE *fp;
#if 1
				DISABLE_C4996//
				TCHAR  buf[16];
					_stprintf(buf, L"\\\\.\\COM%d", i);
				if ((fp = _wfopen(buf, L"r")) != NULL) {	// fopen()
					fclose(fp);
					ComPortTable[comports++] = i;

				}
				ENABLE_C4996//
#else
				char  buf[16];
				errno_t error;
				_sprintf_s(buf, sizeof(buf), "\\\\.\\COM%d", i);
				error = fopen_s(&fp, buf, "r");
				if (error == 0) {	// fopen()
					fclose(fp);
					ComPortTable[comports++] = i;
				}
#endif
			}

		}

		return comports;
	}

	// *********************************************************************
	//  COMポートのフルネーム取得する。
	//  
	//  引数	ComPortTable	Com番号リスト	入力
	//			comports		ComPortTableに格納されているCom番号の個数(配列で若い方から詰められている事)
	//			ComPortDesc		Com番号に該当するフルネーム 出力
	//			ComPortMax		未使用
	//  
	//  上記の処理だけでもユーザビリティは向上するのですが、さらなる欲求として、
	//  各COMポートに付けられる「フルネーム」を同時に表示したくなります。
	//  COMポートの番号とともに、フルネームも付加表示できると、さらに使い勝手がよくなることが期待されます。
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

		for (i = 0; i < gComPortEnd; i++){	// リストにフルネームが残っていたら削除。
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

				if (_wcsnicmp(szPortName, L"COM", 3) == 0) {  // COMポートドライバを発見	_strnicmp()
					int port = _wtoi(&szPortName[3]);	// atoi()
					int i;

					for (i = 0; i < comports; i++) {
						if (ComPortTable[i] == port) {  // 接続を確認	
							ComPortDesc[i] = _wcsdup(szFriendlyName);	// フルネームをコピーする _strdup
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