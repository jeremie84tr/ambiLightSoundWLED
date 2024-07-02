#include <iostream>
#include <cstdint>
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>
#include <windef.h>
#include <wingdi.h>
#include <chrono>
#include <thread>

class Ambilight {
private:
    int nScreenWidth;
    int nScreenHeight;
    HDC hCaptureDC;
    HDC hDesktopDC;
    HBITMAP hCaptureBitmap;

    int BLACK = 40;

    int HAUT = 132;
    int GAUCHE = 97;
    int BAS = 132;
    int DROITE = 97;

    int hbiais = 5;

    int bbiais = 5;

    void setBytes(char* buffer) {

        //std::cout << "debut setBytes" << std::endl;

        RGBQUAD *pPixels = getRgbquad(10,10,nScreenWidth - 20, nScreenHeight - 20);

        //std::cout << "screen printed" << std::endl;
        double coef = static_cast<double>(nScreenWidth - 20) / static_cast<double>(HAUT);
        int i = 0;
        int indice;
        bool blackH = true;
        bool blackB = true;
        while (i < HAUT) {
            indice = (HAUT - i - 1) * coef + (nScreenHeight - 21) * (nScreenWidth - 20) - hbiais * (nScreenWidth - 20);
            buffer[i * 3 + 2] = pPixels[indice].rgbRed;
            blackH &= pPixels[indice].rgbRed <= BLACK;
            blackB &= pPixels[indice + 5 * (nScreenWidth - 20)].rgbRed <= BLACK;
            buffer[i * 3 + 3] = pPixels[indice].rgbGreen;
            blackH &= pPixels[indice].rgbGreen <= BLACK;
            blackB &= pPixels[indice + 5 * (nScreenWidth - 20)].rgbGreen <= BLACK;
            buffer[i * 3 + 4] = pPixels[indice].rgbBlue;
            blackH &= pPixels[indice].rgbBlue <= BLACK;
            blackB &= pPixels[indice + 5 * (nScreenWidth - 20)].rgbBlue <= BLACK;
            i += 1;
        }
        if (blackH && hbiais < nScreenHeight / 2) {
            hbiais += 5;
        } else if (!blackB && hbiais > 6) {
            hbiais -= 5;
        }
//        std::cout << "blackH : " << blackH << std::endl;
//        std::cout << "blackB : " << blackB << std::endl;
//        std::cout << "hbiais : " << hbiais << std::endl;


        coef = static_cast<double>(nScreenHeight - 20 - bbiais - hbiais) / static_cast<double>(GAUCHE);
        i = 0;
        while (i < GAUCHE) {
            indice = int((GAUCHE - i) * coef) * (nScreenWidth - 20) + (bbiais) * (nScreenWidth - 20) ;
            buffer[(i + HAUT) * 3 + 2] = pPixels[indice].rgbRed;
            buffer[(i + HAUT) * 3 + 3] = pPixels[indice].rgbGreen;
            buffer[(i + HAUT) * 3 + 4] = pPixels[indice].rgbBlue;
            i += 1;
        }


        coef = static_cast<double>(nScreenWidth - 20) / static_cast<double>(BAS);
        i = 0;
        blackH = true;
        blackB = true;
        while (i < BAS) {
            indice = i * coef + (nScreenWidth - 20) * 10 + bbiais * (nScreenWidth - 20);
            buffer[(i + HAUT + GAUCHE) * 3 + 2] = pPixels[indice].rgbRed;
            blackH &= pPixels[indice].rgbRed <= BLACK;
            blackB &= pPixels[indice - 5 * (nScreenWidth - 20)].rgbRed <= BLACK;
            buffer[(i + HAUT + GAUCHE) * 3 + 3] = pPixels[indice].rgbGreen;
            blackH &= pPixels[indice].rgbGreen <= BLACK;
            blackB &= pPixels[indice - 5 * (nScreenWidth - 20)].rgbGreen <= BLACK;
            buffer[(i + HAUT + GAUCHE) * 3 + 4] = pPixels[indice].rgbBlue;
            blackH &= pPixels[indice].rgbBlue <= BLACK;
            blackB &= pPixels[indice - 5 * (nScreenWidth - 20)].rgbBlue <= BLACK;
            i += 1;
        }
        if (blackH && bbiais < nScreenHeight / 2) {
            bbiais += 5;
        } else if (!blackB && bbiais > 6) {
            bbiais -= 5;
        }

        //std::cout << "bbiais : " << bbiais << std::endl;


        coef = static_cast<double>(nScreenHeight - 20 - bbiais - hbiais) / static_cast<double>(DROITE);
        i = 0;
        while (i < DROITE) {
            indice = int(i * coef) * (nScreenWidth - 20) + (nScreenWidth - 21) + (bbiais) * (nScreenWidth - 20);
            buffer[(i + HAUT + GAUCHE + BAS) * 3 + 2] = pPixels[indice].rgbRed;
            buffer[(i + HAUT + GAUCHE + BAS) * 3 + 3] = pPixels[indice].rgbGreen;
            buffer[(i + HAUT + GAUCHE + BAS) * 3 + 4] = pPixels[indice].rgbBlue;
            i += 1;
        }
        delete[] pPixels;
        DeleteObject(hCaptureBitmap);
        // Libérez les ressources



    }

