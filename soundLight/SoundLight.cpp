#include "SoundLight.h"

std::mutex isChangingFourrier;
std::mutex isChangingData;

void SoundLight::setBytes(char *buffer, double audioLevel, double tourneIn) {
    uint16_t level = static_cast<uint16_t>(audioLevel * NB_LEDS / 2);
    int tourne = tourneIn;
    int multiplicateur = 1024 / (NB_LEDS / 2);
    for (int i = 0; i < NB_LEDS; ++i) {
        if (i > NB_LEDS / 2 - level && i < NB_LEDS / 2 + level) {
            SoundLightColor couleur;
            if (i < NB_LEDS / 2) {
                couleur = SoundLightColor::getGradient((i - ((NB_LEDS / 2) - level)) * multiplicateur + 64);
                buffer[((i + tourne) % NB_LEDS) * 3 + 2] = couleur.red;
                buffer[((i + tourne) % NB_LEDS) * 3 + 3] = couleur.green;
                buffer[((i + tourne) % NB_LEDS) * 3 + 4] = couleur.blue;
            } else {
                couleur = SoundLightColor::getGradient((level - (i - (NB_LEDS / 2))) * multiplicateur + 64);
                buffer[((i + tourne) % NB_LEDS) * 3 + 2] = couleur.red;
                buffer[((i + tourne) % NB_LEDS) * 3 + 3] = couleur.green;
                buffer[((i + tourne) % NB_LEDS) * 3 + 4] = couleur.blue;
            }
        } else {
            buffer[((i + tourne) % NB_LEDS) * 3 + 2] = 0;
            buffer[((i + tourne) % NB_LEDS) * 3 + 3] = 0;
            buffer[((i + tourne) % NB_LEDS) * 3 + 4] = 0;
        }
    }
}

void SoundLight::setBytes(char* buffer, double* audioLevels, int nbVals, double tourneIn) {
    uint16_t level = nbVals;
    int decalage = 65;
    int tourne = tourneIn + decalage;

    double mult = 16.0;

    int usedValue;
    for (int i = 0; i < NB_LEDS ; ++i) {
        if (i < level || NB_LEDS - i < level) {
            usedValue = i;
            if (usedValue < nbVals && usedValue > 0) {
                SoundLightColor couleur = SoundLightColor::getGradient((int) (audioLevels[usedValue] * 255.0 * mult));
                buffer[((i + tourne) % NB_LEDS) * 3 + 2] = couleur.red;
                buffer[((i + tourne) % NB_LEDS) * 3 + 3] = couleur.green;
                buffer[((i + tourne) % NB_LEDS) * 3 + 4] = couleur.blue;
                couleur = SoundLightColor::getGradient((int) (audioLevels[nbVals - usedValue] * 255.0 * mult));
                buffer[((i + tourne - nbVals) % NB_LEDS) * 3 + 2] = couleur.red;
                buffer[((i + tourne - nbVals) % NB_LEDS) * 3 + 3] = couleur.green;
                buffer[((i + tourne - nbVals) % NB_LEDS) * 3 + 4] = couleur.blue;
            }
        } else if (i - (NB_LEDS / 2) < level && NB_LEDS - i - (NB_LEDS / 2) < level) {
            usedValue = (i - (NB_LEDS / 2));
            if (usedValue < nbVals && usedValue > 0) {
                SoundLightColor couleur = SoundLightColor::getGradient((int) (audioLevels[usedValue] * 255.0 * mult));
                buffer[((i + tourne) % NB_LEDS) * 3 + 2] = couleur.red;
                buffer[((i + tourne) % NB_LEDS) * 3 + 3] = couleur.green;
                buffer[((i + tourne) % NB_LEDS) * 3 + 4] = couleur.blue;
                couleur = SoundLightColor::getGradient((int) (audioLevels[nbVals - usedValue] * 255.0 * mult));
                buffer[((i + tourne - nbVals) % NB_LEDS) * 3 + 2] = couleur.red;
                buffer[((i + tourne - nbVals) % NB_LEDS) * 3 + 3] = couleur.green;
                buffer[((i + tourne - nbVals) % NB_LEDS) * 3 + 4] = couleur.blue;
            }
        } else {
            buffer[((i + tourne) % NB_LEDS) * 3 + 2] = 64;
            buffer[((i + tourne) % NB_LEDS) * 3 + 3] = 64;
            buffer[((i + tourne) % NB_LEDS) * 3 + 4] = 64;
        }
    }
}

double SoundLight::dist(alglib_impl::ae_complex *val)  {
    return val->x * val->x + val->y * val->y;
}

