#ifndef X_EIA3_H
#define X_EIA3_H

void EIA3_Init(u8 *IK, u8 *COUNT, u8 BEARER, u8 DIRECTION);
void EIA3_Update(u8 *M, u32 LENGTH);
u32 EIA3_Final();

#endif
