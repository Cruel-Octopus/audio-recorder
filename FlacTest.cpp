// FlacTest.cpp : main project file.

#include "stdafx.h"
#include "FlacHeader.h"
#include <iostream>
#include <string.h> //for strncmp
#include <cstdint>  //for uint_32
#include <intrin.h>
#include "CRC8.h"
#include "CRC16.h"

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
void RiceDecoding(uint8_t * BitStream,uint8_t shift,uint8_t RiceParameter,uint8_t length)
{
	*BitStream = *BitStream << shift;
	int8_t Data = 0;
	uint8_t Quotient = 0;
	uint8_t Remainder = 0;
	uint8_t Count = 0;
	bool FirstPart = 1;
	uint8_t TailCounter = 0;

	for (uint8_t i =0;i<shift;i++){

	}

	for(uint8_t i =0; i<length;i++)
	{
		for(uint8_t j =0; j<8;j++)
		{
			if(FirstPart)
			{
				if((*BitStream & 0x80) == 0x00)
				{
					Data <<=1;
					Data = Data ^ (1<<0);
					*BitStream <<=1;
				}else{
					*BitStream <<=1;
					FirstPart = false;
					TailCounter = 4;
					}
			}else
				{
					if((*BitStream & 0x80) == 0x80)
					{
						Data <<=1;
						Data = Data ^ (1<<0);
						*BitStream <<=1;
					}else{
						Data <<=1;
						*BitStream <<=1;
						}
					TailCounter--;
					if(TailCounter==0)
					{
						FirstPart = true;

						if((Data & 0x01)==0x00)
						{
							Data>>=1;
						}else{
								Data>>=1;
								Data = 0xff - Data; 
							}
					}
				}
		}
	}
}
uint8_t GetFrameNumber(uint8_t * n,FILE * File)
{
	uint8_t Bytes=0;

	if(*n<0x7F){
	Bytes=1;
	}else
		{
			if(*n<0xDF){
			Bytes=2;
			}else{
					if(*n<0xEF){
					Bytes=3;
					}else{
							if(*n<0xF7){
							Bytes=4;
							}else{
									if(*n<0xFB){
									Bytes=5;
									}else{
											if(*n<0xFD){
											Bytes=6;}
									}}}}
		}
	n++;
	fread(n, Bytes, 1, File);
	return Bytes-1;
}

int main(array<System::String ^> ^args)
{
	Seekpoint sp;
	//sp.TargetSampleInFrame = 999911111111;
	//sp.FrameHeaderOffset = 6667778899;
	//sp.NumberOfSamples = 55689;
	//std::cout<<sizeof(sp);
	FlacDecoder FD;
	void * Temp;
	uint8_t * Temp8;
	uint32_t TempData=0;
	uint32_t fseek_offset =0;
	char *fileName = "c:\\out_2ch_indep_1024.flac";
    FILE *FlacFile = fopen(fileName, "rb");
	
	FlacHeader * FHEADER = new FlacHeader;
	FrameHeader FRAMEHEADER;
	uint8_t  SubframeHeader;

	fread(FHEADER, FlacHeaderSize, 1, FlacFile);
    unsigned t=0;
	if((FHEADER->FlacId[0]==0x66)&&
		(FHEADER->FlacId[1]==0x4C)&&
		(FHEADER->FlacId[2]==0x61)&&
		(FHEADER->FlacId[3]==0x43))
	{
		
		printf("eto flac ");
		fseek_offset += FlacHeaderSize;
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
			TempData = *(uint32_t *)MetadataBlockHeader;
			TempData = _byteswap_ulong(TempData);
			TempData &= 0x00ffffff;
			switch (MetadataBlockHeader[0]&0x7f)		
			{
				case 3:{	//seektable
						fread(&sp, 18, 1, FlacFile);
						sp.NumberOfSamples = _byteswap_ushort(sp.NumberOfSamples);
						printf("case 3");
						break;
				};	
				case 4:{	//vorbis comment
						fseek(FlacFile,TempData,SEEK_CUR);
						break;
				};
				case 1:{    //padding
						fseek(FlacFile,TempData,SEEK_CUR);
						break;
				};
				default:{
					return 0;
				}
			}
		};
		
		delete[] MetadataBlockHeader;
		
		//read frame
		fread(&FRAMEHEADER, 5, 1, FlacFile);
		if(FRAMEHEADER.SyncCodeAndStrategy==FrameSyncFixedBlock)
		{
			uint16_t BlockSize = FRAMEHEADER.BlockSizeAndSampleRate >>4;
			char FrameRate = FRAMEHEADER.BlockSizeAndSampleRate & 0x0f;
			std::cout<<FrameSampleRate[FrameRate]<<std::endl;;
			std::cout<<FrameBlockSize[BlockSize]<<std::endl;;
			
			BlockSize = FrameBlockSize[BlockSize];
			
			char Chanels = FRAMEHEADER.ChanelsAndSampleSize >>4;
			char BitPerSample = FRAMEHEADER.ChanelsAndSampleSize & 0x0f;
			BitPerSample = BitPerSample >> 1;
			std::cout<<FrameBitPerSample[BitPerSample]<<std::endl;;
			
			//InitCRC16();
			uint8_t a[5] ={0xFF, 0xF8, 0x59, 0x18, 0x00}; 
			int b =CRC8(a,5);
			//uint16_t b =CRC16(a,5);
			char qq = CRC8((uint8_t*)&FRAMEHEADER,5+GetFrameNumber(&FRAMEHEADER.FrameNumber[0],FlacFile));
			//uint8_t * ttt = new uint8_t[3145];
			//fseek(FlacFile,0x88,SEEK_SET);
			//fread(ttt, 3145, 1, FlacFile);
			//b =CRC16(ttt,3145);
			
			uint8_t PredictionOrder = 0;
			
			fread(&SubframeHeader, 1, 1, FlacFile); //read SubframeHeader
			{
				if((SubframeHeader & 0x81)==0x0) //check IsIt Subframe Header
				{
					SubframeHeader >>=1;
					if((SubframeHeader & 0x8)==0x08) //check IsIt subframe = FIXED
					{
						PredictionOrder = SubframeHeader && 0x07;	//get fixed prediction order;
					}

					if((SubframeHeader & 0x20)==0x20) //check IsIt subframe = LPC
					{
						PredictionOrder = SubframeHeader && 0x1F;	//get LPC prediction order;
						PredictionOrder -=1;
					}
				}else {exit;}
			}

			uint32_t WarmSamples[4] = {0,0,0,0};

			for (uint8_t i=0; i<PredictionOrder;i++)
			{
				fread(&WarmSamples[i],2, 1, FlacFile); //for 16 bit
			}

			uint8_t Residual = 0;
			uint8_t PartitionOrder = 0;
			uint8_t RiceParameter = 0;
			uint8_t SamplesInPartition = 0;
			bool IsFirstPartition =1;

			fread(&Residual,1, 1, FlacFile); // read residual byte
			
			if((Residual & 0xC0)==0x00) //residual coding method 00
			{
				PartitionOrder = Residual & 0x3C;
				PartitionOrder >>=2;
			}

			if(IsFirstPartition==true)
			{
				//SamplesInPartition = (BlockSize / std::pow( 2, PartitionOrder )) -1;
				PartitionOrder = 2 << PartitionOrder-1;
				SamplesInPartition = (BlockSize / PartitionOrder) -1;
			}else{
					SamplesInPartition = (BlockSize / std::pow( 2, PartitionOrder ));
				}
			
			RiceDecoding(&Residual,0,RiceParameter,1);
			
		}
		
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