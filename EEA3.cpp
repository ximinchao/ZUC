#include "ZUC.h"
#include "EEA3.h"
#include <string.h>

void RevertData(u8 *Data, u32 Len)
{
	u32 i = 0;
	u8 tData = 0;

	for (i=0; i<Len/2; i++)
	{
		tData = Data[i];
		Data[i] = Data[Len-i-1];
		Data[Len-i-1] = tData;
	}
}

bool IsLittleEndian()
{
	u32 t1 = 0x31;

	if ((*(u8*)(&t1)) == 0x31)
	{
		return true;
	}

	return false;
}

/*Length of COUNT is 4*/
void EEA3(u8 *CK, u8 *COUNT, u8 BEARER, u8 DIRECTION, u8 *M, u32 LENGTH, u8 *CM)
{
	u8 IV[16] = {0};
	u32 i = 0, j = 0, tKeyStreamU32 = 0;
	u8 *tKeyStreamU8 = (u8*)&tKeyStreamU32;
	u32 bLENGTH = 0; //Byte LENGTH
	u32 lbLENGTH = 0; //Left Byte LENGTH
	u8 bMask = 0, bFlag = 0;

	IV[0] = COUNT[0];
	IV[1] = COUNT[1];
	IV[2] = COUNT[2];
	IV[3] = COUNT[3];

	IV[4] = ((BEARER << 3) | ((DIRECTION & 1)<<2)) & 0xFC;
	IV[5] = IV[6] = IV[7] = 0;

	IV[8] = IV[0];
	IV[9] = IV[1];
	IV[10] = IV[2];
	IV[11] = IV[3];
	IV[12] = IV[4];
	IV[13] = IV[5];
	IV[14] = IV[6];
	IV[15] = IV[7];

	bLENGTH = LENGTH / 8;
	lbLENGTH = LENGTH % 8;

	Initialization(CK, IV);

	for (i=0; i<bLENGTH; i++)
	{
		if ((i%4) == 0)
		{
			GenerateKeyStream(&tKeyStreamU32, 1);
			if (IsLittleEndian())
			{
				RevertData(tKeyStreamU8, 4);
			}
		}

		CM[i] = tKeyStreamU8[i%4] ^ M[i];
	}

	if (lbLENGTH)
	{	
		if ((i%4) == 0)
		{
			GenerateKeyStream(&tKeyStreamU32, 1);
			if (IsLittleEndian())
			{
				RevertData(tKeyStreamU8, 4);
			}
		}

		bMask = 0;
		bFlag = 0x80;
		for (j=0; j<lbLENGTH; j++)
		{
			bMask |= bFlag;
			bFlag >>= 1;
		}

		CM[i] = ((tKeyStreamU8[i%4] ^ M[i]) & bMask);
	}
}
