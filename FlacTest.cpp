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
	uint8_t		MD5[32];
}StreamInfo;

class FlacDecoder{

	public:
		StreamInfo SI;
};


int main(array<System::String ^> ^args)
{
	FlacDecoder FD;
	void * Temp;
	uint32_t TempData=0;

	char *fileName = "c:\\Channel Test (5.0).flac";
    FILE *FlacFile = fopen(fileName, "rb");
	FlacHeader FHEADER;
	int o =sizeof(FlacHeader);
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
							//FHEADER.MetadataBlockHeader[0]=0x00;
							//void * Temp = FHEADER.MetadataBlockHeader;
							//uint32_t TempData = *(uint32_t *)Temp; 
							//td = _byteswap_ulong(td);

							FD.SI.MinBlockSize = _byteswap_ushort(FHEADER.MBSI.MinBlockSize);
							FD.SI.MaxBlockSize = _byteswap_ushort(FHEADER.MBSI.MaxBlockSize);
							
							Temp = FHEADER.MBSI.MinFrameSize;
							FD.SI.MinFrameSize = *(uint32_t *)Temp;
							FD.SI.MinFrameSize = FD.SI.MinFrameSize << 8;
							FD.SI.MinFrameSize = _byteswap_ulong(FD.SI.MinFrameSize);

							Temp = &FHEADER.MBSI.MinFrameSize[3];
							FD.SI.MaxFrameSize = *(uint32_t *)Temp;
							FD.SI.MaxFrameSize = FD.SI.MaxFrameSize << 8;
							FD.SI.MaxFrameSize = _byteswap_ulong(FD.SI.MaxFrameSize);
							
							Temp = &FHEADER.MBSI.MaxFrameSize[2];
							FD.SI.SampleRate = *(uint32_t *)Temp;
							FD.SI.SampleRate = _byteswap_ulong(FD.SI.SampleRate);

							FD.SI.SampleRate = FD.SI.SampleRate >> 0x0c;

							TempData = *(uint32_t *)Temp;
							TempData = _byteswap_ulong(TempData);
							TempData = TempData >> 4;
							TempData = TempData & 0x000000ff;
							FD.SI.Chanels = TempData >> 5;
							FD.SI.BitsPerSample = TempData & 0x1f;

							FD.SI.TotalSamplesInStream = *(uint64_t *)Temp;
							FD.SI.TotalSamplesInStream = _byteswap_uint64(FD.SI.TotalSamplesInStream);
							FD.SI.TotalSamplesInStream = FD.SI.TotalSamplesInStream & 0x0000000fffffffff;

							Temp = FHEADER.MBSI.MD5;
							static_cast (uint8_t *)Temp;
							Temp--;
							Temp--;

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
