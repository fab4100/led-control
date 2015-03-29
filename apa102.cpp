/* File:   apa102.cpp */
/* Date:   Wed 25 Mar 2015 10:03:22 PM CET */
/* Author: Fabian Wermelinger */
/* Tag:    Implements APA102 LED */
/* Copyright 2015 Fabian Wermelinger. All Rights Reserved. */
#include <cstdio>
#include "apa102.h"

using namespace std;


APA102::APA102(const unsigned int n) : n_led(n),
    nbits_start(32), nbits_end(32*(n/65 + 1)),
    rawdata((nbits_start/8 + n*sizeof(LED) + nbits_end/8), 0xff),
    leds((LED*)(&rawdata[0] + nbits_start/8))
{
    for (unsigned int i=0; i<nbits_start/8; ++i)
        rawdata[i] = 0;
}

APA102::~APA102()
{
}

// LED implementation
APA102::LED::LED(const unsigned int bright, const uint8_t red, const uint8_t green, const uint8_t blue)
    : brightness(0xe0), B(blue), G(green), R(red)
{
    // bright in percent
    if (bright > 100) brightness = 0xff;
    else set_brightness(bright);
}

APA102::LED::LED(const uint8_t red, const uint8_t green, const uint8_t blue)
    : brightness(0xff), B(blue), G(green), R(red)
{
}

APA102::LED::LED(const unsigned int bright, const unsigned int color)
    : brightness(0xe0), B(color), G(color>>8), R(color>>16)
{
    // bright in percent
    if (bright > 100) brightness = 0xff;
    else set_brightness(bright);
}

APA102::LED::LED(const unsigned int color)
    : brightness(0xff), B(color), G(color>>8), R(color>>16)
{
}

void APA102::LED::increase_brightness()
{
    uint8_t cur_bright = brightness & 0x1f;
    if (cur_bright < 31) ++cur_bright;
    brightness = 0xe0 + cur_bright;
}

void APA102::LED::decrease_brightness()
{
    uint8_t cur_bright = brightness & 0x1f;
    if (cur_bright > 0) --cur_bright;
    brightness = 0xe0 + cur_bright;
}

void APA102::LED::state() const
{
    printf("Brightness: %.1f%%\n", static_cast<float>(brightness & 0x1f)/31.0f*100.0f);
    printf("Red:        %i\n", R);
    printf("Green:      %i\n", G);
    printf("Blue:       %i\n", B);
    printf("Hex:        0x%.6X\n", (R<<16) + (G<<8) + B);
    printf("Address:    %p\n", this);
}
