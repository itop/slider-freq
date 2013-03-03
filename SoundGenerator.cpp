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
    m_waveHeader.dwLoops = 1;//0xFFFFFFFF; //Lots of loops
    
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

/*
so, y is the output waveform. 
as all wavesforms, it's a function 
of time y(t). t is the time, 
measured in seconds, 
and sampled at 44100 hertz. 
w is a variable that takes a 
different numerical value depending 
which key/note you are playing. it's 
measured in radians, so for example, 
when you play the note A, which in 
standard instruments is tuned 
ti 440 Hertz, w takes the value 2*PI*440.
*/

void SoundGenerator::FillBuffer(int freq, float amp)
{
    float x = freq*3.14159265; //Corresponds to "w"
    int sample = 0;
    int maxVal = pow(2.0f,m_waveFormat.wBitsPerSample) - 1;
    int sampleNum = 0;
    for(unsigned int i = 0; i < m_waveHeader.dwBufferLength; i+=m_waveFormat.nBlockAlign)
    {
        float t = (float)i/m_waveHeader.dwBufferLength; //Our buffer length just happens to be a second long, so this is our time in seconds
        float y;

        //"Piano" by iq
        y  = 0.6*sin(1.0*x*t)*exp(-0.0008*x*t);
        y += 0.3*sin(2.0*x*t)*exp(-0.0010*x*t);
        y += 0.1*sin(4.0*x*t)*exp(-0.0015*x*t);
        y += 0.2*y*y*y;
        y *= 0.9 + 0.1*cos(70.0*t);
        y  = 2.0*y*exp(-22.0*t) + y;

        //Clamp y to 0..1
        y = y > 1.0 ? 1.0 : y;
        y = y < 0.0 ? 0.0 : y;

        m_pWave[sampleNum] = y*amp;
        //We fill sample with an integer of amplitude appropriate to the bytes per sample
        sample = m_pWave[sampleNum] * maxVal/2.0;
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