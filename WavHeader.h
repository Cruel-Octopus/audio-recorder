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
	// WAV-формат начинается с RIFF-заголовка:
    // Содержит символы "RIFF" в ASCII кодировке
    // (0x52494646 в big-endian представлении)
    char chunkId[4];

	unsigned long chunkSize; // размер wav части файла без chunkId+chunkSize
	
	// Содержит символы "WAVE"
    // (0x57415645 в big-endian представлении)
	char format[4];

	// Содержит символы "fmt "
    // (0x666d7420 в big-endian представлении)
    char subchunk1Id[4];

	// 16 для формата PCM.
    // Это оставшийся размер подцепочки, начиная с этой позиции.
    unsigned long subchunk1Size;

	// Для PCM = 1 (то есть, Линейное квантование).
    // Значения, отличающиеся от 1, обозначают некоторый формат сжатия.
    unsigned short audioFormat;

	// Количество каналов. Моно = 1, Стерео = 2 и т.д.
    unsigned short numChannels;

	// Частота дискретизации. 8000 Гц, 44100 Гц и т.д.
    unsigned long sampleRate;

	// sampleRate * numChannels * bitsPerSample/8
    unsigned long byteRate;

	// numChannels * bitsPerSample/8
    // Количество байт для одного сэмпла, включая все каналы.
    unsigned short blockAlign;

	// Так называемая "глубиная" или точность звучания. 8 бит, 16 бит и т.д.
    unsigned short bitsPerSample;

	// Подцепочка "data" содержит аудио-данные и их размер.
    // Содержит символы "data"
    // (0x64617461 в big-endian представлении)
    char subchunk2Id[4];

	// numSamples * numChannels * bitsPerSample/8
    // Количество байт в области данных.
    unsigned long subchunk2Size;

	// Далее следуют непосредственно Wav данные.
} WAVHEADER;