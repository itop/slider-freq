#ifndef SOUND_CALLBACK_H
#define SOUND_CALLBACK_H

#include <windows.h>

//We only care about uMsg
void CALLBACK waveOutProc(HWAVEOUT, UINT uMsg, DWORD, DWORD, DWORD);

#endif