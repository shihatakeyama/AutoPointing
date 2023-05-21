// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// GnrlFilePath.h
// 
// �t�@�C���p�X�̎擾�A�ϊ����s���B
// 2013/06/31  Createed
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#ifndef GNRL_FILEPATH_H
#define GNRL_FILEPATH_H


#include "GnrlString.h"


namespace GnrlFilepath
{
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	//  BaseFirst�Ƀt�@�C���̃t���p�X��ݒ肷��ƁAResult�ɑ��݂���f�B���N�g����Ԃ��B
	//  �{�֐��ł́Aconst�͂��Ă��܂��񂪁AFirst�ALase�̕�����̑�����s���܂���B
	// ����
	//   First	  in	������̐擪�|�C���^
	//   Last	  in	������̍Ō�̃|�C���^
	//   TerminalYRemoved TRUE:������I�[��'\'����菜���ď�������
	// Result	  out	\�܂���/�̈ʒu������
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	void getBaseDirectory(TCHAR* &Result, TCHAR *First, TCHAR *Last=NULL, bool TerminalYRemoved=TRUE);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// ���΃p�X���΃p�X�ɕϊ�����
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	void toAbsolutePath(TCHAR *Result, int32_t Len, const TCHAR *Current, const TCHAR *Path);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// ��΃p�X�𑊑΃p�X�ɕϊ�����
	// 
	// Drive true:�h���C�u�̕ϊ����s��Ȃ��@false:�h���C�u�̕ϊ����s��
	// 		���΃p�X	��΃p�X	�J�����g�p�X	�h���C�u�ύX��L���ɂ���iAbsolutePath�Ńh���C�u�̎w�肪�L���ɂ���ꍇ�j
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	void toRelativePath(TCHAR *RelativePath, const TCHAR *AbsolutePath, const TCHAR *CurrentBase, bool DriveEnable=false);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	//  ���v���O�����t�@�C�������݂����΃p�X��Ԃ��@�߂�l�͒���
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	Uint32 getModuleFilePath(TCHAR* FilePath, Uint32 Size);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// ���΃p�X����A���s�t�@�C������Ƃ�����΃p�X���o�͂���
	// ���΃p�X�͎��s�t�@�C�����u����Ă���f�B���N�g��
	// FilePath out	��΃p�X
	// FileName in	�t�@�C�������w��
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	Uint32 getModuleAttachmentFilePath(TCHAR* FilePath, Uint32 Size, const TCHAR *FileName);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// �f�B���N�g���̑��݂��m�F����B TRUE:����    FALSE:����
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	Bool8 isDirectory(const TCHAR* DirName);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// �p�X�̎�ʂ��擾����
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	Uint32 getFileAttributes(const TCHAR* DirName);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// �f�B���N�g�����쐬�@������Ƀf�B���N�g�������݂���ꍇTRUE��߂��܂��B
	// ��ANestNum��1���w�肷��ƁADirName�����݂���f�B���N�g�����쐬����
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	Bool makeDir(const TCHAR* DirName);


	Bool compPath(const TCHAR *Path, const TCHAR *Ref);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// �t�@�C�����R�s�[����@��O�����͏㏑���`�F�b�N
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	Bool copyFile(const TCHAR* ExistingFilePath, const TCHAR* NewFilePath, Bool FailIfExists);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// �t�@�C�����폜����B  �֐�����������ƁA0 �ȊO�̒l���Ԃ�܂��B
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	Bool deleteFile(const TCHAR* FilePath);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	// �t�@�C�����ړ�����B  �֐�����������ƁA0 �ȊO�̒l���Ԃ�܂��B
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	inline Bool moveFile(const TCHAR* ExistingFilePath, const TCHAR* NewFilePath);

}

#endif	//#define GNRL_FILEPATH_H





