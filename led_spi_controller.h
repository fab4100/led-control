/* File:   led_spi_controller.h */
/* Date:   Sat 28 Mar 2015 04:40:21 PM CET */
/* Author: Fabian Wermelinger */
/* Tag:    LED and SPI controller */
/* Copyright 2015 Fabian Wermelinger. All Rights Reserved. */
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <vector>

#include "wiringPi.h"
#include "wiringPiSPI.h"

enum SPI_INTERFACE {SPI_UP=0, SPI_DOWN};

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
            const int mode = 0,
            const SPI_INTERFACE spi_state = SPI_UP)
    : led_array(n), spi_speed(speed), spi_channel(channel), spi_mode(mode), file_descriptor(-1)
    {
        if (spi_state == SPI_UP)
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
        else
        {}
    }

    ~led_spi_controller() { close(file_descriptor); }

    inline int channel() const { return spi_channel; }
    inline int mode() const { return spi_mode; }
    inline int speed() const { return spi_speed; }

    void update()
    {
        // use a copy of raw data, because wiringPiSPIDataRW will likely
        // overwrite but we want read-only behavior
        std::vector<typename LED_Type::RAWTYPE> c(led_array.datasize(), 0);
        const typename LED_Type::RAWTYPE* data = led_array.data();
        for (unsigned int i=0; i < c.size(); ++i)
            c[i] = *data++;

        if (wiringPiSPIDataRW(spi_channel, &c[0], c.size()) == -1)
        {
            std::cerr << "SPI transmission failure!" << std::endl;
        }
    }

    void state() const
    {
        for (unsigned int i=0; i < led_array.size(); ++i)
        {
            std::cout << "{ ID = " << i << std::endl;
            led_array[i].state();
            std::cout << "}" << std::endl << std::endl;
        }
    }

    inline unsigned int size() const { return led_array.size(); }
    inline typename LED_Type::LED& operator[](const unsigned int i) { return led_array[i]; }
    inline const typename LED_Type::LED& operator[](const unsigned int i) const { return led_array[i]; }

    class iterator
    {
        typename LED_Type::LED* cur;
        typename LED_Type::LED* end;

    public:
        iterator(led_spi_controller<LED_Type>& iterable) : cur(iterable.led_array.begin()), end(iterable.led_array.end()) {}
        iterator(LED_Type& iterable) : cur(iterable.begin()), end(iterable.end()) {}
        iterator(const iterator& c) : cur(c.cur), end(c.end) {}

        inline bool more() const { return (cur == end) ? false : true; }

        // ++A
        inline iterator& operator++() { ++cur; return *this; }

        // A++
        iterator operator++(int)
        {
            iterator tmp = *this;
            ++cur;
            return tmp;
        }

        iterator& operator=(const iterator& rhs)
        {
            cur = rhs.cur;
            end = rhs.end;
            return *this;
        }

        inline bool operator!=(const typename LED_Type::LED* const rhs) { return (cur == rhs) ? false : true; }
        inline typename LED_Type::LED& operator*() { return *cur; }
        inline const typename LED_Type::LED& operator*() const { return *cur; }
        inline typename LED_Type::LED* operator->() { return cur; }
    };

    // handy shortcuts
    void all_white()
    {
        const typename LED_Type::LED white(255, 255, 255);
        for (iterator i = led_array; i.more(); ++i)
            *i = white;
    }

    void all_black()
    {
        const typename LED_Type::LED black(0, 0, 0);
        for (iterator i = led_array; i.more(); ++i)
            *i = black;
    }

    void all_color(const unsigned int red, const unsigned int green, const unsigned int blue)
    {
        const typename LED_Type::LED color(red, green, blue);
        for (iterator i = led_array; i.more(); ++i)
            *i = color;
    }

    void all_color(const unsigned int hex_color)
    {
        const typename LED_Type::LED color(hex_color);
        for (iterator i = led_array; i.more(); ++i)
            *i = color;
    }

    inline void all_brightness(const unsigned int bright_percent)
    {
        for (iterator i = led_array; i.more(); ++i)
            i->set_brightness(bright_percent);
    }

    inline void all_brightness31(const unsigned int bright31)
    {
        for (iterator i = led_array; i.more(); ++i)
            i->set_brightness31(bright31);
    }
};
