#include "SoundLightColor.h"

// Initialize the static member outside the class definition
SoundLightColor SoundLightColor::gradient[SoundLightColor::gradient_size];

void SoundLightColor::initGradient() {
    for (int i = 0; i < gradient_size; ++i) {
        byte red = 0;
        if (i < 255) {
            red = 0;
        } else if (i < 512) {
            red = i - 255;
        } else if (i < 768) {
            red = 255;
        }
        byte green = 0;
        if (i < 255) {
            green = i;
        } else if (i < 512)
            green = 255;
        else if (i < 768) {
            green = 768 - i;
        }
        byte blue = 0;
        gradient[i] = SoundLightColor(red, green, blue);
    }
}

SoundLightColor SoundLightColor::getGradient(int value) {
    if (value >= 0) {
        if (value < gradient_size) {
            return gradient[value];
        } else {
            return gradient[gradient_size - 1];
        }
    } else {
        return gradient[0];
    }
}

SoundLightColor::SoundLightColor() {
    this->red = 0;
    this->green = 0;
    this->blue = 0;
}

SoundLightColor::SoundLightColor(byte red, byte green, byte blue) {
    this->red = red;
    this->green = green;
    this->blue = blue;
}