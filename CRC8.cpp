#include "stdafx.h"
#include <cstdint>
#include <iostream>
#include "CRC8.h"

#include <fstream>
using namespace std;

static uint8_t crc8_table[256]; 

uint8_t CRC8(uint8_t *Ptr, int len){

	uint8_t crc = 0;
        for (int i = 0; i < len; i++){
            crc = CRC8Table[(crc ^ Ptr[i])];
			//crc = crc8_table[(crc ^ Ptr[i])];
		}
        return crc;
}

void init_crc8(){
	uint8_t j,crc;
	uint16_t i;
	//FILE *fp = fopen("CRCTable.txt","wb");
    for (i=0; i<256; i++) {
      crc = i;
      for (j=0; j<8; j++)
        crc = (crc << 1) ^ ((crc & 0x80) ? 0x07 : 0);
      crc8_table[i] = crc & 0xFF;
	  //fprintf(fp, "0x");
	  //fprintf(fp, "%02x", crc);
	 // fprintf(fp, ",");
    }
	//fclose(fp);
}