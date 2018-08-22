#include "stdafx.h"
#include <cstdint>
#include <iostream>
#include "CRC16.h"

using namespace std;

static uint16_t crc16_table[256];

uint16_t CRC16(uint8_t *Ptr, int len)
{
	uint16_t crc = 0;
	uint8_t index;
        for (uint16_t i = 0; i < len; i++)
		{
            //crc = CRC16Table[(crc ^ Ptr[i])];
			//crc = crc16_table[(crc ^ Ptr[i])];
			//crc = (crc << 8) ^ crc16_table[((crc >> 8) ^ *Ptr++)];
			
			//index = crc >> 8;
			//crc <<= 8;
			//crc ^= crc16_table[index ^ *Ptr++];
			crc = (crc >> 8) ^ CRC16Table[(crc ^ *Ptr++) & 0xff];
		}
	return crc;
}

void InitCRC16()
{
	uint16_t i,j,crc;
	for(int i=0; i<256; i++)
	{
		crc = i<<8;
		for(j=0; j<8; j++)
		{
			if(crc&0x8000)
			{
				crc=(crc<<1)^0x8005;
			}else
				{
					crc=crc<<1;
				}
		}
		//crc &= 0xffff;
		crc16_table[i]=crc;
		printf("%04x\n", crc);
	}
}