void SoundLight::computeFFT(double *output, double *input, int N, int nbVal) {

    try {
        for (int i = 0; i < N; ++i) {
            a.ptr.p_double[i] = input[i];
        }

//            std::cout << "avant FFT" << std::endl;
        // Calculer la FFT
        if(N > 0) {
            alglib_impl::fftr1d(&a, N, &signal, &state);
        }

//            std::cout << "avant magnitudes" << std::endl;
        double coef = N / nbVal;
        // Extraire les magnitudes

        int iMax = 1;
        double val;
        for (int i = 1; i < N; ++i) {
            val = dist(&signal.ptr.p_complex[i]);
            if (val > dist(&signal.ptr.p_complex[iMax])) {
                iMax = i;
            }
        }
        double max = dist(&signal.ptr.p_complex[iMax]);
//            std::cout << "val " << iMax << " -> " << max << std::endl;
//            std::cout << "estimated freq : " << iMax * ((96000 / N) / divider) << std::endl;

        if (max < 1) {
            max = 1;
        }

        double jStart;
        double jEnd;

        int maxJ = 1;

        int biais = 8;


//            std::cout << "avant output" << std::endl;
        for (int i = biais; i < nbVal; i++) {
            output[i - biais] = 0;
            jStart = pow(((double) i - 0.5) / (double) nbVal, 3.0) * nbVal * 10.0 ;
            jEnd = pow(((double) i + 0.5) / (double) nbVal, 3.0) * nbVal * 10.0 ;
            if (dist(&signal.ptr.p_complex[i]) > dist(&signal.ptr.p_complex[maxJ])) {
                maxJ = i;
            }
            for(int j = jStart; j < jEnd; j++) {
                if (j > 0 && j < N) {
//                        std::cout << i << " add " << (dist(&signal.ptr.p_complex[j])) << std::endl;
                    if (i - biais > 0) {
                        output[i - biais] += ((dist(&signal.ptr.p_complex[j]) / max) * 0.2);
                    }
                }
            }
            //            std::cout << "magnitude : " << output[i] << std::endl;
        }
//            std::cout << "apres output" << std::endl;
        if (dist(&signal.ptr.p_complex[maxJ]) > 0.001) {
            maxFreq = maxJ * ((96000 / N) / divider);
        } else {
            maxFreq = 0;
        }

    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

}

void SoundLight::processAudio(float *data, UINT32 numFrames) {

    double max = 0.0;
    double min = 1.0;
    double val;

    UINT32 i = 0;
    while (i < numFrames) {
        val = (data[i] + 1.0) / 2.0;
        if (val > max) {
            max = val;
        } else if (val < min) {
            min = val;
        }
        i++;
    }
    audioLevel = max - min;
}

void SoundLight::processFourrier(float *data, UINT32 numFrames, int repeat) {
//        std::cout << "avant wait" << std::endl;
    isChangingFourrier.lock();
//        std::cout << "nouveau fourrier" << std::endl;
    UINT32 i = 0;

    while (i < numFrames) {
        double value = data[i % repeat] + 0.5; // valeur entre 0 et 1
//            if (i % 200 == 0) {
//                std::cout << "value : " << value << " ;" << std::endl;
//            }
        fourrierValues[i] = value;
        i++;
    }
    fourrierSize = (int) numFrames;
    isChangingFourrier.unlock();

}

HRESULT SoundLight::initializeWASAPI() {
    HRESULT hr;
    REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
    WAVEFORMATEX* pwfx = nullptr;

    hr = CoInitialize(nullptr);
    EXIT_ON_ERROR(hr);

    hr = CoCreateInstance(
            __uuidof(MMDeviceEnumerator), nullptr,
            CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
            (void**)&pEnumerator);
    EXIT_ON_ERROR(hr);

    hr = pEnumerator->GetDefaultAudioEndpoint(
            eRender, eConsole, &pDevice);
    EXIT_ON_ERROR(hr);

    hr = pDevice->Activate(
            __uuidof(IAudioClient), CLSCTX_ALL,
            nullptr, (void**)&pAudioClient);
    EXIT_ON_ERROR(hr);

    hr = pAudioClient->GetMixFormat(&pwfx);
    EXIT_ON_ERROR(hr);

    hr = pAudioClient->Initialize(
            AUDCLNT_SHAREMODE_SHARED,
            AUDCLNT_STREAMFLAGS_LOOPBACK,
            hnsRequestedDuration,
            0,
            pwfx,
            nullptr);
    EXIT_ON_ERROR(hr);

    hr = pAudioClient->GetService(
            __uuidof(IAudioCaptureClient),
            (void**)&pCaptureClient);
    EXIT_ON_ERROR(hr);

    CoTaskMemFree(pwfx);

    Exit:
    if (FAILED(hr)) {
        CoTaskMemFree(pwfx);
        SAFE_RELEASE(pEnumerator);
        SAFE_RELEASE(pDevice);
        SAFE_RELEASE(pAudioClient);
        SAFE_RELEASE(pCaptureClient);
    }
    return hr;
}

void SoundLight::captureAudio() {
    HRESULT hr;
    UINT32 packetLength = 0;
    BYTE* pData;
    DWORD flags;
    UINT32 numFramesAvailable;
    int size = 512;
    float computedData[size];
    float audioData[size];
    fourrierValues = new double[size];
    int adId = 0;
    int cdId = 0;

    int periodSequence = 0;
    float lastData;

    bool packetDone = false;

    memset(&signal, 0, sizeof(signal));
    memset(&a, 0, sizeof(a));
    ae_vector_init(&signal,size,alglib_impl::DT_COMPLEX,&state,true);
    ae_vector_init(&a,size,alglib_impl::DT_REAL,&state,true);
    alglib_impl::ae_state_init(&state);

//        std::cout << "avant capture" << std::endl;
    hr = pAudioClient->Start();
    if (FAILED(hr)) {
        std::cerr << "Failed to start audio client: " << hr << std::endl;
        return;
    }

    while (true) {
        hr = pCaptureClient->GetNextPacketSize(&packetLength);
        if (FAILED(hr)) {
            std::cerr << "Failed to get next packet size: " << hr << std::endl;
            break;
        }

        packetDone = false;
        while (packetLength != 0) {
            packetDone = true;
//                std::cout << "packet ok" << std::endl;
            hr = pCaptureClient->GetBuffer(
                    &pData,
                    &numFramesAvailable,
                    &flags, nullptr, nullptr);
            if (FAILED(hr)) {
                std::cerr << "Failed to get buffer: " << hr << std::endl;
                break;
            }

            if (flags & AUDCLNT_BUFFERFLAGS_SILENT) {
                pData = nullptr;
            }

            if (pData) {
                //std::cout << "avant process audio" << std::endl;
                float* data = (float*)pData;
//                    processAudio(data, numFramesAvailable);
                for(int i = 0 ; i < numFramesAvailable; i += divider) {
                    if (periodSequence == 0) {
                        if (lastData < 0 && data[i] >= 0) {
                            periodSequence++;
                        }
                    } else {
                        if ((lastData < 0 && data[i] >= 0) || (lastData > 0 && data[i] <= 0)) {
                            periodSequence++;
                        }
                    }
                    lastData = data[i];

                    if(periodSequence > 0) {
                        computedData[cdId] = lastData;
                        cdId++;
                    }
                    if (cdId == size || periodSequence == 21) {
//                            for (int j = 0; j < cdId; ++j) {
//                                std::cout << computedData[j] << " ";
//                            }
//                            std::cout << std::endl;
                        //std::cout << "process " << cdId << " elements : " << (96000 / cdId / divider) << "hz" << std::endl;
//                            std::cout << "processAudio" << std::endl;
                        processFourrier(computedData, size, cdId - 1);
                        cdId = 0;
                        periodSequence = 0;
                    }

                    audioData[adId] = lastData;
                    adId++;
                    if (adId >= size / 3) {
                        loopCaptureCount++;
                        processAudio(audioData, adId);
                        adId = 0;
                    }
                }
            }

            hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
            if (FAILED(hr)) {
                std::cerr << "Failed to release buffer: " << hr << std::endl;
                break;
            }

            //std::this_thread::sleep_for(std::chrono::milliseconds(100));



            hr = pCaptureClient->GetNextPacketSize(&packetLength);
            if (FAILED(hr)) {
                std::cerr << "Failed to get next packet size: " << hr << std::endl;
                break;
            }
        }
        if (!packetDone) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

    }

    pAudioClient->Stop();
}

double SoundLight::getAmorti(double *pDouble, int nbValues) {
    double ret = 0.0;
    for (int i = 0; i < nbValues; i++) {
        ret += sqrt(pDouble[i]);
    }
    return ret / nbValues;
}

void SoundLight::getAmorti(double* out, double** ppDouble, int nbValues, int nbAmorti, int start) {
    double ret;
    for (int i = start - 1; i < nbValues + start - 1; i++) {
        int iVal = (i + nbValues) % nbValues;
        ret = 0;
        for (int j = 0; j < nbAmorti; ++j) {
            ret += ppDouble[j][iVal] * ppDouble[j][iVal] ;
        }
        out[iVal] = (ret / (double) nbValues) * 25.0;
    }
}

SoundLight::SoundLight(int nbLeds, uint8_t ip, bool stripReverse, int NB_AMORTISSEMENT) : audioLevel(0.0), pEnumerator(nullptr), pDevice(nullptr), pAudioClient(nullptr), pCaptureClient(nullptr) {
    this->NB_LEDS = nbLeds;
    this->lastIp = ip;
    this->reverse = stripReverse;
    this->NB_AMORTISSEMENT = NB_AMORTISSEMENT;
}

SoundLight::~SoundLight() {
    SAFE_RELEASE(pEnumerator);
    SAFE_RELEASE(pDevice);
    SAFE_RELEASE(pAudioClient);
    SAFE_RELEASE(pCaptureClient);
    CoUninitialize();
}

void SoundLight::Start(bool fourrier) {
    try {
        double amorti[NB_AMORTISSEMENT];
        for (int i = 0; i < NB_AMORTISSEMENT; i++) {
            amorti[i] = 0;
        }
        char buffer[NB_LEDS * 3 + 2];

        SoundLightColor::initGradient();

        buffer[0] = 2;
        buffer[1] = 1;

        for (int i = 0; i < NB_LEDS; i++) {
            buffer[i * 3 + 2] = 0;
            buffer[i * 3 + 3] = 0;
            buffer[i * 3 + 4] = 0;
        }

        uint8_t IP[] = { 192, 168, 1, this->lastIp };
        SOCKADDR_IN address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = *(ULONG*)IP;
        address.sin_port = htons(21324);

        SOCKET datagramSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (datagramSocket == INVALID_SOCKET) {
            std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return;
        }

        int frame = 0;
        int amortiId = 0;
        auto lastDate = std::chrono::high_resolution_clock::now();
        auto lastUpdate = std::chrono::high_resolution_clock::now();
        int nbVal = 0;
        double moyLevel = 0.0;
        double minLevel = 1.0;
        double level;
        double spin;
        if (reverse) {
            spin = NB_LEDS/2;
        } else {
            spin = 0;
        }
        int NB_VAL = 50;

        double* signal[NB_AMORTISSEMENT];
        for (int i = 0; i < NB_AMORTISSEMENT; ++i) {
            signal[i] = new double[NB_VAL];
            for (int j = 0; j < NB_VAL; ++j) {
                signal[i][j] = 0;
            }
        }
        double signalAmorti[NB_VAL];
        int signalAmortiId = 0;

        std::cout << "avant initWASAPI" << std::endl;
        HRESULT hr = initializeWASAPI();
        if (FAILED(hr)) {
            std::cerr << "Failed to initialize WASAPI: " << hr << std::endl;
            return;
        }

        std::cout << "avant audioThread" << std::endl;
        std::thread audioThread(&SoundLight::captureAudio, this);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));


        while (true) {
            loopRenderCount++;
            if (audioLevel < minLevel) {
                minLevel = audioLevel;
            }
            moyLevel += audioLevel;
            nbVal +=1;

            auto currentDate = std::chrono::high_resolution_clock::now();
            std::this_thread::sleep_for(std::chrono::milliseconds(17 - int(std::chrono::duration<double, std::milli>(currentDate - lastUpdate).count())));
            lastUpdate = currentDate;
            amorti[amortiId] = moyLevel / nbVal;
            amortiId += 1;
            if (amortiId >= NB_AMORTISSEMENT) {
                amortiId = 0;
            }
            signalAmortiId++;
            if (signalAmortiId >= NB_AMORTISSEMENT) {
                signalAmortiId = 0;
            }


            isChangingFourrier.lock();
//                    std::cout << "avant compute FFT" << std::endl;
            computeFFT(signal[signalAmortiId],fourrierValues, fourrierSize, NB_VAL);
            isChangingFourrier.unlock();

//                    std::cout << "avant amorti" << std::endl;
            level = getAmorti(amorti, NB_AMORTISSEMENT);
            getAmorti(signalAmorti, signal, NB_VAL, NB_AMORTISSEMENT, signalAmortiId);
            if (maxFreq > 0) {
                double quantite = 500.0 / maxFreq;
//                        std::cout << "tourne : " << quantite << std::endl;
                //spin += quantite;
            }
//                    std::cout << "setBytes" << std::endl;

            if (fourrier) {
                setBytes(buffer, signalAmorti, NB_VAL, spin);
            } else {
                setBytes(buffer, level, spin);
            }

            sendto(datagramSocket, buffer, sizeof(buffer), 0, (SOCKADDR*)&address, sizeof(address));
            moyLevel = 0.0;
            minLevel = 1.0;
            nbVal = 0;

            frame += 1;
            if (frame % 100 == 0) {
                std::cout << 100000.0 / std::chrono::duration<double, std::milli>(currentDate - lastDate).count()
                          << " fps  ; captureLoopCount : " << (loopCaptureCount * 1000.0) / std::chrono::duration<double, std::milli>(currentDate - lastDate).count() << " ; renderLoopCount : " << (loopRenderCount * 1000.0) / std::chrono::duration<double, std::milli>(currentDate - lastDate).count() << std::endl;
                lastDate = currentDate;
                loopCaptureCount = 0;
                loopRenderCount = 0;
            }
        }

        audioThread.join();

    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}