#include <stdio.h>
#include <string.h>
#include <stdlib.h>

inline char GetCharIndex(char c)
{
	if ((c >= 'A') && (c <= 'Z')) {
		return c - 'A';
	} else if ((c >= 'a') && (c <= 'z')) {
		return c - 'a' + 26;
	} else if ((c >= '0') && (c <= '9')) {
		return c - '0' + 52;
	} else if (c == '+') {
		return 62;
	} else if (c == '/') {
		return 63;
	} else if (c == '=') {
		return 0;
	}
	return 0;
}

int Base64Decode(char *lpString, char *lpSrc)
{
	static char lpCode[4];
	register int vLen = 0;
	register int sLen = strlen(lpSrc);
	/*if(sLen % 4)
	   { lpString[0] = '\0';
	   return -1;
	   } */
	while (sLen > 2) {
		lpCode[0] = GetCharIndex(lpSrc[0]);
		lpCode[1] = GetCharIndex(lpSrc[1]);
		lpCode[2] = GetCharIndex(lpSrc[2]);
		lpCode[3] = GetCharIndex(lpSrc[3]);

		*lpString++ = (lpCode[0] << 2) | (lpCode[1] >> 4);
		*lpString++ = (lpCode[1] << 4) | (lpCode[2] >> 2);
		*lpString++ = (lpCode[2] << 6) | (lpCode[3]);

		lpSrc += 4;
		sLen -= 4;
		vLen += 3;
	}
	return vLen;
}

int readWord(char *Src, char *Dest, int offset)
{
	int i = 0;
	while (Src[i + offset] != ' ') {
		Dest[i] = Src[i + offset];
		i++;
	}
	Dest[i] = 0;
	return i + 1 + offset;
}

int WriteToWaveFile(FILE * fp, char *fileName, int offset, int length)
{
	FILE *writefp;
	unsigned int temp;
	unsigned short temp1;
	void *Buffer = malloc(length);
	if ((writefp = fopen(fileName, "wb")) == NULL) {
		printf("Can not Save Wave file!\n");
		goto exit_l;
	}
	memset(Buffer, 0, length);
	fseek(fp, (long int)offset, SEEK_SET);
	fread(Buffer, length, 1, fp);

	fseek(writefp, 44L, SEEK_SET);
	fwrite(Buffer, length, 1, writefp);

	fseek(writefp, 0, SEEK_SET);
	fwrite("RIFF", 4, 1, writefp);
	temp = length + 44;
	fwrite(&temp, 4, 1, writefp);
	fwrite("WAVE", 4, 1, writefp);
	fwrite("fmt ", 4, 1, writefp);
	temp = 16;		//Wave
	fwrite(&temp, 4, 1, writefp);

	temp1 = 1;		//Wave
	fwrite(&temp1, 2, 1, writefp);
	temp1 = 1;		//Channel
	fwrite(&temp1, 2, 1, writefp);
	temp = 44100;		//Samplerate
	fwrite(&temp, 4, 1, writefp);
	temp = 1 * 44100 * 16 / 8;	//Byte per second
	fwrite(&temp, 4, 1, writefp);
	temp1 = 1 * 16 / 8;
	fwrite(&temp1, 2, 1, writefp);
	temp1 = 16;		//Sample size
	fwrite(&temp1, 2, 1, writefp);

	fwrite("data", 4, 1, writefp);
	temp = length;
	fwrite(&temp, 4, 1, writefp);
	fclose(writefp);
exit_l:
	free(Buffer);
}

int main(int argc, char *argv[])
{
	FILE *fp;
	FILE *fpBinary;
	char *StrLine = malloc(512);
	char *DestLine = malloc(512);
	char *dFileName;
	char *dTemp = malloc(16);
	char *nnVer = malloc(10);
	int offset = 0;
	int length = 0;
	int temp = 0;
	if (argc < 3) {
		printf("Niao Niao Soundbank Extractor Version 1.0-alpha1\n");
		printf("Usage %s: <inf_file> <voice_file> <save_directory>\n", argv[0]);
		printf
		    ("\tExtract Wave file from NiaoNiao Soundbank.\n\tHey dsound!\n");
		printf
		    ("Under GPL v3 Licence. WanZhiYuan 2014 http://weirm.info \n");
		exit(0);
	}
	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("Can not Open NiaoNiao Soundbank inf.d!\n");
		goto freemem;
	}
	if ((fpBinary = fopen(argv[2], "rb")) == NULL) {
		printf("Can not Open NiaoNiao Soundbank voice.d!\n");
		goto freemem;
	}
	dFileName = malloc(strlen(argv[3]) + 16);
	fgets(StrLine, 512, fp);
	Base64Decode(nnVer, StrLine);
	fgets(StrLine, 512, fp);
	printf("nn SoundBank Version:%s\n", nnVer);
	while (!feof(fp)) {
		fgets(StrLine, 512, fp);
		Base64Decode(DestLine, StrLine);
		temp = readWord(DestLine, dTemp, 0);
#if defined(__MINGW32__)
		sprintf(dFileName, "%s\\%s.wav", argv[3], dTemp);
#else
		sprintf(dFileName, "%s/%s.wav", argv[3], dTemp);
#endif
		temp = readWord(DestLine, dTemp, temp);
		offset = atoi(dTemp);
		temp = readWord(DestLine, dTemp, temp);
		length = atoi(dTemp);
		printf("Extracted %s offset = %d length = %d\n", dFileName,
		       offset, length);
		WriteToWaveFile(fpBinary, dFileName, offset, length);
	}
	fclose(fp);
	fclose(fpBinary);
	free(dFileName);
freemem:
	free(StrLine);
	free(DestLine);
	free(dTemp);
	free(nnVer);
	return 0;
}
