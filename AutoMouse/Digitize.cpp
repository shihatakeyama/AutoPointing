// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  Digitize.cpp
// 
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include <stdafx.h>

#include "GnrlNumerical.h"
#include "GnrlThread.h"
#include "Define.h"
#include "global.h"

#include "GnrlComList.h"
#include "GnrlCom.h"

//#include "AutoMouseDlg.h"

#include "define.h"
#include "extern.h"
#include "global.h"
#include "sub.h"

void recvVersion(const uint8_t *Data, uint8_t Len);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �f�W�^�C�U�ʐM
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
static uint8_t gTxBuf[TRX_BUF_SIZE * 2];		// ASCII
static uint8_t gRxBuf[TRX_BUF_SIZE * 2];		// �o�C�i��
static int16_t gRxCo = 0;
static int16_t gRxTimer = 0;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �f�W�^�C�U�փR�}���h�𑗐M���܂��B
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t APD_sendCommand(uint8_t Comand1, uint8_t Comand2, const uint8_t *Data, uint8_t Len)
{
	int32_t len, ack;

	//	len = intArrowToDump16((Sint8*)gTxBuf ,(uint8_t*)Paran ,Paran->Len+2);

	intArrowToDump16((Sint8*)&gTxBuf[0], &Comand1, sizeof(Comand1));
	intArrowToDump16((Sint8*)&gTxBuf[2], &Comand2, sizeof(Comand2));
	len = 4;

	intArrowToDump16((Sint8*)&gTxBuf[4], Data, Len);
	len += Len * 2;

	gTxBuf[len] = CR;
	len++;

	ack = gCom.write(gTxBuf, len);
	if (ack < 0){
		return -4;
	}

	return 0;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ��ʂ̉𑜓x��ݒ�
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t AM_setDisplayResolution(int resolution_x, int resolution_y)
{
	uint8_t buf[8];

	Uint16ToUint8Arrow(&buf[0], resolution_x);
	Uint16ToUint8Arrow(&buf[2], resolution_y);

	return APD_sendCommand(EN_digitizer, 0x90, buf, 4);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ��ʉ����ė���
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int32_t AM_click(int x, int y)
{
	uint8_t buf[8];

	Uint16ToUint8Arrow(&buf[0], x);
	Uint16ToUint8Arrow(&buf[2], y);

	return APD_sendCommand(EN_digitizer, 0x07, buf, 4);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �V�X�e���n�̃R�}���h�󂯂��B
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void cmdSystem(const uint8_t *Data, uint8_t Len)
{
	switch (Data[1]){
	case 0x99:
		recvVersion(Data, Len);
		break;
	}
}

char gDebugBuf[64];

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ��M�X���b�h ���[�v
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Uint32 RecvRootine(void* Arg)
{
	int32_t	ack;
	uint8_t	rxcc, rxco_h;

	while (gRecvThread.isLife()){

		ack = gCom.read(&rxcc, sizeof(rxcc));
		if (ack > 0){
			rxco_h = gRxCo >> 1;
			if (rxcc != CR){
				// 0.5�o�C�g�Âo�b�t�@�ɂ����B

				if ((gRxCo & 0x01) == 0){
					gRxBuf[rxco_h] = asciiToInt((uint8_t)rxcc) << 4;
				}
				else{
					gRxBuf[rxco_h] |= asciiToInt((uint8_t)rxcc);
				}
#if 1
				// �ʐM�f�o�O�p
				gDebugBuf[gRxCo] = rxcc;
#endif

				gRxCo++;
				if (gRxCo > TRX_BUF_SIZE){
					gRxCo = 0;
				}

			}
			else{
				// �t���[����M����
				if ((gRxCo & 0x01) == 0){
					switch (gRxBuf[0]){
					case EN_mouse:  // �}�E�X
						break;
					case 0xD0:	// �f�o�O

						break;
					case 0x99:	// �V�X�e���A�����e�i���X
						cmdSystem(&gRxBuf[0], rxco_h);
					default:
						break;
					}
				}
				gRxCo = 0;
			}
			gRxTimer = RX_TIMEOUT;
		}
		else{
			if (gRxTimer>0){
				gRxTimer--;
			}
			else{
				gRxCo = 0;	// RX_TIMEOUT�̊���Byt�擾�ł��Ȃ�������A�o�b�t�@�ʒu0�N���A�B
			}

			Sleep(10);
		}
	}
	return 0;
}


