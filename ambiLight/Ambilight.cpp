#include "Ambilight.h"

void Ambilight::setBytes(char* buffer) {

    RGBQUAD *pPixels = getRgbquad(10,10,nScreenWidth - 20, nScreenHeight - 20);

    double coef = static_cast<double>(nScreenWidth - 20 - gbiais - dbiais) / static_cast<double>(HAUT);
    int i = 0;
    int indice;
    bool blackH = true;
    bool blackB = true;
    while (i < HAUT) {
        indice = (HAUT - i - 1) * coef + (nScreenHeight - 21) * (nScreenWidth - 20) - hbiais * (nScreenWidth - 20) + gbiais;
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


    coef = static_cast<double>(nScreenHeight - 20 - bbiais - hbiais) / static_cast<double>(GAUCHE);
    i = 0;
    bool blackD = true;
    bool blackG = true;
    while (i < GAUCHE) {
        indice = int((GAUCHE - i) * coef) * (nScreenWidth - 20) + (bbiais) * (nScreenWidth - 20) + gbiais ;
        buffer[(i + HAUT) * 3 + 2] = pPixels[indice + 5].rgbRed;
        blackG &= pPixels[indice - 5].rgbRed <= BLACK;
        blackD &= pPixels[indice].rgbRed <= BLACK;
        buffer[(i + HAUT) * 3 + 3] = pPixels[indice + 5].rgbGreen;
        blackG &= pPixels[indice - 5].rgbGreen <= BLACK;
        blackD &= pPixels[indice].rgbGreen <= BLACK;
        buffer[(i + HAUT) * 3 + 4] = pPixels[indice + 5].rgbBlue;
        blackG &= pPixels[indice - 5].rgbBlue <= BLACK;
        blackD &= pPixels[indice].rgbBlue <= BLACK;
        i += 1;
    }
    if (blackD && gbiais < nScreenWidth / 2) {
        gbiais += 5;
    } else if (!blackG && gbiais > 11) {
        gbiais -= 5;
    }


    coef = static_cast<double>(nScreenWidth - 20 - gbiais - dbiais) / static_cast<double>(BAS);
    i = 0;
    blackH = true;
    blackB = true;
    while (i < BAS) {
        indice = i * coef + (nScreenWidth - 20) * 10 + bbiais * (nScreenWidth - 20) + gbiais;
        buffer[(i + HAUT + GAUCHE) * 3 + 2] = pPixels[indice].rgbRed;
        blackB &= pPixels[indice - 5 * (nScreenWidth - 20)].rgbRed <= BLACK;
        blackH &= pPixels[indice].rgbRed <= BLACK;
        buffer[(i + HAUT + GAUCHE) * 3 + 3] = pPixels[indice].rgbGreen;
        blackB &= pPixels[indice - 5 * (nScreenWidth - 20)].rgbGreen <= BLACK;
        blackH &= pPixels[indice].rgbGreen <= BLACK;
        buffer[(i + HAUT + GAUCHE) * 3 + 4] = pPixels[indice].rgbBlue;
        blackB &= pPixels[indice - 5 * (nScreenWidth - 20)].rgbBlue <= BLACK;
        blackH &= pPixels[indice].rgbBlue <= BLACK;
        i += 1;
    }
    if (blackH && bbiais < nScreenHeight / 2) {
        bbiais += 5;
    } else if (!blackB && bbiais > 6) {
        bbiais -= 5;
    }


    coef = static_cast<double>(nScreenHeight - 20 - bbiais - hbiais) / static_cast<double>(DROITE);
    i = 0;
    blackD = true;
    blackG = true;
    while (i < DROITE) {
        indice = int(i * coef) * (nScreenWidth - 20) + (nScreenWidth - 21) + (bbiais) * (nScreenWidth - 20) - dbiais;
        buffer[(i + HAUT + GAUCHE + BAS) * 3 + 2] = pPixels[indice].rgbRed;
        blackG &= pPixels[indice].rgbRed <= BLACK;
        blackD &= pPixels[indice + 5].rgbRed <= BLACK;
        buffer[(i + HAUT + GAUCHE + BAS) * 3 + 3] = pPixels[indice].rgbGreen;
        blackG &= pPixels[indice].rgbGreen <= BLACK;
        blackD &= pPixels[indice + 5].rgbGreen <= BLACK;
        buffer[(i + HAUT + GAUCHE + BAS) * 3 + 4] = pPixels[indice].rgbBlue;
        blackG &= pPixels[indice].rgbBlue <= BLACK;
        blackD &= pPixels[indice + 5].rgbBlue <= BLACK;
        i += 1;
    }
    if (blackG && dbiais < nScreenWidth / 2) {
        dbiais += 5;
    } else if (!blackD && dbiais > 11) {
        dbiais -= 5;
    }
    delete[] pPixels;
    DeleteObject(hCaptureBitmap);
    // Libérez les ressources

}

RGBQUAD* Ambilight::getRgbquad(int x, int y, int width, int height) {
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

void Ambilight::start() {
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