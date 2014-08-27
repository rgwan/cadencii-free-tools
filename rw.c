#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define perror(fmt, arg...) printf(fmt, ## arg);exit(0);
#if defined(__MINGW32__)
char RUCE_EXEC[] = "RUCE.exe";
#else
char RUCE_EXEC[] = "RUCE-CLI";
#endif

const char DEFAULT_FLAGS[] = "B50";

/***** Icenowy $ *******/
char encode(int i)
{
	if(i == 62) return '+';
	if(i == 63) return '/';
	if(i >= 0 && i <= 25) return i + 'A';
	if(i >= 26 && i <= 51) return i + 71;
	if(i >= 52 && i <= 61) return i - 4;
	return ' ';
}

/**** rgwan$ ****/
char *basename(char *input)
{
	int i, len = strlen(input);
	char *ret=NULL;
	for(i = len; i>=0; i--)
	{
	#if defined(__MINGW32__)
		#define s_ch '\\'
	#else
		#define s_ch '/'
	#endif
		if(input[i] == s_ch)
		{
			ret = malloc(i + 2);
			memcpy(ret, input, i);
			ret[i] = s_ch;
			ret[i+1] = 0;
			break;
		}
	}
	return ret;
}

int main(int argc,char *argv[])
{
	int i, j, k;
	char TempoStr[10];
	char *PitchStr;
	int Tempo;
	char **argt;
	char *RUCE_PATH;
	char *BASE_PATH;
	char *Buffer;
	
	/*FILE * fp;
	fp = fopen("F:\\Cadencii\\RUCE-1.0.0-alpha2\\debug.txt","w");
	fprintf(fp,"Hello world!\n");
	if(fp == NULL) exit(0);*/
	
	BASE_PATH = basename(argv[0]);
	if(BASE_PATH == NULL)
	{// In current directory
		RUCE_PATH = RUCE_EXEC;
	}
	else
	{
		RUCE_PATH = malloc(strlen(BASE_PATH) + strlen(RUCE_EXEC) + 1);
		sprintf(RUCE_PATH,"%s%s",BASE_PATH,RUCE_EXEC);
	}
	
	//fprintf(fp,"In:directory %s, RUCE %s\n",BASE_PATH,RUCE_PATH);
	
	printf("RUCE - Cadencii wrapper, a wrapper to convert cadencii style args to utau style by Rocaloid Development Team\n");

	
	if (argc <= 4)
	{
		perror( "Usage:%s : <input file> <output file> <pitch percent>\n" \
				"\t\t<velocity> [<flags> [<offset> <length require>\n" \
				"\t\t[<fixed length> [<end blank> [<volume>\n" \
				"\t\t[<modulation> [<pitch bend>...]]]]]]]\n", argv[0]);
	}
	
	j = (argc <= 12)?argc:14;
	
	argt = malloc((j + 1) * sizeof(size_t));
	argt[j] = NULL;
	argt[0] = malloc(strlen(RUCE_PATH) + 1);
	strcpy(argt[0], RUCE_PATH);
	j = (argc <= 12)?argc:12;
	for(i=1;i<j;i++)
	{
		if(i == 5 && !strcmp(argv[5],""))
		{/** default set argv[5] to "B50" **/
			argt[5] = malloc(strlen(DEFAULT_FLAGS) + 1);
			strcpy(argt[5], DEFAULT_FLAGS);
		}
		else
		{
			argt[i] = malloc(strlen(argv[i]) + 1);
			strcpy(argt[i], argv[i]);
		}
	}
	
	if(argc <= 12)
	{
		/*for(i=0;i<argc;i++)
			printf("%s\n", argt[i]);*/
		for(i=0;i<j;i++)
		{
			k+=strlen(argt[i]) + 4;
		}
		
		//k += 80;
		Buffer = malloc(k);
		memset(Buffer,0,k);
		int offset = 0;
		
		for(i=0;i<j;i++)
		{
			int len = strlen(argt[i]);
			memcpy(Buffer + offset, argt[i], len);
			Buffer[offset + len]=' ';
			offset += len + 1;
		}
		printf("Calling %s\n",Buffer);
		
		system(Buffer); /**** using simple way to call it... */
		exit(0);
	}
	
	printf("Pitch Bend converting...\n");
	
	k = argc - 13;
	sscanf(argv[12], "%*[0-9.]Q%[0-9.]", TempoStr);
	Tempo = atoi(TempoStr);
	printf("Tempo = %d, Pitch Points: %d\n",Tempo, k);
	
	sprintf(TempoStr, "!%d", Tempo);
	argt[12] = TempoStr;
	
	PitchStr = malloc((k + 1) * 2);
	for(i = 0;i<(argc-13); i++)
	{
		j = atoi(argv[13 + i]);
		if(j < 0) j+=4096;
		int low6 = j&0x3F;
		int high6 = (j&0xFC0) >> 6;
		PitchStr[2 * i] = encode(high6);
		PitchStr[2 * i + 1] = encode(low6);
	}
	PitchStr[2 * k] = 0;
	argt[13] = PitchStr;
	
	for(i=0;i<14;i++)
	{
		k+=strlen(argt[i]) + 4;
	}
	
	//k += 80;
	Buffer = malloc(k);
	sprintf(Buffer,"%s %s %s %s %s %s %s %s %s %s %s %s %s %s",argt[0],argt[1],argt[2],argt[3],argt[4],argt[5],argt[6],argt[7],argt[8],argt[9],argt[10],argt[11],argt[12],argt[13]);
	printf("Calling %s\n",Buffer);
	
	system(Buffer); /**** using simple way to call it... */
	/*** free bufferes ***/
	free(Buffer);
	free(PitchStr);
	for(i=0;i<14;i++)
	{
		free(argt[i]);
	}	
	free(argt);
	if(RUCE_PATH!=RUCE_EXEC) free(RUCE_PATH);
	/*if(execv(RUCE_PATH, argt)== -1)
	{
		fprintf(fp,"Execute RUCE Failed!\n");
		printf("Execute RUCE Failed!\n");
	}
	fclose(fp);*/
}
