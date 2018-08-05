#include <cstdint>

typedef struct _MetadataBlockStreamInfo
{
	uint16_t	MinBlockSize;
	uint16_t	MaxBlockSize;
	uint32_t	MinFrameSize;
	uint32_t	MaxFrameSize;
	uint32_t	SampleRateCHBit;
	uint32_t	TotalSamplesInStream;
} MetadataBlockStreamInfo;

typedef struct _FlacHeader
{
	
    // (0x664C6143 в big-endian представлении "fLaC")
	char	FlacId[4];
	//uint32_t  MetadataBlockHeader;
	char	MetadataBlockHeader[4];
	MetadataBlockStreamInfo	MBSI;
} FlacHeader;


	