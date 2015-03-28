/* File:   main.cpp */
/* Date:   Wed 25 Mar 2015 10:04:48 PM CET */
/* Author: Fabian Wermelinger */
/* Tag:    Main application */
/* Copyright 2015 Fabian Wermelinger. All Rights Reserved. */
#include <iostream>

#include "ArgumentParser.h"
#include "led_spi_controller.h"
#include "apa102.h"

using namespace std;

typedef typename APA102::LED LED;

int main(int argc, char *argv[])
{
    ArgumentParser parser(argc, (const char**)argv);

    const unsigned int Nleds = parser("-N").asInt(144);
    const int channel = parser("-channel").asInt(0);
    const int mode = parser("-mode").asInt(0);
    const int speed = parser("-speed").asInt(8*1000000);

    led_spi_controller<APA102> myleds(Nleds, speed, channel,  mode);

    for (unsigned int i=0; i<myleds.size(); ++i)
    {
        LED led(30, 0xffffff);
        myleds.set_led(i, led);
    }
    myleds.update();



    return 0;
}
