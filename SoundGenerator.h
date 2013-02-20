#ifndef SOUND_GENERATOR_H
#define SOUND_GENERATOR_H

#include <windows.h>
#include <mmsystem.h>

class SoundGenerator {
public:
    SoundGenerator();
    ~SoundGenerator();

    void Format(unsigned int samples, unsigned char bitsPerSample);
    bool PrepareDevice();
    bool CloseDevice();
    void FillBuffer(int freq, float amp);
    bool Play();
    bool Stop();

private:
    HWAVEOUT m_hDeviceHandle;
    WAVEFORMATEX m_waveFormat;
    WAVEHDR m_waveHeader;
    unsigned char *m_pBuf;
};

#endif