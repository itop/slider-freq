#ifndef SOUND_GENERATOR_H
#define SOUND_GENERATOR_H

#include <windows.h>
#include <mmsystem.h>
#include "Modulator.h"

class SoundGenerator {
public:
    SoundGenerator();
    ~SoundGenerator();

    void Format(unsigned int samples, unsigned char bitsPerSample);
    bool PrepareDevice();
    bool CloseDevice();
    void FillBuffer(int freq, float amp, const Modulators::Modulator *mod);
    bool Play();
    bool Stop();
    const float *GetWave();

private:
    HWAVEOUT m_hDeviceHandle;
    WAVEFORMATEX m_waveFormat;
    WAVEHDR m_waveHeader;
    unsigned char *m_pBuf;
    float *m_pWave;
};

#endif