#include "stdafx.h"
#include <cstdint>
#include "CRC8.h"
uint8_t CRC8(uint8_t *Ptr, int len)
    {
        uint8_t crc = 0;
        for (int i = 0; i < len; i++){
            crc = CRC8Table[crc ^ Ptr[i]];
		}
        return crc;
    }