// WavHeader.h
/*
+++++++++++++++++++++++++++++++++++++
		|				|			|
Offset	|Field Name		|Fild Size	|
(bytes) |				|(bytes)	|
+++++++++++++++++++++++++++++++++++++________________
	0	|chunkId		|	4		|
	4	|chunkSize		|	4		|	RIFF chunck
	8	|format			|	4		|________________
	12	|subchunk1Id	|	4		|
	16	|subchunk1Size	|	4		|
	20	|audioFormat	|	2		|
	22	|numChannels	|	2		|	fmt sub-chunk
	24	|sampleRate		|	4		|
	28	|byteRate		|	4		|
	32	|blockAlign		|	2		|
	34	|bitsPerSample	|	2		|________________
	36	|subchunk2Id	|	4		|
	40	|subchunk2Size	|	4		|	data sub-chunk
	44	|data			|subchunk2Size|
++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

typedef struct _WAVHEADER
{
	// WAV-������ ���������� � RIFF-���������:
    // �������� ������� "RIFF" � ASCII ���������
    // (0x52494646 � big-endian �������������)
    char chunkId[4];

	unsigned long chunkSize; // ������ wav ����� ����� ��� chunkId+chunkSize
	
	// �������� ������� "WAVE"
    // (0x57415645 � big-endian �������������)
	char format[4];

	// �������� ������� "fmt "
    // (0x666d7420 � big-endian �������������)
    char subchunk1Id[4];

	// 16 ��� ������� PCM.
    // ��� ���������� ������ ����������, ������� � ���� �������.
    unsigned long subchunk1Size;

	// ��� PCM = 1 (�� ����, �������� �����������).
    // ��������, ������������ �� 1, ���������� ��������� ������ ������.
    unsigned short audioFormat;

	// ���������� �������. ���� = 1, ������ = 2 � �.�.
    unsigned short numChannels;

	// ������� �������������. 8000 ��, 44100 �� � �.�.
    unsigned long sampleRate;

	// sampleRate * numChannels * bitsPerSample/8
    unsigned long byteRate;

	// numChannels * bitsPerSample/8
    // ���������� ���� ��� ������ ������, ������� ��� ������.
    unsigned short blockAlign;

	// ��� ���������� "��������" ��� �������� ��������. 8 ���, 16 ��� � �.�.
    unsigned short bitsPerSample;

	// ���������� "data" �������� �����-������ � �� ������.
    // �������� ������� "data"
    // (0x64617461 � big-endian �������������)
    char subchunk2Id[4];

	// numSamples * numChannels * bitsPerSample/8
    // ���������� ���� � ������� ������.
    unsigned long subchunk2Size;

	// ����� ������� ��������������� Wav ������.
} WAVHEADER;