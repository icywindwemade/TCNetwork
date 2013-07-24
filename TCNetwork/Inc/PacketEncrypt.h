#pragma once

#include "Packet.h"

enum ePacketEncryptCode
{
	ePE_OK,
	ePE_StartMark,
	ePE_Checksum,
	ePE_Sequence,
};

class CPacketEncrypt
{
public:
	void Init();

	void SetNextMask(unsigned char MaskKey);

	bool CheckSequenceNum(unsigned char Num);			//Sequence �˻�			
	bool CheckCheckSum(PACKET* packet);			//Checksum �˻�

	void ApplyUnionMark(PACKET* packet);		//SequenceNum ����
	void ApplyCheckSum(PACKET* packet);			//CheckSum	����
	void ApplyMask(char* buffer,int size);		//Mask ����

private:
	unsigned char	m_bMask;

	unsigned char	m_bKeyIndex;

	unsigned char	m_bSendSequenceNum;
	short			m_sRecvSequenceNum;
};

