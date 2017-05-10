#include "wave.h"

void monoWaveRead(MONO_PCM *pcm, char *fileName)
{
    FILE *fp;
    int i;
    WAVE_FORMAT waveFormat;

    fp = fopen(fileName, "rb");

    if(!fp)
    {
        printf("file open error");
        exit(0);
    }

    fread(waveFormat.riffChunk.chunkID, 1, 4, fp);
    fread(&waveFormat.riffChunk.chunkSize, 4, 1, fp);
    fread(waveFormat.riffChunk.chunkFormType, 1, 4, fp);
    // RIFFチャンクの読み込み

    fread(waveFormat.fmtChunk.chunkID, 1, 4, fp);
    fread(&waveFormat.fmtChunk.chunkSize, 4, 1, fp);
    fread(&waveFormat.fmtChunk.waveFormatType, 2, 1, fp);
    fread(&waveFormat.fmtChunk.formatChannel, 2, 1, fp);
    fread(&waveFormat.fmtChunk.samplesPerSec, 4, 1, fp);
    fread(&waveFormat.fmtChunk.bytesPerSec, 4, 1, fp);
    fread(&waveFormat.fmtChunk.blockSize, 2, 1, fp);
    fread(&waveFormat.fmtChunk.bitsPerSample, 2, 1, fp);
    // fmtチャンクの読み込み

    fread(waveFormat.dataChunk.chunkID, 1, 4, fp);
    fread(&waveFormat.dataChunk.chunkSize, 4, 1, fp);

    pcm->fs = waveFormat.fmtChunk.samplesPerSec;
    pcm->bits = waveFormat.fmtChunk.bitsPerSample;
    pcm->length = waveFormat.dataChunk.chunkSize;
    pcm->s = calloc(pcm->length, sizeof(double));

    short data;
    for(i = 0; i < pcm->length; i++)
    {
        fread(&data, 2, 1, fp);
        pcm->s[i] = (double)data/GROUND;
    }
    // dataチャンクの読み込み

    fclose(fp);
}

void monoWaveWrite(MONO_PCM *pcm, char *fileName)
{
    FILE *fp;
    int i;
    WAVE_FORMAT waveFormat;

    strcpy(waveFormat.riffChunk.chunkID, "RIFF");
    waveFormat.riffChunk.chunkSize = 36 + pcm->length * 2;
    strcpy(waveFormat.riffChunk.chunkFormType, "WAVE");

    strcpy(waveFormat.fmtChunk.chunkID, "fmt ");
    waveFormat.fmtChunk.chunkSize = 16;
    waveFormat.fmtChunk.waveFormatType = 1;
    // PCMの場合は1
    waveFormat.fmtChunk.formatChannel = 1;
    // モノラルの場合は1,ステレオの場合は2
    waveFormat.fmtChunk.samplesPerSec = pcm->fs;
    waveFormat.fmtChunk.bytesPerSec = (pcm->fs * pcm->bits) / 8;
    waveFormat.fmtChunk.blockSize = pcm->bits / 8;
    waveFormat.fmtChunk.bitsPerSample = pcm->bits;

    strcpy(waveFormat.dataChunk.chunkID, "data");
    waveFormat.dataChunk.chunkSize = pcm->length * 2;

    fp = fopen(fileName, "wb");

    if(!fp)
    {
        printf("file open error");
        exit(0);
    }

    fwrite(waveFormat.riffChunk.chunkID, 1, 4, fp);
    fwrite(&waveFormat.riffChunk.chunkSize, 4, 1, fp);
    fwrite(waveFormat.riffChunk.chunkFormType, 1, 4, fp);
    // RIFFチャンクの書き込み

    fwrite(waveFormat.fmtChunk.chunkID, 1, 4, fp);
    fwrite(&waveFormat.fmtChunk.chunkSize, 4, 1, fp);
    fwrite(&waveFormat.fmtChunk.waveFormatType, 2, 1, fp);
    fwrite(&waveFormat.fmtChunk.formatChannel, 2, 1, fp);
    fwrite(&waveFormat.fmtChunk.samplesPerSec, 4, 1, fp);
    fwrite(&waveFormat.fmtChunk.bytesPerSec, 4, 1, fp);
    fwrite(&waveFormat.fmtChunk.blockSize, 2, 1, fp);
    fwrite(&waveFormat.fmtChunk.bitsPerSample, 2, 1, fp);
    // fmtチャンクの書き込み

    fwrite(waveFormat.dataChunk.chunkID, 1, 4, fp);
    fwrite(&waveFormat.dataChunk.chunkSize, 4, 1, fp);

    short data;
    double s;
    for(i = 0; i < pcm->length; i++)
    {
        s = ((pcm->s[i] + 1.0) / 2) * (MAXPLUS + 1.0);
    	
        if(s > MAXPLUS)
            s = MAXPLUS;
        else if(s < MAXMINUS)
            s = MAXMINUS;

        data = (short)(s + 0.5) - GROUND;
        fwrite(&data, 2, 1, fp);
    }

    fclose(fp);
}
