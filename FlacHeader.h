#include <cstdint>

#define FlacHeaderSize 42
#define MetadataBlockHeaderSize 4
#define MetadataBlockStreamInfoSize 34

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

} Seelpoint;

typedef struct _FlacHeader
{
	char	FlacId[4];		// (0x664C6143 в big-endian представлении "fLaC")
	char	MetadataBlockHeader[4];		//uint32_t  MetadataBlockHeader;
	MetadataBlockStreamInfo	MBSI;
} FlacHeader;


	