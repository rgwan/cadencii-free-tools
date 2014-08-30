#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define perror(fmt, arg...) printf(fmt, ## arg);exit(0);
#if defined(__MINGW32__)
char RUCE_EXEC[] = "RUCE.exe";
#else
char RUCE_EXEC[] = "RUCE_CLI";
#endif

const char DEFAULT_FLAGS[] = "B50";
const char DBG_NAME[] = "rucedbg.txt";

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

int findCh(unsigned char *whole, char part)
{
	int i;
	int len = strlen(whole);
	for(i = 0;i <= len;i ++)
	{
		if(whole[i] == part)
			return i;
	}
	return -1;
}


int main(int argc,char *argv[])
{
	int i, j, k;
	char TempoStr[10];
	char *PitchStr;
	int Tempo;
	char **argt;
	char *RUCE_PATH, *DBG_PATH;
	char *BASE_PATH, *DBG_FILE = NULL;
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
		if(i == 5)
		{
			if(!strcmp(argv[5], ""))
			{
				/** default set argv[5] to "B50" **/
				argt[5] = malloc(strlen(DEFAULT_FLAGS) + 1);
				strcpy(argt[5], DEFAULT_FLAGS);
			}
			else
			{
				if(findCh(argv[5],'V') != -1)
				{

					DBG_PATH = basename(argv[1]);
					if(DBG_PATH == NULL)
					{
						DBG_FILE = malloc(sizeof(DBG_NAME) + 20);
						sprintf(DBG_FILE," >> %s 2>&1",DBG_NAME);
					}
					else
					{
						DBG_FILE = malloc(sizeof(DBG_NAME) + strlen(DBG_PATH) + 20);
						sprintf(DBG_FILE," >> %s%s 2>&1",DBG_PATH,DBG_NAME);
					}
				}
				goto normalcpy;
			}
		}
		else
		{
			normalcpy:
			argt[i] = malloc(strlen(argv[i]) + 1);
			strcpy(argt[i], argv[i]);
		}
	}
	
	if(argc <= 12)
	{
		k = 0;
		for(i=0;i<j;i++)
		{
			k+=strlen(argt[i]) + 4;
		}
		
		if(DBG_FILE != NULL)
			k += 80;
		Buffer = malloc(k);
		memset(Buffer,0,k - 1);
		int offset = 0;
		for(i=0;i<j;i++)
		{
			int len = strlen(argt[i]);
			if(i == 0)
			{
				Buffer[offset] = ' ';
				Buffer[offset + len + 1]=' ';
			}
			else
			{
				Buffer[offset] = '\"';
				Buffer[offset + len + 1]='\"';	
			}
			Buffer[offset + len + 2]=' ';	
			memcpy(Buffer + offset + 1, argt[i], len);
			offset += len + 3;
		}
		if(DBG_FILE != NULL)
		{
			memcpy(Buffer + offset, DBG_FILE, strlen(DBG_FILE));
			printf("Debug\n");
		}
		printf("Calling %s\n",Buffer);
		
		system(Buffer); /**** using simple way to call it... */
		
		for(i=0;i<j;i++)
		{
			free(argt[i]);
		}
		free(argt);
		exit(0);
	}
	
	printf("Pitch Bend converting...\n");
	
	k = argc - 13;
	sscanf(argv[12], "%*[0-9.-]Q%[0-9.-]", TempoStr);
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
	
	if(DBG_FILE != NULL)
		k+=80;
	Buffer = malloc(k);
	if(DBG_FILE == NULL)
	{
		sprintf(Buffer,"%s \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"",argt[0],argt[1],argt[2],argt[3],argt[4],argt[5],argt[6],argt[7],argt[8],argt[9],argt[10],argt[11],argt[12],argt[13]);
	}
	else
	{
		printf("Debug\n");
		sprintf(Buffer,"%s \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" %s",argt[0],argt[1],argt[2],argt[3],argt[4],argt[5],argt[6],argt[7],argt[8],argt[9],argt[10],argt[11],argt[12],argt[13],DBG_FILE);
	}
	printf("Calling %s\n",Buffer);
	
	system(Buffer); /**** using simple way to call it... */
	/*** free bufferes ***/
	free(Buffer);
	free(PitchStr);
	for(i=0;i<14;i++)
	{
		if(i == 12) continue;
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
