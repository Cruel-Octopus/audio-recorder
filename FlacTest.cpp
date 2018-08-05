// FlacTest.cpp : main project file.

#include "stdafx.h"
#include "FlacHeader.h"
#include <iostream>
#include <string.h> //for strncmp
#include <cstdint>  //for uint_32
#include <intrin.h>

using namespace System;

typedef struct _StreamInfo{
	uint16_t	MinBlockSize;
	uint16_t	MaxBlockSize;
	uint32_t	MinFrameSize;
	uint32_t	MaxFrameSize;
	uint32_t	SampleRate;
	uint8_t		Chanels;
	uint8_t		BitsPerSample;
	uint64_t	TotalSamplesInStream;
}StreamInfo;

class FlacDecoder{

	public:
		StreamInfo SI;
};

static uint32_t reverseBits(uint32_t n) {
        //n = (n >> 1) & 0x55555555 | (n << 1) & 0xaaaaaaaa;
        //n = (n >> 2) & 0x33333333 | (n << 2) & 0xcccccccc;
        n = (n >> 4) & 0x0f0f0f0f | (n << 4) & 0xf0f0f0f0;
        n = (n >> 8) & 0x00ff00ff | (n << 8) & 0xff00ff00;
        n = (n >> 16) & 0x0000ffff | (n << 16) & 0xffff0000;
        return n;
    }
int main(array<System::String ^> ^args)
{
	FlacDecoder FD;

	char *fileName = "c:\\stereo.flac";
    FILE *FlacFile = fopen(fileName, "rb");
	FlacHeader FHEADER;

	fread(&FHEADER, sizeof(FHEADER), 1, FlacFile);
    unsigned t=0;
	if((FHEADER.FlacId[0]==0x66)&&
		(FHEADER.FlacId[1]==0x4C)&&
		(FHEADER.FlacId[2]==0x61)&&
		(FHEADER.FlacId[3]==0x43))
	{
		printf("eto flac");

		//char *temp = FHEADER.MetadataBlockHeader;
		//do {readblockinfo}(if last exit)
		bool IsLastMetadataBlock=0;
		do{
			IsLastMetadataBlock=FHEADER.MetadataBlockHeader[0]>>7;	//get last block flag
			switch (FHEADER.MetadataBlockHeader[0]&0x7f)		//get type of block
			{
				case 0:{
							FHEADER.MetadataBlockHeader[0]=0x00;
							void * temp = FHEADER.MetadataBlockHeader;
							uint32_t td = *(uint32_t *)temp; 
							td = _byteswap_ulong(td);
							FD.SI.MinBlockSize = _byteswap_ushort(FHEADER.MBSI.MinBlockSize);
							FD.SI.MaxBlockSize = _byteswap_ushort(FHEADER.MBSI.MaxBlockSize);
							FD.SI.MinFrameSize = _byteswap_ulong(FHEADER.MBSI.MinFrameSize);
							FD.SI.MaxFrameSize = _byteswap_ulong(FHEADER.MBSI.MaxFrameSize);
							FD.SI.SampleRate = FHEADER.MBSI.SampleRateCHBit;
							FD.SI.SampleRate = FD.SI.SampleRate>>4;
							FD.SI.SampleRate = _byteswap_ulong(FD.SI.SampleRate);
					};	//STREAMINFO 
			}
		}while(IsLastMetadataBlock!=1);


		printf("eto flac");
	}
	
	//unsigned u = reverseBits(FHEADER.MetadataBlockHeader);
	//unsigned u = _byteswap_ulong(FHEADER.MetadataBlockHeader);
	//void * u32 = &FHEADER.MetadataBlockHeader[1];
	//unsigned u = *(unsigned *)u32;
	//u=FHEADER.MetadataBlockHeader<<8;
    return 0;
}
