#include <cstdint>

#define FlacIdSize 4
#define FlacHeaderSize 42
#define MetadataBlockHeaderSize 4
#define MetadataBlockStreamInfoSize 34
#define FrameSyncFixedBlock 0xf8ff	//bigendian 0xfff8

//uint8_t GetFrameNumber(uint8_t * n,FILE * File);

typedef struct _MetadataBlockStreamInfo
{
	uint16_t	MinBlockSize;
	uint16_t	MaxBlockSize;
	uint8_t		MinFrameSize[4];
	uint8_t		MaxFrameSize[4];
	uint32_t	SampleRateCHBit;
	uint32_t	TotalSamplesInStream;
	uint8_t		MD5[16];
} MetadataBlockStreamInfo;
typedef struct _Seekpoint
{
	uint64_t	TargetSampleInFrame;
	uint64_t	FrameHeaderOffset;
	uint16_t	NumberOfSamples;

} Seekpoint;
typedef struct _FlacHeader
{
	char	FlacId[4];		// (0x664C6143 в big-endian представлении "fLaC")
	char	MetadataBlockHeader[4];		//uint32_t  MetadataBlockHeader;
	MetadataBlockStreamInfo	MBSI;
} FlacHeader;
typedef struct _FrameHeader
{
	uint16_t	SyncCodeAndStrategy;
	uint8_t		BlockSizeAndSampleRate;
	uint8_t		ChanelsAndSampleSize;
	uint8_t		FrameNumber[6];
	uint8_t		CRC8;
} FrameHeader;
typedef struct _FrameFooter{
	uint16_t	CRC16;
} FrameFooter;

//MetadataBlockHeader
//------------------------------------------------------------------------
//1-bit last block flag | 7-bits block type | 24-bits length of metadata |
//------------------------------------------------------------------------

uint32_t FrameSampleRate[12] = {0,88200,176400,192000,8000,16000,22050,24000,32000,44100,48000,96000};
uint16_t FrameBlockSize[16]={0,192,576,1152,2304,4608,0,0,256,512,1024,2048,4096,8192,16384,32768};
uint8_t FrameBitPerSample[7]={0,8,12,0,16,20,24};
enum FSampleRate{
	sr_88_2khz,
	sr_176_4kHz,
	sr_192kHz,
	sr_8kHz,
	sr_16kHz,
	sr_22_05kHz,
	sr_24kHz,
	sr_32kHz,
	sr_44_1kHz,
	sr_48kHz,
	sr_96kHz
};