#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(int argv, char **argc)
{
    MONO_PCM pcmForWrite, pcmForRead;
    int i;
    double amplitude, frequency;

    pcmForWrite.fs = 8000;
    pcmForWrite.bits = 16;
    pcmForWrite.length = 8000*3;
    pcmForWrite.s = calloc(pcmForWrite.length, sizeof(double));

    amplitude = 0.25;
    //frequency = 400.0;
    frequency = 444.0*2;

    for(i = 0; i < pcmForWrite.length; i++)
    {
        pcmForWrite.s[i] = amplitude * sin((2.0 * M_PI * frequency * i) / pcmForWrite.fs);
        // A sin(2πft0/fs) + A/2 sine(2πft1/fs) ・・・
    }
    monoWaveWrite(&pcmForWrite, "testWave.wav");
    free(pcmForWrite.s);

    monoWaveRead(&pcmForRead, "testWave.wav");
    for(i = 0; i < pcmForRead.length; i++)
    {
        printf("%d,%lf\n", i, pcmForRead.s[i]);
    }
    free(pcmForRead.s);

    return 0;
}
