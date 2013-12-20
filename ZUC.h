#ifndef X_ZUC_H
#define X_ZUC_H
typedef unsigned char u8;
typedef unsigned int u32;

void GenerateKeyStream(u32 *pKeyStream, u32 KeyStreamLen);
void Initialization(u8* k, u8* iv);

#endif
