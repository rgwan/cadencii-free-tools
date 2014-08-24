#include <stdio.h>
char encode(int i)
{
	if(i == 62) return '+';
	if(i == 63) return '/';
	if(i >= 0 && i <= 25) return i + 'A';
	if(i >= 26 && i <= 51) return i + 71;
	if(i >= 52 && i <= 61) return i - 4;
	return ' ';
}
void base64encoderForUtau(int i)
{
	if(i < 0) i+=4096;
	int low6 = i&0x3F;
	int high6 = (i&0xFC0) >> 6;
	putchar(encode(high6));
	putchar(encode(low6));
} 
int main()
{
	float f;
	int i;
	scanf("%f",&f);
	i = (int)f;
	base64encoderForUtau(i);
}
