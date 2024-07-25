//
// Created by secondaire on 25/07/2024.
//

#ifndef WLEDCONTROLLERCPP_AMBILIGHT_H
#define WLEDCONTROLLERCPP_AMBILIGHT_H

#include <iostream>
#include <cstdint>
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>
#include <windef.h>
#include <wingdi.h>
#include <chrono>
#include <thread>
#include <valarray>

#endif //WLEDCONTROLLERCPP_AMBILIGHT_H

class Ambilight {
private:
    int nScreenWidth;
    int nScreenHeight;
    int BLACK = 40;
    int HAUT = 132;
    int GAUCHE = 97;
    int BAS = 132;
    int DROITE = 97;
    int hbiais = 5;
    int gbiais = 5;
    int bbiais = 5;
    int dbiais = 5;
    HDC hCaptureDC;
    HDC hDesktopDC;
    HBITMAP hCaptureBitmap;

    void setBytes(char* buffer);

    RGBQUAD *getRgbquad(int x, int y, int width, int height);
public:
    void start();
};