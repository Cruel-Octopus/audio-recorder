#include <cstdint>

typedef struct _MetadataBlockStreamInfo
{
	uint16_t	MinBlockSize;
	uint16_t	MaxBlockSize;
	uint8_t		MinFrameSize[4];
	uint8_t		MaxFrameSize[4];
	uint32_t	SampleRateCHBit;
	uint32_t	TotalSamplesInStream;
	uint8_t		MD5[32];
} MetadataBlockStreamInfo;

typedef struct _FlacHeader
{
	char	FlacId[4];		// (0x664C6143 в big-endian представлении "fLaC")
	char	MetadataBlockHeader[4];		//uint32_t  MetadataBlockHeader;
	MetadataBlockStreamInfo	MBSI;
} FlacHeader;


	