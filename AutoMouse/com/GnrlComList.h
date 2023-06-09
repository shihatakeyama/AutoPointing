// *********************************************************************
//                                                                     *
//  FILE        :GnrlComList.h								           *
//  DATE        :2014/07/04			                                   *
//  
//  USE :Win32API,MFC                                                  *
// *********************************************************************


#ifndef GNRLCOMLIST_H
#define GNRLCOMLIST_H


#define MAX_COMPORT_LIST	256



namespace GnrlComList{

	extern unsigned char gComPortNoList[MAX_COMPORT_LIST];	// Comポート番号リスト
	extern TCHAR *gComPortNameList[MAX_COMPORT_LIST];		// Comフルネームリスト
	extern int	gComPortEnd;

	#ifdef USE_MFC											// 使用するComポート番号をコンボボックスで選択する場合。
	int setGuiComList(CComboBox &ComList ,int ComNo);		// �B Comポートの番号と名前を探して、コンボボックスに表示する。		旧 putComList()
	int setGuiComList(CComboBox &ComList ,const GnrlCom &Com);
	int getGuiPortNo(const CComboBox &ComList, GnrlCom &ComNo);// �C
	int getGuiPortNo(const CComboBox &ComList);				// �C GUIで選択されたComポート番号を取得
	#endif
																		// ポート番号と名前を引数で取得したい場合。
	inline int getComPortNum()		{ return gComPortEnd;}						// �B 見つかったComポートの数
	int getComPortNo(int Index, int *PortNo, const TCHAR **PortName);	// �C Index は0から始まり、gComPortEnd-1で終わります。

	void initComList();			//  �@ 起動時に一度呼んで下さい。
	int searchComList();		//  �A Com番号のリストを取得して自メモリ(変数)に格納します。
	void clearComList();		//  �D Comリストをクリア  クリアしたいときに何度でも呼んで可。 ※終了時に呼ばないとメモリリークする。

};	// namespace GnrlComList

#endif // #ifndef GNRLCOMLIST_H

