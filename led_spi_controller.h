/* File:   led_spi_controller.h */
/* Date:   Sat 28 Mar 2015 04:40:21 PM CET */
/* Author: Fabian Wermelinger */
/* Tag:    LED and SPI controller */
/* Copyright 2015 Fabian Wermelinger. All Rights Reserved. */
#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "wiringPi.h"
#include "wiringPiSPI.h"

template <typename LED_Type>
class led_spi_controller
{
    LED_Type led_array;

    const int spi_speed;
    const int spi_channel;
    const int spi_mode;
    int file_descriptor;

public:
    led_spi_controller(const unsigned int n = 1,
            const int speed = 8000000,
            const int channel = 0,
            const int mode = 0)
    : led_array(n), spi_speed(speed), spi_channel(channel), spi_mode(mode)
    {
        // set up wiringPi
        wiringPiSetup();
        // setup SPI
        file_descriptor = wiringPiSPISetupMode(spi_channel, spi_speed, spi_mode);
        if (file_descriptor < 0)
        {
            std::cerr << "Can not open SPI bus" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    ~led_spi_controller() { close(file_descriptor); }

    inline int channel() const { return spi_channel; }
    inline int mode() const { return spi_mode; }
    inline int speed() const { return spi_speed; }

    inline unsigned int size() const { return led_array.size(); }
    void set_led(const unsigned int i, const typename LED_Type::LED led) { led_array.set_led(i, led); }
    void update()
    {
        if (wiringPiSPIDataRW(spi_channel, led_array.data(), led_array.datasize()) == -1)
        {
            std::cerr << "SPI transmission failure!" << std::endl;
        }
    }

    /* TODO: (Sat 28 Mar 2015 05:50:36 PM CET) implement this */
    class iterator
    {
        typename LED_Type::LED* cur;
        typename LED_Type::LED* end;

    public:
        iterator(LED_Type& iterable) : cur(iterable.begin()), end(iterable.end()) {}
    };
};
