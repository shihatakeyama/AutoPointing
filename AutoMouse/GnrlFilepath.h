// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// GnrlFilePath.h
// 
// ファイルパスの取得、変換を行う。
// 2013/06/31  Createed
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#ifndef GNRL_FILEPATH_H
#define GNRL_FILEPATH_H


#include "GnrlString.h"


namespace GnrlFilepath
{
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	//  BaseFirstにファイルのフルパスを設定すると、Resultに存在するディレクトリを返す。
	//  本関数では、constはついていませんが、First、Laseの文字列の操作を行いません。
	// 引数
	//   First	  in	文字列の先頭ポインタ
	//   Last	  in	文字列の最後のポインタ
	//   TerminalYRemoved TRUE:文字列終端の'\'を取り除いて処理する
	// Result	  out	\または/の位置を示す
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	void getBaseDirectory(TCHAR* &Result, TCHAR *First, TCHAR *Last=NULL, bool TerminalYRemoved=TRUE);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// 相対パスを絶対パスに変換する
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	void toAbsolutePath(TCHAR *Result, int32_t Len, const TCHAR *Current, const TCHAR *Path);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// 絶対パスを相対パスに変換する
	// 
	// Drive true:ドライブの変換を行わない　false:ドライブの変換を行う
	// 		相対パス	絶対パス	カレントパス	ドライブ変更を有効にする（AbsolutePathでドライブの指定が有効にする場合）
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	void toRelativePath(TCHAR *RelativePath, const TCHAR *AbsolutePath, const TCHAR *CurrentBase, bool DriveEnable=false);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	//  自プログラムファイルが存在する絶対パスを返す　戻り値は長さ
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	Uint32 getModuleFilePath(TCHAR* FilePath, Uint32 Size);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// 相対パスから、実行ファイルを基準とした絶対パスを出力する
	// 相対パスは実行ファイルが置かれているディレクトリ
	// FilePath out	絶対パス
	// FileName in	ファイル名を指定
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	Uint32 getModuleAttachmentFilePath(TCHAR* FilePath, Uint32 Size, const TCHAR *FileName);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// ディレクトリの存在を確認する。 TRUE:存在    FALSE:無し
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	Bool8 isDirectory(const TCHAR* DirName);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// パスの種別を取得する
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	Uint32 getFileAttributes(const TCHAR* DirName);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// ディレクトリを作成　処理後にディレクトリが存在する場合TRUEを戻します。
	// 例、NestNumに1を指定すると、DirNameが存在するディレクトリを作成する
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	Bool makeDir(const TCHAR* DirName);


	Bool compPath(const TCHAR *Path, const TCHAR *Ref);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// ファイルをコピーする　第三引数は上書きチェック
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	Bool copyFile(const TCHAR* ExistingFilePath, const TCHAR* NewFilePath, Bool FailIfExists);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// ファイルを削除する。  関数が成功すると、0 以外の値が返ります。
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	Bool deleteFile(const TCHAR* FilePath);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// ファイルを移動する。  関数が成功すると、0 以外の値が返ります。
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	inline Bool moveFile(const TCHAR* ExistingFilePath, const TCHAR* NewFilePath);

}

#endif	//#define GNRL_FILEPATH_H





