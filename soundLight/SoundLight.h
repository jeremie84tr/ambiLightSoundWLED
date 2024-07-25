//
// Created by secondaire on 25/07/2024.
//

#ifndef WLEDCONTROLLERCPP_SOUNDLIGHT_H
#define WLEDCONTROLLERCPP_SOUNDLIGHT_H

#include <chrono>
#include <thread>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <cmath>
#include <complex>
#include <vector>
#include <mutex>
#include "../alglib/fasttransforms.h"
#include "SoundLightColor.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Ole32.lib")

#define REFTIMES_PER_SEC  1000000
#define REFTIMES_PER_MILLISEC  10000

#define EXIT_ON_ERROR(hres)  \
              if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }
#endif //WLEDCONTROLLERCPP_SOUNDLIGHT_H

class SoundLight {
private:
    bool reverse;
    int divider = 4;
    int fourrierSize = -1;
    int maxFreq = 0;
    int NB_LEDS;
    int loopCaptureCount = 0;
    int loopRenderCount = 0;
    int NB_AMORTISSEMENT;
    double audioLevel;
    double* fourrierValues;
    uint8_t lastIp;
    alglib_impl::ae_vector signal;
    alglib_impl::ae_vector a;
    alglib_impl::ae_state state;
    IMMDeviceEnumerator* pEnumerator;
    IMMDevice* pDevice;
    IAudioClient* pAudioClient;
    IAudioCaptureClient* pCaptureClient;

    void setBytes(char* buffer, double audioLevel, double tourneIn = 0);
    void setBytes(char* buffer, double* audioLevels, int nbVals, double tourneIn = 0);

    void computeFFT(double* output, double* input, int N, int nbVal);

    void processAudio(float* data, UINT32 numFrames);

    void processFourrier(float* data, UINT32 numFrames, int repeat);

    void captureAudio();

    double getAmorti(double* pDouble, int nbValues);
    void getAmorti(double* out, double** ppDouble, int nbValues, int nbAmorti, int start);

    double dist(alglib_impl::ae_complex* val);

    HRESULT initializeWASAPI();

public:
    SoundLight(int nbLeds, uint8_t ip, bool stripReverse, int NB_AMORTISSEMENT);

    ~SoundLight();

    void Start(bool fourrier);
};