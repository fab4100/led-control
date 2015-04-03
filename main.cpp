/* File:   main.cpp */
/* Date:   Wed 25 Mar 2015 10:04:48 PM CET */
/* Author: Fabian Wermelinger */
/* Tag:    Main application */
/* Copyright 2015 Fabian Wermelinger. All Rights Reserved. */
#include <iostream>
#include <string>
#include <map>
#include <unistd.h>
#include <cstdlib>
#include <time.h>
#include <cmath>

#include "ArgumentParser.h"
#include "led_spi_controller.h"
#include "apa102.h"

using namespace std;

typedef typename APA102::LED LED;
typedef led_spi_controller<APA102> mycontroller;
typedef mycontroller::iterator myiterator;


void default_demo(mycontroller& leds, ArgumentParser& parser)
{
    leds.all_black();
    LED colors[4];
    colors[0] = LED(0xffffff); // white
    colors[1] = LED(0xff0000); // red
    colors[2] = LED(0x00ff00); // green
    colors[3] = LED(0x0000ff); // blue

    const unsigned int wait_ms = parser("-wait_ms").asInt(20);
    unsigned int k = 0;

    while (true)
    {
        for (unsigned int i=0; i < leds.size(); ++i)
        {
            leds[i] = colors[k%4];
            leds.update();
            usleep(wait_ms * 1000);
        }
        ++k;

        for (unsigned int i=leds.size()-1; i >= 0; --i)
        {
            leds[i] = colors[k%4];
            leds.update();
            usleep(wait_ms * 1000);
        }
        ++k;
    }
}

void random_lights(mycontroller& leds, ArgumentParser& parser)
{
    srand(time(NULL));

    const unsigned int wait_ms = parser("-wait_ms").asInt(60);
    while (true)
    {
        for (myiterator i=leds; i.more(); ++i)
        {
            const LED rand_led(rand()%256, rand()%256, rand()%256);
            *i = rand_led;
        }
        leds.update();
        usleep(wait_ms * 1000);
    }
}

void brightness(mycontroller& leds, ArgumentParser& parser)
{
    leds.all_black();
    unsigned int colors[4] = {0xffffff, 0xff0000, 0x00ff00, 0x0000ff};

    const unsigned int wait_ms = parser("-wait_ms").asInt(60);
    unsigned int k = 0;

    while (true)
    {
        for (myiterator i=leds; i.more(); ++i)
            i->set_color(colors[k%4]);
        ++k;
        for (unsigned int b=0; b < 32; ++b)
        {
            for (myiterator i=leds; i.more(); ++i)
                i->set_brightness31(b);
            leds.update();
            usleep(wait_ms * 1000);
        }
        for (unsigned int b=30; b >= 0; --b)
        {
            for (myiterator i=leds; i.more(); ++i)
                i->set_brightness31(b);
            leds.update();
            usleep(wait_ms * 1000);
        }
    }
}

void cycle(mycontroller& leds, ArgumentParser& parser)
{
}

/* float f(const float t, const unsigned int N) */
/* { */
/*     return 0.5*(sin(2*2*M_PI*t) + 1.0f)*N; */
/* } */

int main(int argc, char *argv[])
{
    ArgumentParser parser(argc, (const char**)argv);

    const unsigned int Nleds = parser("-N").asInt(144);
    const int channel = parser("-channel").asInt(0);
    const int mode = parser("-mode").asInt(1);
    const int speed = parser("-speed").asInt(600*1000);

    mycontroller myleds(Nleds, speed, channel, mode, SPI_DOWN);

    // run demo
    const string demo = parser("-demo").asString("default");
    if (demo == "random_lights") random_lights(myleds, parser);
    else if (demo == "brightness") brightness(myleds, parser);
    else if (demo == "cycle") cycle(myleds, parser);
    else if (demo == "default") default_demo(myleds, parser);
    else
    {
        cerr << "Unknown demo \"" << demo << "\"" << endl;
        exit(EXIT_FAILURE);
    }


    /* const int brightness = parser("-brightness").asInt(100); */
    /* const int spread = parser("-spread").asInt(15); */


    /* myleds.all_color(0xff0000); */
    /* myleds.all_brightness31(12); */

    /* myleds.state(); */


    /* const unsigned int brightness = 100; */
    /* const unsigned int wait = 1000000; */

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

    /* const LED red(brightness, 0xff0000); */
    /* const LED green(brightness, 0x00ff00); */
    /* const LED blue(brightness, 0x0000ff); */
    /* const LED white(brightness, 0xffffff); */
    /* const LED off(brightness, 0x000000); */

    /* for (unsigned int i=0; i<myleds.size(); ++i) */
    /* { */
    /*     /1* LED led(brightness, 0x000000); *1/ */
    /*     /1* myleds.set_led(i, led); *1/ */
    /* } */
    /* myleds.update(); */

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
    /* float t = 0; */
    /* const float dt = 0.001; */
    /* /1* const int spread = 15; *1/ */
    /* int k = 0; */

    /* while (true) */
    /* { */
    /*     /1* int j = f(t, Nleds); *1/ */
    /*     /1* int j = k%Nleds; *1/ */
    /*     /1* for (int i=0; i<myleds.size(); ++i) *1/ */
    /*     /1*     myleds.set_led(i, off); *1/ */
    /*     /1* for (int s=k-spread; s<=k+spread; ++s) *1/ */
    /*     /1*     { *1/ */
    /*     /1*         /2* LED led(brightness, k%256, k%256, k%256); *2/ *1/ */
    /*     /1*         myleds.set_led(s%Nleds, white); *1/ */
    /*     /1*         /2* myleds.set_led(s%Nleds, led); *2/ *1/ */
    /*     /1*     } *1/ */
    /*     myleds.update(); */
    /*     usleep((int)dt*1000000); */
    /*     ++k; */
    /*     t += dt; */
    /* } */



    return 0;
}
