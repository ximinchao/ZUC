#include "ZUC.h"
#include "EIA3.h"

/*Length range of input message*/
u32 gMsgLen;

/*Key Stream*/
#define KEY_STREAM_COUNT 2
u8 gKeyStream[KEY_STREAM_COUNT*4] = {0};

/*Length of COUNT is 4*/
u32 EIA3_Init(u8 *IK, u8 *COUNT, u8 DIRECTION, u8 BEARER)
{
	u8 IV[16] = {0};
	u32 i = 0;
	u32 TempKeyStream = 0;

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
	
	for (i=0; i<KEY_STREAM_COUNT; i++)
	{
		//need update to byte stream version
		GenerateKeyStream(&TempKeyStream, 1);
		memcpy(&gKeyStream[i*4], &TempKeyStream, 4);
	}

	gMsgLen = 0;
}

/*Length of MAC is 4*/
u32 EIA3_Update(u8 *M, u32 LENGTH, u8 *MAC)
{
	u32 i;
	u8 MACTemp[4] = {0};

	for (i=0; i<LENGTH; i++)
	{
		if (GET_BIT(M, i))
		{
			GET_NEXT_KEYSTREAM(MACTemp);
			for (j=0; j<4; j++)
			{
				MAC[j] = MAC[j] ^ MACTemp[j];
			}
		}
		gMsgLen++;
	}
}

/*Length of MAC is 4*/
void EIA3_Final(u8 *MAC)
{
	
}

u8 GET_GIT(u8 *DATA, u32 i)
{
	return (DATA[i/8] & (0x80>>(i%8))) ? 1 : 0;
}

/*Length of DATAOUT is 4*/
void GET_WORD(u8 *DATA, u32 i, u8 *DATAOUT)
{
	u32 j = 0;

	for (j=0; j<4; j++)
	{
		if (i%8 == 0)
		{
			DATAOUT[j] = DATA[i/8+j];
		}
		else
		{
			DATAOUT[j] = (DATA[i/8+j] << (i%8)) | (DATA[i/8+j+1] >> (8-i%8));
		}
	}
}

/*Length of DATA is 4*/
void GET_NEXT_KEYSTREAM(u8 *DATA)
{
	u32 i = 0;
	u32 TempKeyStream = 0;
	
	//error!!!!!!!!!need to change the chance of generate new key stream!!!!!!!!!!
	if ((gMsgLen % 32) == 1)
	{
		for (i=0; i<KEY_STREAM_COUNT-1; i++)
		{
			memcpy(&gKeyStream[i*4], &gKeyStream[(i+1)*4], 4);
		}

		//need update to byte stream version
		GenerateKeyStream(&TempKeyStream, 1);
		memcpy(&gKeyStream[i*4], &TempKeyStream, 4);
	}

	GET_WORD(gKeyStream, , DATA);
}
