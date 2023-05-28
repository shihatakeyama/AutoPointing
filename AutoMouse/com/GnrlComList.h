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

	extern unsigned char gComPortNoList[MAX_COMPORT_LIST];	// Com�|�[�g�ԍ����X�g
	extern TCHAR *gComPortNameList[MAX_COMPORT_LIST];		// Com�t���l�[�����X�g
	extern int	gComPortEnd;

	#ifdef USE_MFC											// �g�p����Com�|�[�g�ԍ����R���{�{�b�N�X�őI������ꍇ�B
	int setGuiComList(CComboBox &ComList ,int ComNo);		// �B Com�|�[�g�̔ԍ��Ɩ��O��T���āA�R���{�{�b�N�X�ɕ\������B		�� putComList()
	int setGuiComList(CComboBox &ComList ,const GnrlCom &Com);
	int getGuiPortNo(const CComboBox &ComList, GnrlCom &ComNo);// �C
	int getGuiPortNo(const CComboBox &ComList);				// �C GUI�őI�����ꂽCom�|�[�g�ԍ����擾
	#endif
																		// �|�[�g�ԍ��Ɩ��O�������Ŏ擾�������ꍇ�B
	inline int getComPortNum()		{ return gComPortEnd;}						// �B ��������Com�|�[�g�̐�
	int getComPortNo(int Index, int *PortNo, const TCHAR **PortName);	// �C Index ��0����n�܂�AgComPortEnd-1�ŏI���܂��B

	void initComList();			//  �@ �N�����Ɉ�x�Ă�ŉ������B
	int searchComList();		//  �A Com�ԍ��̃��X�g���擾���Ď�������(�ϐ�)�Ɋi�[���܂��B
	void clearComList();		//  �D Com���X�g���N���A  �N���A�������Ƃ��ɉ��x�ł��Ă�ŉB ���I�����ɌĂ΂Ȃ��ƃ��������[�N����B

};	// namespace GnrlComList

#endif // #ifndef GNRLCOMLIST_H

