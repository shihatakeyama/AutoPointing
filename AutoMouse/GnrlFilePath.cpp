// *********************************************************************************
// OSなどが提供する数々のモジュールをラップする。
// GnrlFilePath.cpp
// 
// 2019/10/14  Createed
// *********************************************************************************


#include "stdafx.h"

#include <stdio.h>
#include <direct.h>

#include "GnrlDefine.h"
#include "GnrlFilePath.h"


///#ifndef MAX_BUF		// バッファサイズ　有限なサイズの為、頻繁に使用するべきでない。
//#define MAX_BUF	256
//#endif

// **********************************************************************************
//  実行ファイルが置かれているファイルパスを返します
//  関数が成功すると、バッファにコピーされた文字列の長さが、文字単位で返る
// **********************************************************************************
Uint32 GnrlFilepath::getModuleFilePath( TCHAR *FilePath, Uint32 Size)
{
	return ::GetModuleFileName(NULL ,FilePath ,Size);

#if 0	// Linuxの場合の参考
    char buf[1024];
    int ret = readlink("/proc/self/exe", buf, sizeof(buf));
    if (ret != -1) {
        buf[ret] = '\0';
        printf("path=%s\n", buf);
    }
>path=/home/shigeo/a.out
#endif
}

// **********************************************************************************
//  実行ファイルがが置かれているパスを基準にした、相対パスを絶対パスに変換する。
//  引数
//　  FilePath out	絶対パス
//　  FileName in		相対ファイル名を指定,絶対パスも指定可能
//  戻り値
//　  生成した絶対パス（FilePath）の長さを返します。 ==0:エラー
// **********************************************************************************
Uint32 GnrlFilepath::getModuleAttachmentFilePath(TCHAR* FilePath, Uint32 Size, const TCHAR *FileName)
{
	Uint32	size;
	TCHAR	*cbp;

	// 自実行ファイルのパスを取得
	size = getModuleFilePath(FilePath, Size);

	if( size == 0)
		return size;

	// 実行ファイルが存在するディレクトリを取得
	getBaseDirectory(cbp, FilePath);

	if(cbp == NULL)
		return 0;

	*cbp = '\0';

	toAbsolutePath(FilePath, Size ,FilePath, FileName);

	return Tstrlen(FilePath);
}

// **********************************************************************************
//  BaseFirstが存在するディレクトリを&Resultに返す。
//  本関数では、constはついていませんが、First、Laseの文字列の操作を行いません。
// 引数
//   First	  in	文字列の先頭ポインタ
//   Last	  in	文字列の最後のポインタ Firstに設定された文字列がNULL終端である場合は、NULLを設定可能。
//   TerminalYRemoved TRUE:文字列終端の'\'がある場合、取り除いて処理する
// Result	  out	\または/の位置を示す
// **********************************************************************************
void GnrlFilepath::getBaseDirectory(TCHAR* &Result, TCHAR *First, TCHAR *Last, Bool TerminalYRemoved)
{
	TCHAR *p;

	if(First==NULL || First[0]=='\0'){
		Result = NULL;
		return;
	}

	// Baseの終端アドレスを格納
	if(Last!=NULL)
	{
		p = Last;
	}else{
		p = First + Tstrlen(First);
	}

	if(TerminalYRemoved)
		if(*p=='\\' || *p=='/')		// 語尾の\、/は取り除く
			p--;

	while(First < p){
		if(*p==':'){		// C:
			Result = Last;
			return;
		}
		
		if(*p=='\\'||*p=='/'){	
			Result = p;//        ↓Lastが\を指す
			return;		// C:\hoge\　
		}
		p--;
	}

	Result = Last;
	return;
}


