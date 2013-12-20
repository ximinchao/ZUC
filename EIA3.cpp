#include "ZUC.h"
#include "EIA3.h"

u32 L0;
u32 L1;

/*Length of COUNT is 4*/
u32 EIA3_Init(u8 *IK, u8 *COUNT, u8 DIRECTION, u8 BEARER)
{
	u8 IV[16] = {0};

	L0 = 0;
	L1 = 0;

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
}

/*Length of MAC is 4*/
u32 EIA3_Update(u8 *M, u32 LENGTH, u8 *MAC)
{
	u32 i;
	u8 MACTemp[4] = {0};

	L0 = L1;
	L1 = LENGTH;

	for (i=L0; i<L1; i++)
	{
		if (GET_BIT(M, i-L0))
		{
			for (j=0; j<4; j++)
			{
				GET_WORD(i, MACTemp);
				MAC[i] = MAC[i] ^ MACTemp[i];
			}
		}
	}
}

/*Length of MAC is 4*/
void EIA3_Final(u8 *MAC)
{
	
}
