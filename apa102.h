/* File:   apa102.h */
/* Date:   Wed 25 Mar 2015 10:15:11 PM CET */
/* Author: Fabian Wermelinger */
/* Tag:    APA102 LED */
/* Copyright 2015 Fabian Wermelinger. All Rights Reserved. */
#include <cstdint>
#include <vector>

class APA102
{
public:
    // representation of one LED
    struct LED
    {
        // order matters here, defined by APA102 datasheet
        uint8_t brightness, B, G, R;
        // color 3-tuples
        LED(const unsigned int bright, const uint8_t red, const uint8_t green, const uint8_t blue);
        LED(const uint8_t red, const uint8_t green, const uint8_t blue);
        // hex color
        LED(const unsigned int bright, const unsigned int color);
        LED(const unsigned int color);

        inline void set_brightness(const unsigned int bright)
        {
            brightness = 0xe0 + static_cast<uint8_t>(bright/100.0f * 31.0f);
        }

        inline void set_color(const uint8_t red, const uint8_t green, const uint8_t blue)
        {
            B = blue; G = green; R = red;
        }

        inline void set_color(const unsigned int hex_color)
        {
            B = hex_color; G = hex_color >> 8; R = hex_color >> 16;
        }

        inline void set_led(const LED c)
        {
            brightness = c.brightness; B = c.B; G = c.G; R = c.R;
        }

        void increase_brightness();
        void decrease_brightness();
        void state() const;
    };

private:
    const unsigned int n_led;
    const unsigned int nbits_start, nbits_end;

    std::vector<uint8_t> rawdata;
    LED * const leds;

public:
    APA102(const unsigned int n = 1);
    ~APA102();

    inline void set_led(const unsigned int i, const LED led_state) { *(leds + i) = led_state; }
    inline unsigned int size() const { return n_led; }
    inline LED * begin() const { return leds; }
    inline LED * end()   const { return leds + n_led; }

    inline unsigned int datasize() const { return rawdata.size(); }
    inline uint8_t* data() { return &rawdata[0]; }
};