#if 0
// (Path)を絶対パスに変換する
void GnrlFilepath::toAbsolutePath(int8_t *Result, 
							const int8_t *Base,
							const int8_t *Path)
{
	int32_t length;
	const int8_t *pp,*bp;	// パスポインタ、ベースポインタ
	int8_t *rp;

	// pathが絶対パス表記である場合
	if(Path[1]==':')
	{
		strcpy(Result, Path);	// Pathの値を返す
		return;
	}

#if 0
	// pathがカレントディレクトリである場合
	if(!strcmp(Path,".")||!strcmp(path,".\"))
	{
		strcpy(Result,Path);
		return;
	}
#endif

	length = strlen(Base);
	bp = Base + length;		// bpは文字列baseの終端を指す
	if(*(bp-1)=='\\' || *(bp-1)=='/')
		bp--;				// Baseの語尾の\、/は取り除く

	// Pathの先頭部にある".."または"../"を解析する事で
	// Baseのパス表記のうち、Resultと共通部分を調べる
	for(pp=Path; *pp!='\0' && *pp=='.';)
	{
		if(!strncmp(pp,"..\\",3)||!strncmp(pp,"../",3))	// Path前部が[..\]1階層上へ移動可能
		{
			pp += 3;
			getBaseDirectory(bp, Base, bp);
		}else if(!strncmp(pp,".\\",2) || !strncmp(pp,"./",2))
		{
			pp +=2;
		}else if(!strncmp(pp,"..\0",3))
		{
			pp += 2;
			getBaseDirectory(bp, Base,bp);
		}else
		{
			break;
		}
	}

	length = bp - Base;				// Baseのパス名と共通部分をResultに複写する
	strncpy(Result,Base,length);

	// BaseとPathの間の文字を挿入
	rp = Result + length;
	if(strchr(Base,'\\') || strchr(Path,'\\'))
	{
		*rp++ = '\\';
	}else
	{
		*rp++ = '/';
	}

	// Pathのうち、先頭部分の".\"や"..\"を取り除いた残りの部分
	// (ppが示す文字列)を、Resultの文字列に追加する。
	strcpy(rp,pp);
	return;
}
#else
void GnrlFilepath::toAbsolutePath(TCHAR *Result, int32_t Len,
							const TCHAR *Current,
							const TCHAR *Path)
{
	TCHAR *ct = (TCHAR*)Current;
	int32_t length;
	TCHAR	*pp,*bp;	// パスポインタ、ベースポインタ
	TCHAR	*rp;
	TCHAR	subdircode;		// '\'または'/'

	if(Tstrchr(Current,'\\')){
		subdircode = '\\';
	}else{
		subdircode = '/';
	}

#if 0
	if(!strcmp(Path,".")||!strcmp(path,".\\"))	// pathがカレントディレクトリである場合
	{
		strcpy(Result,Path);
		return;
	}
#endif
	if(*Path == subdircode){	// Pathがルートディレクトリ指定の場合

		Tstrcpy_s(Result, Len, Current);
		rp = Tstrchr(Result, subdircode);
		if(rp && rp[1]==subdircode){
			// Currentが　http:// である場合
			rp = Tstrchr(rp+3, subdircode);
			if(rp){
				Tstrcpy_s(rp, Len - (rp - Result), Path);	// "http://yasuneko/"の後ろにPath"Name"を付加
				return;
			}else{
				Tstrcat_s(Result, Len , Path);				// "http://yasuneko"の後ろにPath"/Name"を付加
				return;
			}
		}else{
			// Currentが　C:\　//
			Tstrcpy_s(rp, Len - (rp - Result), Path);
			return;
		}
	}

	// Pathが絶対パス表記である(ファイルパスの根元がある)場合
	// ネットワークアクセスの場合は考慮
	if(Tstrchr(Path, ':') != NULL
	||( Path[0]==subdircode && Path[1]==subdircode)){
		Tstrcpy_s(Result, Len, Path);	// Pathの値を返す
		return;
	}

	length = Tstrlen(Current);
	bp = ((TCHAR*)Current) + length;	// bpは文字列baseの終端を指す
	if(*(bp-1)=='\\' || *(bp-1)=='/'){
		bp--;				// Baseの語尾の\、/は取り除く
	}

	// Pathの先頭部にある".."または"../"を解析する事で
	// Baseのパス表記のうち、Resultと共通部分を調べる
	for(pp=(TCHAR*)Path; *pp!='\0' && *pp=='.';){
		if(!Tstrncmp(pp ,_T("..\\") ,3) || !Tstrncmp(pp ,_T("../") ,3)){	// Path前部が[..\]1階層上へ移動可能
			pp += 3;
			getBaseDirectory(bp, ct, bp);
		}else if(!Tstrncmp(pp ,_T(".\\"),2) || !Tstrncmp(pp ,_T("./") ,2)){
			pp +=2;
		}else if(!Tstrncmp(pp ,_T("..\0") ,3)){
			pp += 2;
			getBaseDirectory(bp, ct, bp);
		}else{
			break;
		}
	}

	// Baseのパス名と共通部分をResultに複写する
	length = bp - Current;
//--	Nstrncpy(Result, Current, length);			// 
	wcsncpy_s(Result, Len, Current, length);		//

	// BaseとPathの間の文字を挿入
	rp = Result + length;
//	if(strchr(Current,'\\') || strchr(Path,'\\'))
//	{
//		*rp++ = subdircode;//'\\';
//	}else
//	{
//		*rp++ = subdircode;// '/';
//	}
	*rp++ = subdircode;

	// Pathのうち、先頭部分の".\"や"..\"を取り除いた残りの部分
	// (ppが示す文字列)を、Resultの文字列に追加する。
	Tstrcpy_s(rp, Len - (rp - Result), pp);

	return;
}
#endif

#if 0 // コンパイルエラー出るので。。
// **********************************************************************************
//  絶対パスを相対パスに変換する
//
//  引数
//    RelativePath 相対パス出力
//    AbsolutePath 絶対パス入力
//    CurrentBase  カレントパス入力
//    DriveEnable　FALSE:ドライブの変換無効　TRUE:ドライブの変換有効（AbsolutePathでドライブの指定が有効にする場合）
// **********************************************************************************
void GnrlFilepath::toRelativePath(Nchar *RelativePath, const Nchar *AbsolutePath, const Nchar *CurrentBase, Bool DriveEnable)
{
	const Nchar *apath,*cbase;
	TCHAR *upsequence,sepsequence;
	const Nchar *acoron,*ccoron;
	int32_t	len,i;
	
	// 引数絶対Pathが相対パスである場合
	acoron = Nstrrchr(AbsolutePath,':');
	ccoron = Nstrrchr(CurrentBase,':');
	if(ccoron == NULL || acoron == NULL){
		Nstrcpy(RelativePath, AbsolutePath);	// そのまま出力
		return;
	}

	if(Nstrrchr(AbsolutePath, '\\') == NULL){
		upsequence = _T("../");
		sepsequence = '/';
	}else{
		upsequence = _T("..\\");
		sepsequence = '\\';
	}

	if(DriveEnable){	// ドライブの変更を有効にする場合
		// ***ドライブ変換が有効である場合相対パスのドライブを使用する***
		len = acoron - AbsolutePath;
		if(len == (ccoron-CurrentBase)){
			i = 0;
			while(1){
				if(i >= len ){
					break;
				}
				if(tolower(*AbsolutePath) != tolower(*CurrentBase)){
					// ドライブ名不一致の場合絶対パスをコピーしてリターンする
					Nstrcpy(RelativePath, AbsolutePath);
					return;
				}
				i++;
			}
		}else{
			Nstrcpy(RelativePath, AbsolutePath);
			return;
		}
		// ドライブ変換が無効である場合絶対パスのドライブを使用する	
//		memcpy(RelativePath, CurrentBase, (ccoron+1 - CurrentBase));
	}
	
	// ドライブは考慮しなくていいので、どのくらいのネストまで一致するかを調べる
	RelativePath[0] = '\0';
	apath = acoron + 1;		// 絶対パス
	cbase = ccoron + 1;		// 相対パス
	
	while(*apath){
		if(*apath=='\\' || *apath=='/'){
			apath++;
		}
	}
	while(*cbase){
		if(*cbase=='\\' || *cbase=='/'){
			cbase++;
		}
	}

	while(1)
	{
		if(*acoron==sepsequence){	// どのディレクトリネストまで同じなのかメモする
			acoron = apath+1;
		}

		if(cbase=='\0')
		{
			Nstrcat(RelativePath, upsequence);
			Nstrcat(RelativePath, acoron);
			return;
		}

		if(tolower(*AbsolutePath) != tolower(*CurrentBase))
		{	// 文字列が違っていたら、残りのネスト数を調べてシーケンスを追加する
			Nstrcat(RelativePath, upsequence);
			
			while(1){
				if(*cbase==NULL)	break;

				if(*cbase==sepsequence && cbase[1]!= '\0'){
					Nstrcat(RelativePath, upsequence);
				}
				cbase++;
			}
			Nstrcat( RelativePath, upsequence+2);	// "\"または"/"を追加
			Nstrcat(RelativePath, acoron);
			return;
		}
		apath++;
		cbase++;	
	}
}
#endif

// ディレクトリの存在を確認する
Bool8 GnrlFilepath::isDirectory(const TCHAR* DirName)
{
	Uint32 result;

	result = GetFileAttributes(DirName);

	if(result==0xFFFFFFFF)
	{
		return FALSE;
	}else if(result & FILE_ATTRIBUTE_DIRECTORY)
	{
		return TRUE;
	}else{
		return FALSE;
	}
}
// パスの種別を取得する
Uint32 GnrlFilepath::getFileAttributes(const TCHAR* DirName)
{
	return GetFileAttributes(DirName);
}

#if 0	// コンパイルエラー出るので。。
// ディレクトリを作成します。処理後にディレクトリが存在する場合TRUEを戻します
Bool GnrlFilepath::makeDir(const Nchar* DirName)
{
	Nchar buf[MAX_PATH],*buflast;
	Nchar *bufsep;
	int32_t loopnew,loopold=0x7FFFFFFF;

	// 既存にディレクトリが存在していないかチェック
	if(isDirectory(DirName)==TRUE)
		return TRUE;

	do{
		Nstrncpy(buf, DirName, MAX_PATH);
		buflast = buf + Nstrlen(buf);

		for(;DirName==0;DirName--)	// DirNameから何階層サブディレクトリを作らない？
		{
			getBaseDirectory(bufsep, buf, buflast);
			buflast = (Nchar*)bufsep;
			*((TCHAR*)bufsep) = '\0';
		}

		loopnew = 0;
		while(Nmkdir(buf)!=0)
		{	// ディレクトリの作成に失敗したら、ディレクトリをUpさせる
			int32_t err = errno;

			getBaseDirectory(bufsep, buf, buflast);
			if(bufsep == buflast)
				return FALSE;	// ディレクトリが作成できない DirNameの値が異常
			*((int8_t*)bufsep) = '\0';
			buflast = (Nchar*)bufsep;
			loopnew++;
		}
		if(loopnew<loopold)	// _mkdir()連続エラーによる無限ループ回避対策
		{
			loopold = loopnew;
		}else{
			return FALSE;
		}
	}while(loopnew!=0);

	return TRUE;
}
#endif

//**********************************************************************************
// ファイルをコピーする
//**********************************************************************************
Bool GnrlFilepath::copyFile(const TCHAR* ExistingFilePath, const TCHAR* NewFilePath, Bool FailIfExists)
{
	if(::CopyFile(ExistingFilePath, NewFilePath, FailIfExists))
	{return true;}
	else{return false;}
}

#if 1
//**********************************************************************************
// ファイルを削除する
//**********************************************************************************
Bool GnrlFilepath::deleteFile(const TCHAR* FilePath)
{
	if(DeleteFile(FilePath)){
		return TRUE;
	}else{
		return FALSE;	
	}
}
#endif


// パスが一致してうるかチェックする	Refにはワイルカードを含める事が可能 大文字小文字の区別は行わない
// 引数
//		Path	フルパス入力				例、"c:\Program\bin\hoge.exe"
//		Ref		検索用パス(ワイルカード可能)例、"c:\Program\*"
// 戻り値
//		一致している場合、TRUEを返します
Bool GnrlFilepath::compPath(const TCHAR *Path, const TCHAR *Ref)
{
	const TCHAR *path;

	if(!Path || !Ref)	return false;

	while(1){
		if(*Ref == '\0'){
			if(*Path == '\0'){
				return true;
			}else{
				return false;
			}
		}else if(*Ref == '*'){		// '*'ワイルカード発見
			while(*Ref == '*')
				Ref++;

			if(*Ref == '\0')
			{
				return true;	// '*'の後に'\0'があった場合、一致決定とする
			}else{
				path = Path;
//				Ref++;		// '*'ワイルカードの文字列長さ調整
				while(*path)
				{
					if( //tolower(*path) == tolower(*Ref)	/* 処理を高速にする為に追加 */
						//&&
						compPath(path, Ref)
						)
						return true;
					path++;
				}
			}
		}else{
			if(*Path == '\0')
			{
				return false;
			}
		}

		if(tolower(*Path) != tolower(*Ref)							// 文字一致
		&& !((*Path == '\\' || *Path == '/') && (*Ref == '\\' || *Ref == '/'))	// ディレクトリシーケンス
		&& *Ref != '?'){	// '?'ワイルカード
			return false;
		}else{
			Path++;
			Ref++;
		}
	}
}

Bool GnrlFilepath::moveFile(const TCHAR* ExistingFilePath, const TCHAR* NewFilePath)
{
	if(::MoveFile(ExistingFilePath, NewFilePath)){
		return true;
	}else
	{
		return false;
	}
}
