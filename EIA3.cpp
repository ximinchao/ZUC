#include "ZUC.h"
#include "EIA3.h"
#include <string.h>

/*Length range of input message*/
u32 gMsgLen;

/*Key Stream*/
#define KEY_STREAM_COUNT 3
u32 gKeyStream[KEY_STREAM_COUNT] = {0};

/*MAC*/
u32 gMAC = 0;

/*Get i-th bit in bit stream DATA*/
u32 GET_BIT(u8 *DATA, u32 i)
{
	return (DATA[i/8] & ((0x80)>>(i%8))) ? 1 : 0;
}

/*Get i-th WORD(4 bytes) in bit stream DATA*/
u32 GET_WORD(u32 *DATA, u32 i)
{
	if (i%32 == 0)
	{
		return DATA[i/32];
	}
	else
	{
		return ( (DATA[i/32] << (i%32)) | (DATA[i/32+1] >> (32-i%32)) );
	}
}

u32 GET_NEXT_KEYSTREAM()
{
	u32 i = 0;

	if ((gMsgLen % 32) == 0)
	{
		for (i=0; i<KEY_STREAM_COUNT-1; i++)
		{
			gKeyStream[i] = gKeyStream[i+1];
		}

		GenerateKeyStream(&gKeyStream[i], 1);
	}

	return GET_WORD(gKeyStream, (gMsgLen%32));
}

u32 GET_FINAL_KEYSTREAM()
{
	return gKeyStream[2];
}

/*Length of COUNT is 4*/
void EIA3_Init(u8 *IK, u8 *COUNT, u8 BEARER, u8 DIRECTION)
{
	u8 IV[16] = {0};
	u32 i = 0;

	IV[0] = COUNT[0];
	IV[1] = COUNT[1];
	IV[2] = COUNT[2];
	IV[3] = COUNT[3];

	IV[4] = BEARER << 3;
	IV[5] = IV[6] = IV[7] = 0;

	IV[8] = IV[0] ^ ((DIRECTION & 1) << 7);
	IV[9] = IV[1];
	IV[10] = IV[2];
	IV[11] = IV[3];
	IV[12] = IV[4];
	IV[13] = IV[5];
	IV[14] = IV[6] ^ ((DIRECTION & 1) << 7);
	IV[15] = IV[7];

	Initialization(IK, IV);
	
	/*Skip the first 4 bytes, because GET_NEXT_KEYSTREAM will do this for us at the beginning*/
	for (i=1; i<KEY_STREAM_COUNT; i++)
	{
		GenerateKeyStream(&gKeyStream[i], 1);
	}

	gMsgLen = 0;
	gMAC = 0;
}

/*Length of MAC is 4*/
void EIA3_Update(u8 *M, u32 LENGTH)
{
	u32 i = 0;
	u32 MACTemp = 0;

	for (i=0; i<LENGTH; i++)
	{
		MACTemp = GET_NEXT_KEYSTREAM();
		if (GET_BIT(M, i))
		{
			gMAC = gMAC ^ MACTemp;
		}
		gMsgLen++;
	}
}

u32 EIA3_Final()
{
	u32 i = 0;
	u32 MACTemp = 0;

	MACTemp = GET_NEXT_KEYSTREAM();
	gMAC = gMAC ^ MACTemp;

	MACTemp = GET_FINAL_KEYSTREAM();
	gMAC = gMAC ^ MACTemp;

	return gMAC;
}
