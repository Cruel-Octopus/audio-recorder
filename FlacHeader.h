#include <cstdint>

#define FlacIdSize 4
#define FlacHeaderSize 42
#define MetadataBlockHeaderSize 4
#define MetadataBlockStreamInfoSize 34
#define FrameSyncFixedBlock 0xfff8

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
	uint32_t	FrameNumber;
	uint8_t		CRC8;
} FrameHeader;
typedef struct _FrameFooter{
	uint16_t	CRC16;
} FrameFooter;

//MetadataBlockHeader
//------------------------------------------------------------------------
//1-bit last block flag | 7-bits block type | 24-bits length of metadata |
//------------------------------------------------------------------------