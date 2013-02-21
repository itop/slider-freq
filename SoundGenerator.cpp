#include "SoundGenerator.h"
#include <windows.h>
#include <mmsystem.h>
#include <math.h>

SoundGenerator::SoundGenerator()
{
    m_pBuf = NULL;
    m_pWave = NULL;
}

SoundGenerator::~SoundGenerator()
{
    if(m_pBuf)
    {
        delete[] m_pBuf;
    }
    m_pBuf = NULL;

    if(m_pWave)
    {
        delete[] m_pWave;
    }
    m_pWave = NULL;
}

void SoundGenerator::Format(unsigned int samples, unsigned char bitsPerSample)
{
    m_waveFormat.nSamplesPerSec = samples;                              //Samples per second
    m_waveFormat.wBitsPerSample = bitsPerSample;                        //Bits per sample
    m_waveFormat.nChannels = 1;                                         //Number of channels
    m_waveFormat.cbSize = sizeof(WAVEFORMATEX);                         //Data size
    m_waveFormat.wFormatTag = WAVE_FORMAT_PCM;                          //Wave format
    m_waveFormat.nBlockAlign = (bitsPerSample / 8);                     //Size in bytes of a block
    m_waveFormat.nAvgBytesPerSec = m_waveFormat.nBlockAlign * samples;  //Per second bandwidth

    if(m_pBuf)
    {
        delete[] m_pBuf;
    }

    m_pBuf = new unsigned char[m_waveFormat.nAvgBytesPerSec];
    m_pWave = new float [samples];
}

bool SoundGenerator::PrepareDevice()
{
    if(waveOutOpen(&m_hDeviceHandle, WAVE_MAPPER, &m_waveFormat, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR)
    {
        return false;
    }

    m_waveHeader.lpData = (LPSTR)m_pBuf;
    m_waveHeader.dwBufferLength = m_waveFormat.nAvgBytesPerSec;
    m_waveHeader.dwFlags = WHDR_BEGINLOOP;
    m_waveHeader.dwLoops = 1;
    
    if(waveOutPrepareHeader(m_hDeviceHandle, &m_waveHeader, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
    {
        return false;
    }

    m_waveHeader.dwFlags=WHDR_BEGINLOOP|WHDR_ENDLOOP|WHDR_PREPARED;
    m_waveHeader.dwLoops=0xFFFFFFFF; //Lots of loops
    
    //Everything was ok
    return true;
}

bool SoundGenerator::CloseDevice()
{
    if(waveOutReset(m_hDeviceHandle) != MMSYSERR_NOERROR)
    {
        return false;
    }

    if(waveOutClose(m_hDeviceHandle) != MMSYSERR_NOERROR)
    {
        return false;
    }
}

void SoundGenerator::FillBuffer(int freq, float amp)
{
    float x = freq*3.1415935/m_waveFormat.nSamplesPerSec;
    int sample = 0;
    int maxVal = pow(2.0f,m_waveFormat.wBitsPerSample) - 1;
    int sampleNum = 0;
    for(unsigned int i = 0; i < m_waveHeader.dwBufferLength; i+=m_waveFormat.nBlockAlign)
    {
        m_pWave[sampleNum] = cos(x*i)*amp;
        //We fill sample with an integer of amplitude appropriate to the bytes per sample
        sample = m_pWave[sampleNum] * maxVal/2;
        memcpy(&m_pBuf[i], &sample, m_waveFormat.nBlockAlign);
        ++sampleNum;
    }
}

const float *SoundGenerator::GetWave()
{
    return m_pWave;
}

bool SoundGenerator::Play()
{
    if(waveOutReset(m_hDeviceHandle) != MMSYSERR_NOERROR)
    {
        return false;
    }

    if(waveOutWrite(m_hDeviceHandle, &m_waveHeader,sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
    {
        return false;
    }

    //Everything went ok
    return true;
}

bool SoundGenerator::Stop()
{
    return waveOutReset(m_hDeviceHandle) != MMSYSERR_NOERROR;
}