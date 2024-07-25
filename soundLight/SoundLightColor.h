//
// Created by secondaire on 25/07/2024.
//

#ifndef WLEDCONTROLLERCPP_AMBILIGHTCOLOR_H
#define WLEDCONTROLLERCPP_AMBILIGHTCOLOR_H

#include <iostream>
#include <winsock2.h>

#endif //WLEDCONTROLLERCPP_AMBILIGHTCOLOR_H

class SoundLightColor {
public:
    static const int gradient_size = 768;
    static SoundLightColor gradient[gradient_size];

    static void initGradient();
    static SoundLightColor getGradient(int value);

    byte red;
    byte green;
    byte blue;

    SoundLightColor();
    SoundLightColor(byte red, byte green, byte blue);
};