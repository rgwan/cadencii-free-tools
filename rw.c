#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define perror(fmt, arg...) printf(fmt, ## arg);exit(0);
const char RUCE_PATH[] = "RUCE.exe";
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


int main(int argc,char *argv[])
{
	int i, j, k;
	char TempoStr[10];
	char *PitchStr;
	int Tempo;
	char **argt;
	
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
		execv(RUCE_PATH, argt);
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
	/*for(i=0;i<14;i++)
		printf("%d %s\n", i ,argt[i]);	*/
	execv(RUCE_PATH, argt);
}