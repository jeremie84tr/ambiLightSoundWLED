#include <iostream>
#include <cstdint>
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>
#include <windef.h>
#include <wingdi.h>
#include <chrono>
#include <thread>
#include <ws2tcpip.h>

class Ambilight {
private:
    int nScreenWidth;
    int nScreenHeight;
    HDC hCaptureDC;
    HDC hDesktopDC;
    HBITMAP hCaptureBitmap;

    int HAUT = 132;
    int GAUCHE = 97;
    int BAS = 132;
    int DROITE = 97;

    void setBytes(char* buffer) {

        //std::cout << "debut setBytes" << std::endl;

        RGBQUAD *pPixels = getRgbquad(10,10,nScreenWidth - 10, 1);

        //std::cout << "screen printed" << std::endl;
        double coef = static_cast<double>(nScreenWidth - 20) / static_cast<double>(HAUT);
        int i = 0;
        int indice;
        while (i < HAUT) {
            indice = (HAUT - i) * coef;
            buffer[i * 3 + 2] = pPixels[indice].rgbRed;
            buffer[i * 3 + 3] = pPixels[indice].rgbGreen;
            buffer[i * 3 + 4] = pPixels[indice].rgbBlue;
            i += 1;
        }
        delete[] pPixels;
        DeleteObject(hCaptureBitmap);
        // Libérez les ressources


        pPixels = getRgbquad(10,10,1, nScreenHeight - 10);
        coef = static_cast<double>(nScreenHeight - 20) / static_cast<double>(GAUCHE);
        i = 0;
        while (i < GAUCHE) {
            indice = (GAUCHE - i) * coef;
            buffer[(i + HAUT) * 3 + 2] = pPixels[indice].rgbRed;
            buffer[(i + HAUT) * 3 + 3] = pPixels[indice].rgbGreen;
            buffer[(i + HAUT) * 3 + 4] = pPixels[indice].rgbBlue;
            i += 1;
        }
        delete[] pPixels;
        DeleteObject(hCaptureBitmap);
        // Libérez les ressources


        pPixels = getRgbquad(10,nScreenHeight - 10,nScreenWidth - 10, 1);
        coef = static_cast<double>(nScreenWidth - 20) / static_cast<double>(BAS);
        i = 0;
        while (i < BAS) {
            indice = i * coef;
            buffer[(i + HAUT + GAUCHE) * 3 + 2] = pPixels[indice].rgbRed;
            buffer[(i + HAUT + GAUCHE) * 3 + 3] = pPixels[indice].rgbGreen;
            buffer[(i + HAUT + GAUCHE) * 3 + 4] = pPixels[indice].rgbBlue;
            i += 1;
        }
        delete[] pPixels;
        DeleteObject(hCaptureBitmap);
        // Libérez les ressources


        pPixels = getRgbquad(nScreenWidth - 10,10,1, nScreenHeight - 10);
        coef = static_cast<double>(nScreenHeight - 20) / static_cast<double>(DROITE);
        i = 0;
        while (i < DROITE) {
            indice = i * coef;
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
        nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
        nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
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

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    Ambilight ambilight;
    ambilight.start();



    return 0;
}