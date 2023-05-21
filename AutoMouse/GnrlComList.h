// *********************************************************************
//                                                                     *
//  FILE        GnrlComList.h								           *
//  DATE        2014/07/04			                                   *
//  
//  USE :Win32API,MFC                                                  *
// *********************************************************************


#ifndef GNRLCOMLIST_H
#define GNRLCOMLIST_H


#define MAX_COMPORT_LIST	256

#ifndef USE_MFC
#define USE_MFC
#endif


namespace GnrlComList{

	extern unsigned char gComPortNoList[MAX_COMPORT_LIST];	// Comポート番号リスト
	extern TCHAR *gComPortNameList[MAX_COMPORT_LIST];		// Comフルネームリスト
	extern int	gComPortEnd;

	#ifdef USE_MFC
	int putComList(CComboBox &ComList);
	int getComPortNo(const CComboBox &ComList);
	#endif

	int searchComList();
	void initComList();
	void clearComList();

};	// namespace GnrlComList

#endif // #ifndef GNRLCOMLIST_H

