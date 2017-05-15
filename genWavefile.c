#define _CRT_SECURE_NO_WARNINGS 1
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// 44100Hz, 8bit, 1ch のWAVデータ
#define SMPL 44100
#define BIT  8

double divide(double a, double b, double *remainder)
{
	double quotient = floor(a / b);
	
	*remainder = a - (quotient * b);
	
	return quotient;
}

double f1(double x, double a, double b)
{
	if (x < a / 2) {
		return 2 * b / a * x;
	}
	else {
		//return 2 * b / a * (x - a / 2) - b;
		return 2 * b * x / a - 2 * b;
	}
}

double sinwave(double x, double cycle, double b)
{
//#ifdef _WINDOWS
//#define M_PI 3.14L
//#endif
	return(b * sin(x * M_PI * 2.0L / cycle));
}

double f2(double x, double cycle, double b)
{
	double r;
	divide(x, cycle, &r);
	return f1(r, cycle, b);
}


void wav_write(const char *filename, char *buf, int size)
{
 int filesize = 44 + size;
 char *work = (char *) malloc(filesize);
 FILE *fp = fopen(filename, "wb");

 if (fp == NULL) return;

 /* RIFFヘッダ */
 memcpy(work, "RIFF", 4);
 work[4] = (filesize - 8) >> 0  & 0xff;
 work[5] = (filesize - 8) >> 8  & 0xff;
 work[6] = (filesize - 8) >> 16 & 0xff;
 work[7] = (filesize - 8) >> 24 & 0xff;
 /* WAVEヘッダ */
 memcpy(work+8, "WAVE", 4);
 /* fmtチャンク */
 memcpy(work+12, "fmt ", 4);
 work[16] = 16;
 work[17] = work[18] = work[19] = 0;
 work[20] = 1;
 work[21] = 0;
 work[22] = 1;
 work[23] = 0;
 work[24] = SMPL >> 0  & 0xff;
 work[25] = SMPL >> 8  & 0xff;
 work[26] = SMPL >> 16 & 0xff;
 work[27] = SMPL >> 24 & 0xff;
 work[28] = (SMPL * (BIT / 8)) >> 0  & 0xff;
 work[29] = (SMPL * (BIT / 8)) >> 8  & 0xff;
 work[30] = (SMPL * (BIT / 8)) >> 16 & 0xff;
 work[31] = (SMPL * (BIT / 8)) >> 24 & 0xff;
 work[32] = ((BIT / 8)) >> 0 & 0xff;
 work[33] = ((BIT / 8)) >> 8 & 0xff;
 work[34] = BIT >> 0 & 0xff;
 work[35] = BIT >> 8 & 0xff;
 /* dataチャンク */
 memcpy(work+36, "data", 4);
 work[40] = size >> 0  & 0xff;
 work[41] = size >> 8  & 0xff;
 work[42] = size >> 16 & 0xff;
 work[43] = size >> 24 & 0xff;
 memcpy(work + 44, buf, size);

 /* 書き出し */
 fwrite(work, filesize, 1, fp);
 fclose(fp);
 free(work);
}

int main(int argc, char* argv[])
{
 int size;
 char *buf;
	FILE* fp;
	fp = fopen("####.csv", "w");
	
	if (argc<3) {
		printf("param: time, freq");
		return(-1);
	}
	
	double (*WaveFunc)(double, double, double) = sinwave;

	double t = atol(argv[1]); //50.0L; // playing time
	
 size = (SMPL*(BIT/8)) * t; /* 1秒間 */
 buf = (char *) malloc(size);

 //memset(buf, 128, size);
	double freq = atol(argv[2]); //1000.0L;

	double cycle = size / freq / t;
	
	int i;
	for (i = 0; i < size; i++) { // i = 0 .. 44k
		//if (i%1000==0) {
		//	printf("i=%d\n", i);
		//}
	  //double w = floor(i * (440.0 / (double)SMPL));   // ノコギリ波生成 // SMPL=44.1k
	    // w = 0 .. 440.0
		// w = f2(i, cycle, 220.0L) + 220;
#if 0
		double w = f2((double)i, cycle, 220.0L) + 220.0L;
		buf[i] = (char)(127.0 / w) + 128;               // 8bit量子化してバッファへ
#endif
		double w = WaveFunc((double)i, cycle, 64.0L) + 64.0L;
		buf[i] = (char)(w);               // 8bit量子化してバッファへ

		fprintf(fp, "%d,%lf\n", (unsigned char)buf[i], w);
	}	
	fclose(fp);
 wav_write("test.wav", buf, size);

 free(buf);
 return 0;
}