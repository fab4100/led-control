/* File:   main.cpp */
/* Date:   Wed 25 Mar 2015 10:04:48 PM CET */
/* Author: Fabian Wermelinger */
/* Tag:    Main application */
/* Copyright 2015 Fabian Wermelinger. All Rights Reserved. */
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <time.h>
#include <cmath>

#include "ArgumentParser.h"
#include "led_spi_controller.h"
#include "apa102.h"

using namespace std;

typedef typename APA102::LED LED;

float f(const float t, const unsigned int N)
{
    return 0.5*(sin(2*2*M_PI*t) + 1.0f)*N;
}

int R(int i, int N)
{
    if (i < (int)0.2*N) return 255;
    else if (i >= (int)0.2*N && i < (int)0.4*N) 255 - (int)(255/0.2 * (float)i/N);
    else return 0;
}

int G(int i, int N)
{
    if (i < (int)0.2*N) return 0;
    else if (i >= (int)0.2*N && i < (int)0.4*N) (int)255/0.2 * ((float)i/N-0.2);
    else if (i >= (int)0.4*N && i < (int)0.6*N) 255;
    else if (i >= (int)0.6*N && i < (int)0.8*N) 255 - (int)(255/0.2 * (float)i*N);
    else return 255;
}

int B(int i, int N)
{
    if (i < (int)0.6*N) return 0;
    else if (i >= (int)0.6*N && i < (int)0.8*N) (int)255/0.2 * ((float)i/N-0.6);
    else return 255;
}

int main(int argc, char *argv[])
{
    ArgumentParser parser(argc, (const char**)argv);

    const unsigned int Nleds = parser("-N").asInt(144);
    const int channel = parser("-channel").asInt(0);
    const int mode = parser("-mode").asInt(1);
    const int speed = parser("-speed").asInt(600*1000);

    const int brightness = parser("-brightness").asInt(100);
    const int spread = parser("-spread").asInt(15);

    led_spi_controller<APA102> myleds(Nleds, speed, channel, mode);

    /* const unsigned int brightness = 100; */
    const unsigned int wait = 1000000;

    /* for (unsigned int i=0; i<myleds.size(); ++i) */
    /* { */
    /*     LED led(brightness, 0xff0000); */
    /*     myleds.set_led(i, led); */
    /* } */
    /* myleds.update(); */
    /* usleep(wait); */

    /* for (unsigned int i=0; i<myleds.size(); ++i) */
    /* { */
    /*     LED led(brightness, 0x00ff00); */
    /*     myleds.set_led(i, led); */
    /* } */
    /* myleds.update(); */
    /* usleep(wait); */

    /* for (unsigned int i=0; i<myleds.size(); ++i) */
    /* { */
    /*     LED led(brightness, 0x0000ff); */
    /*     myleds.set_led(i, led); */
    /* } */
    /* myleds.update(); */
    /* usleep(wait); */

    /* for (unsigned int i=0; i<myleds.size(); ++i) */
    /* { */
    /*     LED led(brightness, 0xffffff); */
    /*     myleds.set_led(i, led); */
    /* } */
    /* myleds.update(); */
    /* while (true) {} */
    /* usleep(wait); */

    /* for (unsigned int i=0; i<myleds.size(); ++i) */
    /* { */
    /*     LED led(brightness, 0x000000); */
    /*     myleds.set_led(i, led); */
    /* } */
    /* myleds.update(); */
    /* usleep(wait); */

    /* int flipflop = 0; */
    /* for (;;) */
    /* { */
    /*     if (flipflop) */
    /*     { */
    /*         LED led1(brightness, 0x000000); */
    /*         myleds.set_led(0, led1); */
    /*         LED led2(brightness, 0x00ff00); */
    /*         myleds.set_led(1, led2); */
    /*         LED led3(brightness, 0xe00000); */
    /*         myleds.set_led(2, led3); */
    /*     } */
    /*     else */
    /*     { */
    /*         LED led(brightness, 0xff0000); */
    /*         myleds.set_led(0, led); */
    /*     } */
    /*     myleds.update(); */
    /*     usleep(200000); */
    /*     flipflop = 1-flipflop; */
    /* } */

    /* srand(time(NULL)); */
    /* while (true) */
    /* { */
    /*     for (unsigned int i=0; i<myleds.size(); ++i) */
    /*     { */
    /*         LED led(brightness, rand()%256, rand()%256, rand()%256); */
    /*         myleds.set_led(i, led); */
    /*     } */
    /*     myleds.update(); */
    /*     usleep(50000); */
    /* } */

    const LED red(brightness, 0xff0000);
    const LED green(brightness, 0x00ff00);
    const LED blue(brightness, 0x0000ff);
    const LED white(brightness, 0xffffff);
    const LED off(brightness, 0x000000);

    for (unsigned int i=0; i<myleds.size(); ++i)
    {
        LED led(brightness, 0x000000);
        myleds.set_led(i, led);
    }
    myleds.update();

    /* for (unsigned int j=0; j<myleds.size(); ++j) */
    /* { */
    /*     for (unsigned int i=0; i<myleds.size(); ++i) */
    /*     { */
    /*         if (j<=i) myleds.set_led(i, red); */
    /*         else myleds.set_led(i, off); */
    /*     } */
    /*     myleds.update(); */
    /*     usleep(2000); */
    /* } */
    float t = 0;
    const float dt = 0.001;
    /* const int spread = 15; */
    int k = 0;

    while (true)
    {
        /* int j = f(t, Nleds); */
        int j = k%Nleds;
        for (int i=0; i<myleds.size(); ++i)
            myleds.set_led(i, off);
        for (int s=k-spread; s<=k+spread; ++s)
            {
                /* LED led(brightness, k%256, k%256, k%256); */
                myleds.set_led(s%Nleds, white);
                /* myleds.set_led(s%Nleds, led); */
            }
        myleds.update();
        usleep((int)dt*1000000);
        ++k;
        t += dt;
    }



    return 0;
}