    RGBQUAD *getRgbquad(int x, int y, int width, int height) {
        BitBlt(hCaptureDC, 0, 0, width, height, hDesktopDC, x, y, SRCCOPY | CAPTUREBLT);

        BITMAPINFO bmi = {0};
        bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD *pPixels = new RGBQUAD[width * height];

        GetDIBits(
                hCaptureDC,
                hCaptureBitmap,
                0,
                height,
                pPixels,
                &bmi,
                DIB_RGB_COLORS
        );
        return pPixels;
    }

public:
    void start() {
        nScreenWidth = 1920; //GetSystemMetrics(SM_CXSCREEN);
        nScreenHeight = 1080; //GetSystemMetrics(SM_CYSCREEN);
        HWND hDesktopWnd = GetDesktopWindow();
        hDesktopDC = GetDC(hDesktopWnd);
        hCaptureDC = CreateCompatibleDC(hDesktopDC);
        hCaptureBitmap = CreateCompatibleBitmap(hDesktopDC, nScreenWidth, nScreenHeight);
        SelectObject(hCaptureDC, hCaptureBitmap);

        try {
            std::cout << "Hello world!" << std::endl;

            const int NB_LEDS = 458;

            char buffer[NB_LEDS * 3 + 2];

            buffer[0] = 2;
            buffer[1] = 1;

            for (int i = 0; i < NB_LEDS; i++) {
                buffer[i * 3 + 2] = 0;
                buffer[i * 3 + 3] = 0;
                buffer[i * 3 + 4] = 0;
            }

            uint8_t IP[] = {192, 168, 1, 5};
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

            auto lastDate = std::chrono::high_resolution_clock::now();

            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            while (true) {
                sendto(datagramSocket, buffer, sizeof(buffer), 0, (SOCKADDR*)&address, sizeof(address));

                frame += 1;
                setBytes(buffer);
                std::this_thread::sleep_for(std::chrono::milliseconds(0));
                if (frame % 100 == 0) {
                    auto currentDate = std::chrono::high_resolution_clock::now();
                    std::cout << 100000.0 / std::chrono::duration<double, std::milli>(currentDate - lastDate).count() << " fps" << std::endl;
                    lastDate = currentDate;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
};

#include <iostream>
#include <chrono>
#include <thread>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cmath>
#include <complex>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Ole32.lib")

#define REFTIMES_PER_SEC  10
#define REFTIMES_PER_MILLISEC  10000

#define EXIT_ON_ERROR(hres)  \
              if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

class SoundLight {
private:
    void setBytes(char* buffer, double audioLevel, double tourneIn = 0) {
        uint16_t level = static_cast<uint16_t>(audioLevel * NB_LEDS / 4);
        int tourne = tourneIn;
        int decalage = 65;
        for (int i = decalage; i < NB_LEDS + decalage; ++i) {
            if (i - decalage < level || NB_LEDS - i + decalage < level) {
                buffer[((i + tourne) % NB_LEDS) * 3 + 2] = 64;
                buffer[((i + tourne) % NB_LEDS) * 3 + 3] = 255;
                buffer[((i + tourne) % NB_LEDS) * 3 + 4] = 64;
            } else if (i - decalage - (NB_LEDS / 2) < level && NB_LEDS - i + decalage - (NB_LEDS / 2) < level) {
                buffer[((i + tourne) % NB_LEDS) * 3 + 2] = 64;
                buffer[((i + tourne) % NB_LEDS) * 3 + 3] = 64;
                buffer[((i + tourne) % NB_LEDS) * 3 + 4] = 255;
            } else {
                buffer[((i + tourne) % NB_LEDS) * 3 + 2] = 0;
                buffer[((i + tourne) % NB_LEDS) * 3 + 3] = 0;
                buffer[((i + tourne) % NB_LEDS) * 3 + 4] = 0;
            }
        }
    }
    void setBytes(char* buffer, double* audioLevels, int nbVals, double tourneIn = 0) {
        uint16_t level = 50;
        int tourne = tourneIn;
        int decalage = 65;

        int usedValue;
        for (int i = decalage; i < NB_LEDS + decalage; ++i) {
            if (i - decalage < level || NB_LEDS - i + decalage < level) {
                usedValue = (i - decalage);
                if (usedValue < nbVals && usedValue > 0) {
                    buffer[((i + tourne) % NB_LEDS) * 3 + 2] = audioLevels[usedValue] * 64.0;
                    buffer[((i + tourne) % NB_LEDS) * 3 + 3] = audioLevels[usedValue] * 255.0;
                    buffer[((i + tourne) % NB_LEDS) * 3 + 4] = audioLevels[usedValue] * 64.0;
                }
            } else if (i - decalage - (NB_LEDS / 2) < level && NB_LEDS - i + decalage - (NB_LEDS / 2) < level) {
                usedValue = (i - decalage - (NB_LEDS / 2));
                if (usedValue < nbVals && usedValue > 0) {
                    buffer[((i + tourne) % NB_LEDS) * 3 + 2] = audioLevels[usedValue] * 64.0;
                    buffer[((i + tourne) % NB_LEDS) * 3 + 3] = audioLevels[usedValue] * 64.0;
                    buffer[((i + tourne) % NB_LEDS) * 3 + 4] = audioLevels[usedValue] * 255.0;
                }
            } else {
                buffer[((i + tourne) % NB_LEDS) * 3 + 2] = 255;
                buffer[((i + tourne) % NB_LEDS) * 3 + 3] = 0;
                buffer[((i + tourne) % NB_LEDS) * 3 + 4] = 0;
            }
        }
    }

    const double PI = 3.141592653589793238460;

    void fft(std::complex<double>* a, int n) {
        if (n <= 1) return;

        // Diviser
        std::vector<std::complex<double>> a0(n / 2), a1(n / 2);

        for (int i = 0; 2 * i + 1 < n; i++) {
            a0[i] = a[i*2];
            a1[i] = a[i*2 + 1];
        }

        // Conquérir
        fft(a0.data(), n / 2);
        fft(a1.data(), n / 2);

        // Combiner
        for (int i = 0; 2 * i < n; i++) {
            std::complex<double> t = std::polar(1.0, -2 * PI * i / n) * a1[i];
            a[i] = a0[i] + t;
            a[i + n / 2] = a0[i] - t;
        }
    }

    void computeFFT(double* output, double* input, int N, int nbVal) {
        std::vector<std::complex<double>> signal(N);

        // Convertir l'entrée en complexe
        for (int i = 0; i < N; ++i) {
            signal[i] = std::complex<double>(input[i], 0);
        }

        // Calculer la FFT
        fft(signal.data(), N);

        // Extraire les magnitudes
        for (int i = 0; i < nbVal; ++i) {
            output[i] = std::abs(signal[i]);
        }
    }

    void processAudio(BYTE* pData, UINT32 numFrames) {
        float* data = (float*)pData;
        double max = 0.0;
        double val;
        for (UINT32 i = 0; i < numFrames; ++i) {
            val = data[i] * data[i];
            if (val > max) {
                max = val;
            }
        }

        audioLevel = max;
    }

    HRESULT initializeWASAPI() {
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

    void captureAudio() {
        HRESULT hr;
        UINT32 packetLength = 0;
        BYTE* pData;
        DWORD flags;
        UINT32 numFramesAvailable;

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

            while (packetLength != 0) {
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
                    processAudio(pData, numFramesAvailable);
                }

                hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
                if (FAILED(hr)) {
                    std::cerr << "Failed to release buffer: " << hr << std::endl;
                    break;
                }

                hr = pCaptureClient->GetNextPacketSize(&packetLength);
                if (FAILED(hr)) {
                    std::cerr << "Failed to get next packet size: " << hr << std::endl;
                    break;
                }
            }
        }

        pAudioClient->Stop();
    }

    double getAmorti(double* pDouble, int nbValues) {
        double ret = 0.0;
        for (int i = 0; i < nbValues; i++) {
            ret += sqrt(pDouble[i]);
        }
        return ret / nbValues;
    }

public:
    SoundLight() : audioLevel(0.0), pEnumerator(nullptr), pDevice(nullptr), pAudioClient(nullptr), pCaptureClient(nullptr) {}

    ~SoundLight() {
        SAFE_RELEASE(pEnumerator);
        SAFE_RELEASE(pDevice);
        SAFE_RELEASE(pAudioClient);
        SAFE_RELEASE(pCaptureClient);
        CoUninitialize();
    }

    void Start() {
        try {
            int NB_AMORTISSEMENT = 5;
            double amorti[NB_AMORTISSEMENT];
            for (int i = 0; i < NB_AMORTISSEMENT; i++) {
                amorti[i] = 0;
            }
            const int NB_LEDS = 458;
            char buffer[NB_LEDS * 3 + 2];

            buffer[0] = 2;
            buffer[1] = 1;

            for (int i = 0; i < NB_LEDS; i++) {
                buffer[i * 3 + 2] = 0;
                buffer[i * 3 + 3] = 0;
                buffer[i * 3 + 4] = 0;
            }

            uint8_t IP[] = { 192, 168, 1, 5 };
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
            double spin = 0.0;

            HRESULT hr = initializeWASAPI();
            if (FAILED(hr)) {
                std::cerr << "Failed to initialize WASAPI: " << hr << std::endl;
                return;
            }

            std::thread audioThread(&SoundLight::captureAudio, this);

            std::this_thread::sleep_for(std::chrono::milliseconds(10));


            while (true) {
                if (audioLevel < minLevel) {
                    minLevel = audioLevel;
                }
                moyLevel += audioLevel;
                nbVal +=1;

                auto currentDate = std::chrono::high_resolution_clock::now();
                if (std::chrono::duration<double, std::milli>(currentDate - lastUpdate).count() > 17) {
                    lastUpdate = currentDate;
                    amorti[amortiId] = moyLevel / nbVal;
                    amortiId += 1;
                    if (amortiId >= NB_AMORTISSEMENT) {
                        amortiId = 0;
                    }

                    level = getAmorti(amorti, NB_AMORTISSEMENT);
                    //spin += ((level - 0.1) * (level - 0.1) * (level - 0.1)) * 50.0;

                    setBytes(buffer, level, spin);
                    sendto(datagramSocket, buffer, sizeof(buffer), 0, (SOCKADDR*)&address, sizeof(address));
                    moyLevel = 0.0;
                    minLevel = 1.0;
                    nbVal = 0;

                    frame += 1;
                    if (frame % 100 == 0) {
                        std::cout << 100000.0 / std::chrono::duration<double, std::milli>(currentDate - lastDate).count()
                                  << " fps" << std::endl;
                        lastDate = currentDate;
                    }
                }
            }

            audioThread.join();

        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

private:
    double audioLevel;
    double* fourrier;
    int fourrierSize = 0;
    const int NB_LEDS = 458;

    IMMDeviceEnumerator* pEnumerator;
    IMMDevice* pDevice;
    IAudioClient* pAudioClient;
    IAudioCaptureClient* pCaptureClient;
};



int main() {
    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    if (false) {
        Ambilight ambilight;
        ambilight.start();
    } else {
        SoundLight soundLight;
        soundLight.Start();
    }


    return 0;
}