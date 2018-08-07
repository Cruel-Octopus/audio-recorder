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
	uint8_t		MD5[16];
}StreamInfo;

class FlacDecoder{

	public:
		StreamInfo SI;
};


int main(array<System::String ^> ^args)
{

	FlacDecoder FD;
	void * Temp;
	uint8_t * Temp8;
	uint32_t TempData=0;

	char *fileName = "c:\\stereo.flac";
    FILE *FlacFile = fopen(fileName, "rb");
	
	FlacHeader * FHEADER = new FlacHeader;

	fread(FHEADER, FlacHeaderSize, 1, FlacFile);
    unsigned t=0;
	if((FHEADER->FlacId[0]==0x66)&&
		(FHEADER->FlacId[1]==0x4C)&&
		(FHEADER->FlacId[2]==0x61)&&
		(FHEADER->FlacId[3]==0x43))
	{
		printf("eto flac ");

		//READ STREAMINFO 
		bool IsLastMetadataBlock= true;
		IsLastMetadataBlock=FHEADER->MetadataBlockHeader[0]>>7;	//get last block flag
		if((FHEADER->MetadataBlockHeader[0]&0x7f)==0x00){	//get type of block

			FD.SI.MinBlockSize = _byteswap_ushort(FHEADER->MBSI.MinBlockSize);
			FD.SI.MaxBlockSize = _byteswap_ushort(FHEADER->MBSI.MaxBlockSize);
							
			Temp = FHEADER->MBSI.MinFrameSize;
			FD.SI.MinFrameSize = *(uint32_t *)Temp;
			FD.SI.MinFrameSize = FD.SI.MinFrameSize << 8;
			FD.SI.MinFrameSize = _byteswap_ulong(FD.SI.MinFrameSize);

			Temp = &FHEADER->MBSI.MinFrameSize[3];
			FD.SI.MaxFrameSize = *(uint32_t *)Temp;
			FD.SI.MaxFrameSize = FD.SI.MaxFrameSize << 8;
			FD.SI.MaxFrameSize = _byteswap_ulong(FD.SI.MaxFrameSize);
							
			Temp = &FHEADER->MBSI.MaxFrameSize[2];
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

			Temp8 = FHEADER->MBSI.MD5;
			Temp8--;
			Temp8--;
			for(int i=0;i<16;i++){
				FD.SI.MD5[i] = Temp8[i];
				printf("%02x",FD.SI.MD5[i]);
			}

		}	//END STREAMINFO 
		else {return 0;}
		delete FHEADER;	//free memory
		
		uint8_t	* MetadataBlockHeader = new uint8_t[4];
		
		//read next block if exist
		while(IsLastMetadataBlock!=1)
		{
			fread(MetadataBlockHeader, MetadataBlockHeaderSize, 1, FlacFile);
			IsLastMetadataBlock=MetadataBlockHeader[0]>>7;
			switch (MetadataBlockHeader[0]&0x7f)		
			{
				case 3:{
						printf("case 3");
						break;
				};	
				default:{
					return 0;
				}
			}
		};


		printf("eto flac ");
	}
	
	//unsigned u = reverseBits(FHEADER.MetadataBlockHeader);
	//unsigned u = _byteswap_ulong(FHEADER.MetadataBlockHeader);
	//void * u32 = &FHEADER.MetadataBlockHeader[1];
	//unsigned u = *(unsigned *)u32;
	//u=FHEADER.MetadataBlockHeader<<8;
    return 0;
}

void ReadSeekpoinTable()
{

}